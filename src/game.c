#include "game.h"

/* TETRAMINOES [NUMBER][ROTATION][POSITION] */
POSITION tetraminoes[7][4][4] = {
    // From left to right, top to bottom
    {
        {{1,0},{1,1},{1,2},{1,3}}, /* .... */ 
        {{0,2},{1,2},{2,2},{3,2}}, /* #### */
        {{2,0},{2,1},{2,2},{2,3}}, /* .... */
        {{0,1},{1,1},{2,1},{3,1}}  /* .... */
    },

    {
        {{1,1},{1,2},{2,1},{2,2}}, /* .... */
        {{1,1},{1,2},{2,1},{2,2}}, /* .##. */
        {{1,1},{1,2},{2,1},{2,2}}, /* .##. */
        {{1,1},{1,2},{2,1},{2,2}}  /* .... */
    },

    {
        {{0,1},{1,0},{1,1},{1,2}}, /* .#.. */
        {{0,1},{1,1},{1,2},{2,1}}, /* ###. */
        {{1,0},{1,1},{1,2},{2,1}}, /* .... */
        {{0,1},{1,0},{1,1},{2,1}}  /* .... */
    },

    {
        {{0,0},{1,0},{1,1},{1,2}}, /* #... */
        {{0,1},{0,2},{1,1},{2,1}}, /* ###. */
        {{1,0},{1,1},{1,2},{2,2}}, /* .... */
        {{0,1},{1,1},{2,1},{2,0}}  /* .... */
    },

    {
        {{0,2},{1,0},{1,1},{1,2}}, /* ..#. */
        {{0,1},{1,1},{2,1},{2,2}}, /* ###. */
        {{1,0},{1,1},{1,2},{2,0}}, /* .... */
        {{0,0},{0,1},{1,1},{2,1}}  /* .... */
    },

    {
        {{0,1},{0,2},{1,0},{1,1}}, /* .##. */
        {{0,1},{1,1},{1,2},{2,2}}, /* ##.. */
        {{1,1},{1,2},{2,0},{2,1}}, /* .... */
        {{0,0},{1,0},{1,1},{2,1}}  /* .... */
    },

    {
        {{0,0},{0,1},{1,1},{1,2}}, /* ##.. */
        {{0,2},{1,1},{1,2},{2,1}}, /* .##. */
        {{1,0},{1,1},{2,1},{2,2}}, /* .... */
        {{0,1},{1,0},{1,1},{2,0}}  /* .... */
    }
};

/* Main game loop */
void game_play() {

    /* Check for size of the screen */
    getmaxyx(stdscr, LINES, COLS);
    if (LINES < (BORDER_Y + 1) || COLS < (BORDER_X + 8 + 2*CONTROL_X)) {
        attron(A_STANDOUT | A_UNDERLINE);
        mvwprintw(stdscr, (LINES)/2 - 8, (COLS - 30)/2, "Please make the window bigger!");
        attroff(A_STANDOUT | A_UNDERLINE);
        refresh();
        return;
    }

    /* Essential variables */
    bool status = FALSE;
    int returnval = 1;
    STATS score;
    score.points = 0;
    score.lines = 0;

    /* Randomizes the pseudorandom generating key using time */
    time_t t = time(NULL);
    struct tm today = *localtime(&t);
    srand(t * today.tm_hour * today.tm_sec); 

    /* Creates game structure */
    bool init = game_win_create();
    if (init == FALSE) return;
    Matrix game_matrix = game_playfield_matrix(PLAYFIELD_Y, PLAYFIELD_X);
    nodelay(win_game, TRUE);
    scrollok(win_game, FALSE);

    /* Select initial current piece */
    int current = rand() % 7, next;
    
    /* MAIN GAME LOOP */
    while(!status) {
        next = rand() % 7; // Gets the next piece randomly 
        game_update_next(next); // Updates the next window
        returnval = game_drop_piece(game_matrix, current); // Drops a piece
        if (returnval != -1) score.points += returnval; // Register score
        score = game_check_playfield(game_matrix, score); // Scans the visible area for completed lines and gets the score
        game_stats_update(score); // Updates the score
        status = game_check_gameover(game_matrix); // Check for gameover
        if (returnval == -1) status = TRUE; // If the user presses Q, the loop will be stopped
        current = next; // Switches the current piece to the next
    }

    /* If the Quit Button was not pressed and the game is over, show the gameover window */
    game_save_score(score);
    if (returnval != -1) {
        game_over_display();
        napms(1000 * 5);
    }
    game_win_delete(game_matrix);
    
}

/* Initialises color pairs that will be used to color the pieces*/
void init_colors() {

    start_color();

    if (can_change_color()) {
        init_color(21, 1000, 1000, 0); // Brigth Yellow
        init_color(51, 1000, 647, 0); // Orange

        init_pair(2, COLOR_BLACK, 21); // O
        init_pair(5, COLOR_BLACK, 51); // L
    } else {
        init_pair(2, COLOR_BLACK, COLOR_YELLOW); // O
        init_pair(5, COLOR_BLACK, COLOR_RED); // L
    }

    init_pair(0, COLOR_BLACK, COLOR_BLACK); // Black
    init_pair(1, COLOR_BLACK, COLOR_CYAN); // I
    init_pair(3, COLOR_BLACK, COLOR_MAGENTA); // T
    init_pair(4, COLOR_BLACK, COLOR_BLUE); // J
    init_pair(6, COLOR_BLACK, COLOR_GREEN); // S
    init_pair(7, COLOR_BLACK, COLOR_RED); // Z
    init_pair(8, COLOR_WHITE, COLOR_WHITE); // Border
    init_pair(9, COLOR_WHITE, COLOR_BLACK); // Pause
    init_pair(10, COLOR_RED, COLOR_BLACK); // Gameover

}

/* Creates the game window and initialises the subwindows */
bool game_win_create() {

    /* Create general game window of same size as the terminal */
    win_game = newwin(0,0,0,0);
    if (win_game == NULL) {
        waddstr(stdscr, "Error generating the win_game!");
        return FALSE;
    }

    /* Get size of the terminal, initialise colors and allow for keypad input */
    int win_gamex, win_gamey;
    getmaxyx(win_game, win_gamey, win_gamex);
    touchwin(win_game);
    init_colors();
    keypad(win_game, true);

    /* Create playfield border subwindow */
    win_border = subwin(win_game, BORDER_Y, BORDER_X, (win_gamey - BORDER_Y)/2, (win_gamex - BORDER_X)/2 - 4);
    if (win_border == NULL) {
        waddstr(stdscr, "Error generating the win_border!");
        return FALSE;
    }

    /* Draw Borders (Left, Right, Bottom) into win_border */
    wattron(win_border, COLOR_PAIR(8));
    for (int y = 4; y < BORDER_Y; y++)
        mvwaddstr(win_border, y, 0, SQUARE);

    for (int y = 4; y < BORDER_Y; y++)
        mvwaddstr(win_border, y, BORDER_X - 2, SQUARE);

    for (int x = 0; x < BORDER_X; x++)
        mvwaddstr(win_border, BORDER_Y - 1, x, SQUARE);

    wattroff(win_border, COLOR_PAIR(8));
    
    /* Initialise actual playfield subwindow */
    win_playfield = derwin(win_border, PLAYFIELD_Y, WIN_PLAYFIELD_X, 0, 2);
    if (win_playfield == NULL) {
        waddstr(stdscr, "Error generating the win_playfield!");
        return FALSE;
    }

    /* Initialise controls subwindow */
    win_controls = subwin(win_game, CONTROL_Y, CONTROL_X, (win_gamey - CONTROL_Y)/2 + 7, (win_gamex - BORDER_X)/2 - CONTROL_X - 6);
    if (win_controls == NULL) {
        waddstr(stdscr, "Error generating the win_border!");
        return FALSE;
    }
    box(win_controls, 0, 0);

    /* Print the controls into the window*/
    char * controls[] = {
        "<-  Move Left",
        "->  Move Right",
        "v   Move Down",
        "_   Hard Drop",
        "^   Rotate",
        "P   Pause ",
        "Q   Quit Game"
    };

    mvwaddstr(win_controls, 1, 5, "CONTROLS");
    for (int txt = 0; txt < 7; txt++)
        mvwaddstr(win_controls, 3 + txt, 2, controls[txt]);
    
    /* Initialise Next Block subwindow */
    win_next = subwin(win_game, 9, 18, (win_gamey - 9)/2 - 2, (win_gamex - BORDER_X)/2 + BORDER_X - 2);
    if (win_next == NULL) {
        waddstr(stdscr, "Error generating the win_next!");
        return FALSE;
    }
    box(win_next, 0, 0);
    mvwaddstr(win_next, 1, 4, "NEXT PIECE");

    /* Initialise the STATS subwindow */
    win_stats = subwin(win_game, 7, 18, (win_gamey - 7)/2 - 3, (win_gamex - BORDER_X)/2 - CONTROL_X - 6);
    if (win_stats == NULL) {
        waddstr(stdscr, "Error generating the win_next!");
        return FALSE;
    }
    box(win_stats, 0, 0);
    mvwaddstr(win_stats, 1, 4, "STATISTICS");
    game_stats_update((STATS){.lines = 0, .points = 0});

    wrefresh(win_game);
    return TRUE;

}

/* Allocate memory dynamically for the 2D Array which keeps track of the pieces */
Matrix game_playfield_matrix(int height, int width) {

    Matrix game_matrix = (int **)calloc(height, sizeof(int *));
    for (int i = 0; i < height; i++)
        game_matrix[i] = (int *)calloc(width, sizeof(int));

    return game_matrix;

}

/* Updates the playfield subwindow according to the current state of the matrix */
void game_playfield_update(Matrix game_matrix) {

    // Update from left to right, top to down
    for (int y = 4; y < PLAYFIELD_Y; y++) {
        for (int x = 0; x < PLAYFIELD_X; x++) {
            wattrset(win_playfield, COLOR_PAIR(game_matrix[y][x]));
            // wattrset(win_game, COLOR_PAIR(game_matrix[y][x]));
            mvwprintw(win_playfield, y, x*2, SQUARE);
            // mvwprintw(win_game, 6 + y, 1 + x*2, "%d ", game_matrix[y][x]);
        }
    }

    wrefresh(win_playfield);

}

/* Generates a piece into the playfield according to the position, type, orientation. Or erases the piece */
void game_draw_piece(int y, int x, int type, int orientation, int del) {

    POSITION pos;
    for (int i = 0; i < 4; i++) {
        pos = tetraminoes[type][orientation][i];
        if (del || y + pos.y < 4) {
            wattrset(win_playfield, COLOR_PAIR(0));
            mvwaddstr(win_playfield, y + pos.y, x*2 + pos.x*2, SQUARE);
        }
        else {
            wattrset(win_playfield, COLOR_PAIR(type + 1));
            mvwaddstr(win_playfield, y + pos.y, x*2 + pos.x*2, SQUARE);
        }
    }
}

/* Displays the next piece that will be placed into the playfield in the subwindow win_next */
void game_update_next(int type) {

    POSITION pos;
    int y = 3, x = 5;

    /* Clears the previous piece */
    wattrset(win_next, COLOR_PAIR(0));
    werase(win_next);
    box(win_next, 0, 0);
    mvwaddstr(win_next, 1, 4, "NEXT PIECE");

    /* Aligns the piece to the center if it is not O or I */
    if (!(type == 0 || type == 1)) {x++; y++;}

    /* Prints the piece into the next window*/
    wattrset(win_next, COLOR_PAIR(type + 1));
    for (int i = 0; i < 4; i++) {
        pos = tetraminoes[type][0][i];
        mvwaddstr(win_next, y + pos.y, x + pos.x*2, SQUARE);
    }
    wrefresh(win_next);

}

/* Returns FALSE if a piece collides with the left, right and bottom borders or if collides with a locked piece. Returns TRUE, which means that the position is valid */
bool game_check_position(Matrix game_matrix, int y, int x, int type, int orientation) {
    
    POSITION pos;
    for (int i = 0; i < 4; i++) {
        pos = tetraminoes[type][orientation][i];
        /* mvwprintw(win_game, 3 + i, 1, "pos[%d]: y = %02d & x = %02d", i, y + pos.y, x + pos.x); Uncomment for Debugging */
        /* Collision with: LEFT or RIGHT or DOWN or OTHER PIECE */
        if (x + pos.x < 0 || x + pos.x >= PLAYFIELD_X || y + pos.y > PLAYFIELD_Y - 1 || (game_matrix[y + pos.y][x + pos.x] > 0)) {
            /* mvwprintw(win_game, 8, 1, "Allowed: FALSE"); Uncomment for debugging */ 
            return FALSE; /* Not Allowed */
        }
    }
    /* mvwprintw(win_game, 8, 1, "Allowed: TRUE "); Uncomment for debugging */
    return TRUE;
}

/* Core function that is responsible for dropping the piece. Receives input for the pieces movement or for pausing and quitting the game. Returns a status or the points scored by executing a soft or hard drop */
int game_drop_piece(Matrix game_matrix, int type) {

    int y = 0; 
    int x = PLAYFIELD_X/2 - 2;
    int orientation = 0;
    int soft = 0, hard = 0;
    int pause = 1;

    fd_set t1, t2;
	struct timeval timeout;
	int sel_ret;

    timeout.tv_sec = 0;
	timeout.tv_usec = 300000;

    FD_ZERO(&t1);
	FD_SET(0, &t1);

    int ch = '\n';
    game_draw_piece(y, x, type, orientation, 0);

    while (1) {
        t2 = t1;
        sel_ret = select(FD_SETSIZE, &t2, (fd_set *) 0, (fd_set *) 0, &timeout);
        ch = wgetch(win_game);
        /* mvwprintw(win_game, 1, 1, "Y: %02d", y); Uncomment for Debugging */
        /* mvwprintw(win_game, 2, 1, "X: %02d", x); Uncomment for Debugging */

        switch(ch) {
            
            /* Rotate Piece */
            case KEY_UP:
                if (game_check_position(game_matrix, y, x, type, orientation + 1 == 4 ? 0 : orientation + 1)) {
                    game_draw_piece(y, x, type, orientation, 1);
					if(++orientation == 4) orientation = 0;
					game_draw_piece(y, x, type, orientation, 0);
                } else {
                    // Wall Kick - Try to shift to the right
                    for (int try = 1; try <= 2; try++) {
                        if (game_check_position(game_matrix, y, x + try, type, orientation + 1 == 4 ? 0 : orientation + 1)) {
                            game_draw_piece(y, x, type, orientation, 1);
					        if(++orientation == 4) orientation = 0;
                            x += try;
					        game_draw_piece(y, x, type, orientation, 0);
                            break;
                        }    
                    }

                    // Wall Kick - Try to shift to the left
                    for (int try = 1; try <= 2; try++) {
                        if (game_check_position(game_matrix, y, x - try, type, orientation + 1 == 4 ? 0 : orientation + 1)) {
                            game_draw_piece(y, x, type, orientation, 1);
					        if(++orientation == 4) orientation = 0;
                            x -= try;
					        game_draw_piece(y, x, type, orientation, 0);
                            break;
                        }    
                    }
                } break; 

            /* Shift to the Left */
            case KEY_LEFT:
                if (game_check_position(game_matrix, y , x - 1, type, orientation)) {
                    game_draw_piece(y, x, type, orientation, 1);
                    game_draw_piece(y, --x, type, orientation, 0);
                } break;

            /* Shift to the Right */
            case KEY_RIGHT:
                if (game_check_position(game_matrix, y , x + 1, type, orientation)) {
                    game_draw_piece(y, x, type, orientation, 1);
                    game_draw_piece(y, ++x, type, orientation, 0);
                } break;

            /* Shift Down by one row */
            case KEY_DOWN:
                if (game_check_position(game_matrix, y + 1, x, type, orientation)) {
                    game_draw_piece(y, x, type, orientation, 1);
                    game_draw_piece(++y, x, type, orientation, 0);
                } soft++; break;
            
            /* Drop Piece until touches one piece or the bottom */
            case ' ':
                game_draw_piece(y, x, type, orientation, 1);
                while (game_check_position(game_matrix, y + 1, x, type, orientation) == 1) {
                    y++;
                    hard++;
                }
                game_draw_piece(y, x, type, orientation, 0);
                game_lock_piece(game_matrix, y, x, type, orientation);
                wrefresh(win_playfield);
                return 2*hard; // Hard Drop

            /* Pause the Game */
            case 'p':
                pause = 0;
                pause = game_pause();
                if (pause == -1) return -1;
                game_playfield_update(game_matrix);
                break;

            /* Quit Game */
            case 'q':
                return -1;
                break;
        }

        wrefresh(win_playfield);
        if (sel_ret == 0) {
            if (game_check_position(game_matrix, y + 1, x, type, orientation) && pause) {
                game_draw_piece(y, x, type, orientation, 1);
                game_draw_piece(++y, x, type, orientation, 0);
            } else {
                napms(300);
                game_lock_piece(game_matrix, y, x, type, orientation);
                return soft; // Soft Drop
            }
            timeout.tv_sec = 0;
		    timeout.tv_usec = 300000;
        }
        
        wrefresh(win_playfield);
    }

}

/* Writes the position of the piece into the matrix */
void game_lock_piece(Matrix game_matrix, int y, int x, int type, int orientation) {
    POSITION pos;
    
    for (int i = 0; i < 4; i++) {
        pos = tetraminoes[type][orientation][i];
        game_matrix[y + pos.y][x + pos.x] = type + 1;
    }
}

/* Receives the Game Matrix and a row number. Returns TRUE if the row is full, FALSE otherwise*/
bool game_check_row(Matrix game_matrix, int row) {
    
    /* Buffer rows can't have pieces */
    if (row > PLAYFIELD_Y) return FALSE;

    /* Returns FALSE if there is one empty space, TRUE if the entire row is different than 0*/
    for (int x = 0; x < PLAYFIELD_X; x++) {
        if (game_matrix[row][x] == 0)
            return FALSE;
    }

    return TRUE;
}

/* Takes the Game Matrix and a row number. Erases the row and shifts the pieces down */
void game_remove_row(Matrix game_matrix, int row) {

    /* Set row to 0 */
    for (int x = 0; x < PLAYFIELD_X; x++)
        game_matrix[row][x] = 0;

    /* Shifts the above rows */
    for (int y = row - 1; y > 0; y--) {
        for (int x = 0; x < PLAYFIELD_X; x++)
            game_matrix[y + 1][x] = game_matrix[y][x];
    }

    /* Update the field and the subwindow */
    game_playfield_update(game_matrix);
    wrefresh(win_playfield);
}

/* Scans the visible playfield area for completed lines. If a line is completed, it calls the functions to erase it and shift the pieces, and returns the number of lines erased and the number of points awarded according to the number of erased lines (maximum 4) */
STATS game_check_playfield(Matrix game_matrix, STATS score) {

    int row_count = 0;
    for (int i = 4; i < PLAYFIELD_Y; i++) {
        if (game_check_row(game_matrix, i)) {
            game_remove_row(game_matrix, i);
            row_count++;
        }    
    }

    /* Award points to the player according to the number of cleared rows */
    switch (row_count) {
        case 0: return score;
        case 1: 
            score.lines += 1;
            score.points += 100;
            break;
        case 2:
            score.lines += 2;
            score.points += 300;
            break;
        case 3:
            score.lines += 3;
            score.points += 500;
            break;
        case 4:
            score.lines += 4;
            score.points += 800;
            break;
    }
    return score;
}

/* Updates the subwindow stats to display the points and the number of completed liens */
void game_stats_update(STATS score) {
    
    mvwprintw(win_stats, 3, 3, "Points %05d", score.points);
    mvwprintw(win_stats, 4, 3, "Lines  %05d", score.lines);
    wrefresh(win_stats);

}

/* Creates win_pause and displays option if the game is paused */
int game_pause() {

    /* Variables and Sizes */
    int width = 12;
    int heigth = 6;
    int win_gamey, win_gamex;
    getmaxyx(win_border, win_gamey, win_gamex);

    int win_y = (win_gamey - heigth)/2;
    int win_x = (win_gamex - width)/2;
    
    /* Clear playfield animation */
    for (int y = 0; y < heigth; y++) {
        for (int x = 0; x < width/2; x++) {
            wattrset(win_playfield, COLOR_PAIR(0));
            mvwaddstr(win_playfield, win_y + y, win_x + x, SQUARE);
            napms(50);
            wrefresh(win_playfield);
        }
    }

    /* Display options */
    win_pause = derwin(win_border, heigth, width, win_y, win_x);
    wattron(win_pause, COLOR_PAIR(9));
    box(win_pause, 0, 0);
    mvwaddstr(win_pause, 0, 3, "PAUSED");
    mvwaddstr(win_pause, 2, 2, "P Resume");
    mvwaddstr(win_pause, 3, 3, "Q Quit");
    wattroff(win_pause, COLOR_PAIR(10));
    wrefresh(win_pause);

    /* Get input */
    nodelay(win_game, FALSE);
    char opt = ' ';
    while (1) {
        opt = wgetch(win_game);
        switch (opt) {
        case 'p':
            werase(win_pause);
            nodelay(win_game, TRUE);
            delwin(win_pause);
            return 1;
        case 'q':
            return -1;
        }
    }
    
    return 1;
}

/* Check the rows for any piece above the visible area in the game matrix, returns TRUE if there is a piece, which means that the game is over or FALSE if the game is still not over */
bool game_check_gameover(Matrix game_matrix) {
   
    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < PLAYFIELD_X; x++) {
            if (game_matrix[y][x] > 0)
                return TRUE;
        }
    }
    return FALSE;
}

/* Displays the GAME OVER! message to the user */
void game_over_display() {
    
    int width = 14;
    int heigth = 5;
    int win_gamey, win_gamex;
    getmaxyx(win_border, win_gamey, win_gamex);

    int win_y = (win_gamey - heigth)/2;
    int win_x = (win_gamex - width)/2;
    
    // Clear playfield animation
    for (int y = 0; y < heigth; y++) {
        for (int x = 0; x < width/2; x++) {
            wattrset(win_playfield, COLOR_PAIR(0));
            mvwaddstr(win_playfield, win_y + y, win_x + x, SQUARE);
            napms(50);
            wrefresh(win_playfield);
        }
    }

    win_gameover = derwin(win_border, heigth, width, win_y, win_x);
    werase(win_gameover);
    wattron(win_gameover, COLOR_PAIR(10));
    box(win_gameover, 0, 0);
    char text[] = "GAME OVER!";
    for (int x = 0; x < 10; x++) {
        mvwprintw(win_gameover, 2, 2 + x, "%c", text[x]);
        wrefresh(win_gameover);
        napms(100);
    }
    
}

/* Saves the score/statistics of the game and the date into a file */
void game_save_score(STATS score) {
    
    /* Get current time*/
    time_t current = time(NULL);
    struct tm t = *localtime(&current);

    /* Open File*/
    FILE * input = fopen(SAVE_SCORES, "a");
    if (input == NULL) {
        fclose(input);
        return;
    }

    /* Avoid registering 0 points and cap the score to 999999 to avoid file handling problems */
    if (score.points > 0) {
        if (score.points >= 1000000) score.points = 999999;
        if (score.lines >= 100000) score.lines = 99999;
        fprintf(input, "%06d %05d %04d %02d %02d %02d %02d %02d\n", score.points, score.lines, t.tm_year + 1900, t.tm_mon + 1, t.tm_mday, t.tm_hour, t.tm_min, t.tm_sec);
    }

    fclose(input);
}

/* Frees the dynamic allocated matrix and deletes the windows and subwindows */
void game_win_delete(Matrix game_matrix) {
    
    for (int i = 0; i < PLAYFIELD_Y; i++)
        free(game_matrix[i]);
    free(game_matrix);
    werase(win_game);
    wrefresh(win_game);
    delwin(win_playfield);
    delwin(win_gameover);
    delwin(win_pause);
    delwin(win_border);
    delwin(win_stats);
    delwin(win_next);
    delwin(win_controls);
    delwin(win_game);
    refresh();
}