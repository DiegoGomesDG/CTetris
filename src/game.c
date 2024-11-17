#include "game.h"

// TETRAMINOES [NUMBER][ROTATION][POSITION]
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

void game_play() {

    bool status = FALSE;
    STATS score;
    score.points = 0, score.lines = 0;

    time_t t = time(NULL);
    struct tm today = *localtime(&t);
    srand(t * today.tm_hour * today.tm_sec);

    game_win_create();
    Matrix game_matrix = game_playfield_matrix(PLAYFIELD_Y, PLAYFIELD_X);
    nodelay(win_game, TRUE);

    int current = rand() % 7, next;
    
    while(!status) {

        next = rand() % 7;
        game_update_next(next);
        score.points += game_drop_piece(game_matrix, current);
        score = game_check_playfield(game_matrix, score);
        game_stats_update(score);
        status = game_check_gameover(game_matrix);// check game over
        current = next;
    }

    gameover_win();
    napms(1000 * 10);
    game_win_delete(game_matrix);
    
    
}

void game_win_create() {

    // Create general window of same size as the terminal
    win_game = newwin(0,0,0,0);
    if (win_game == NULL) {
        waddstr(stdscr, "Error!");
        exit(1);
    }

    int win_gamex, win_gamey;
    getmaxyx(win_game, win_gamey, win_gamex);
    touchwin(win_game);
    init_colors();
    keypad(win_game, true);

    // Create playfield border window
    win_border = subwin(win_game, BORDER_Y, BORDER_X, (win_gamey - BORDER_Y)/2, (win_gamex - BORDER_X)/2);

    if (win_border == NULL) {
        waddstr(stdscr, "Error!");
        endwin();
        exit(1);
    }

    // Draw Borders (Left, Right, Bottom)
    wattron(win_border, COLOR_PAIR(8));
    for (int y = 4; y < BORDER_Y; y++)
        mvwaddstr(win_border, y, 0, SQUARE);

    for (int y = 4; y < BORDER_Y; y++)
        mvwaddstr(win_border, y, BORDER_X - 2, SQUARE);

    for (int x = 0; x < BORDER_X; x++)
        mvwaddstr(win_border, BORDER_Y - 1, x, SQUARE);

    wattroff(win_border, COLOR_PAIR(8));
    
    // Create actual playfield window
    win_playfield = derwin(win_border, PLAYFIELD_Y, WIN_PLAYFIELD_X, 0, 2);
    

    // Create controls info window
    win_controls = subwin(win_game, CONTROL_Y, CONTROL_X, (win_gamey - CONTROL_Y)/2 + 7, (win_gamex - BORDER_Y)/2 - CONTROL_X + 1);
    box(win_controls, 0, 0);

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
    
    // Create Next Block Window
    win_next = subwin(win_game, 9, 18, (win_gamey - 9)/2 - 2, (win_gamex - BORDER_Y)/2 + BORDER_X + 5);
    box(win_next, 0, 0);
    mvwaddstr(win_next, 1, 4, "NEXT PIECE");

    // Initialise the STATS window
    win_stats = subwin(win_game, 7, 18, (win_gamey - 7)/2 - 3, (win_gamex - BORDER_Y)/2 - CONTROL_X + 1);
    if (win_stats == NULL) exit(2);
    box(win_stats, 0, 0);
    mvwaddstr(win_stats, 1, 4, "STATISTICS");
    game_stats_update((STATS){.lines = 0, .points = 0});

    wrefresh(win_game);

}

Matrix game_playfield_matrix(int height, int width) {

    Matrix game_matrix = (int **)calloc(height, sizeof(int *));
    for (int i = 0; i < height; i++)
        game_matrix[i] = (int *)calloc(width, sizeof(int));

    return game_matrix;

}

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

void game_update_next(int type) {

    POSITION pos;
    int y = 3, x = 5;

    // Clear the previous piece 
    wattrset(win_next, COLOR_PAIR(0));
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            mvwaddstr(win_next, y + i, x + j*2, SQUARE);

    if (!(type == 0 || type == 1)) {x++; y++;}

    for (int i = 0; i < 4; i++) {
        pos = tetraminoes[type][0][i];
        wattrset(win_next, COLOR_PAIR(type + 1));
        mvwaddstr(win_next, y + pos.y, x + pos.x*2, SQUARE);
    }
    wrefresh(win_next);

}

bool game_check_position(Matrix game_matrix, int y, int x, int type, int orientation) {
    
    POSITION pos;
    for (int i = 0; i < 4; i++) {
        pos = tetraminoes[type][orientation][i];
        mvwprintw(win_game, 3 + i, 1, "pos[%d]: y = %02d & x = %02d", i, y + pos.y, x + pos.x);
        // LEFT || RIGHT || UP || OTHER
        if (x + pos.x < 0 || x + pos.x >= PLAYFIELD_X || y + pos.y > PLAYFIELD_Y - 1 || (game_matrix[y + pos.y][x + pos.x] > 0)) {
            mvwprintw(win_game, 8, 1, "Allowed: FALSE");
            return FALSE;
        }
    }
    mvwprintw(win_game, 8, 1, "Allowed: TRUE ");
    return TRUE;
}

void game_lock_piece(Matrix game_matrix, int y, int x, int type, int orientation) {
    POSITION pos;
    
    for (int i = 0; i < 4; i++) {
        pos = tetraminoes[type][orientation][i];
        game_matrix[y + pos.y][x + pos.x] = type + 1;
    }
}

int game_drop_piece(Matrix game_matrix, int type) {

    int y = 0; 
    int x = PLAYFIELD_X/2 - 2;
    int orientation = 0;
    int soft = 0, hard = 0;

    fd_set t1, t2;
	struct timeval timeout;
	int sel_ret;

    timeout.tv_sec = 0;
	timeout.tv_usec = 500000;

    FD_ZERO(&t1);			//initialise
	FD_SET(0, &t1);

    int ch = ' ';
    int delay = 0;
    game_draw_piece(y, x, type, orientation, 0);

    while (1) {
        t2 = t1;
        sel_ret = select(FD_SETSIZE, &t2, (fd_set *) 0, (fd_set *) 0, &timeout);
        ch = wgetch(win_game);
        mvwprintw(win_game, 1, 1, "Y: %02d", y);
        mvwprintw(win_game, 2, 1, "X: %02d", x);

        switch(ch) {
            
            case KEY_UP:
                if (game_check_position(game_matrix, y, x, type, orientation + 1 == 4 ? 0 : orientation + 1)) {
                    game_draw_piece(y, x, type, orientation, 1);
					++orientation == 4 ? orientation = 0 : 0;
					game_draw_piece(y, x, type, orientation, 0);
                } /* else {
                    game_draw_piece(y, x, type, orientation, 1);
                    while(game_check_position(game_matrix, y, x, type, orientation + 1 == 4 ? 0 : orientation + 1 ) == 0) {
                        if (x < PLAYFIELD_X/2) x++;
                        if (x > 0) x--;
                        if (y > 20) y--;
                    }
					++orientation == 4 ? orientation = 0 : 0;
					game_draw_piece(y, x, type, orientation, 0);
                } */
                soft++; break; 

            case KEY_LEFT:
                if (game_check_position(game_matrix, y , x - 1, type, orientation)) {
                    game_draw_piece(y, x, type, orientation, 1);
                    game_draw_piece(y, --x, type, orientation, 0);
                } break;

            case KEY_RIGHT:
                if (game_check_position(game_matrix, y , x + 1, type, orientation)) {
                    game_draw_piece(y, x, type, orientation, 1);
                    game_draw_piece(y, ++x, type, orientation, 0);
                } break;

            case KEY_DOWN:
                if (game_check_position(game_matrix, y + 1, x, type, orientation)) {
                    game_draw_piece(y, x, type, orientation, 1);
                    game_draw_piece(++y, x, type, orientation, 0);
                } soft++; break;
            
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

        }

        if (sel_ret == 0) {
            if (game_check_position(game_matrix, y + 1, x, type, orientation)) {

                game_draw_piece(y, x, type, orientation, 1);
                game_draw_piece(++y, x, type, orientation, 0);
            } else {
                napms(300);
                game_lock_piece(game_matrix, y, x, type, orientation);
                return soft; // Soft Drop
            }
            timeout.tv_sec = 0;
		    timeout.tv_usec = 500000;
        }
        
        wrefresh(win_playfield);
    }

}

/* Takes the Game Matrix (2D Array) and a row number, returns TRUE if it is full, FALSE otherwise*/
bool game_check_row(Matrix game_matrix, int row) {
    
    if (row > PLAYFIELD_Y) return FALSE;

    for (int x = 0; x < PLAYFIELD_X; x++) {
        if (game_matrix[row][x] == 0)
            return FALSE;
    }

    return TRUE;
}

void game_remove_row(Matrix game_matrix, int row) {

    // Set row to 0
    for (int x = 0; x < PLAYFIELD_X; x++)
        game_matrix[row][x] = 0;

    // Shift all the above
    for (int y = row - 1; y > 0; y--) {
        for (int x = 0; x < PLAYFIELD_X; x++)
            game_matrix[y + 1][x] = game_matrix[y][x];
    }

    game_playfield_update(game_matrix);
    wrefresh(win_playfield);
}

STATS game_check_playfield(Matrix game_matrix, STATS score) {

    int row_count = 0;
    for (int i = 4; i < PLAYFIELD_Y; i++) {
        if (game_check_row(game_matrix, i)) {
            game_remove_row(game_matrix, i);
            row_count++;
        }    
    }

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

bool game_check_gameover(Matrix game_matrix) {
    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < PLAYFIELD_X; x++) {
            if (game_matrix[y][x] > 0)
                return TRUE;
        }

    }
    return FALSE;
}

void game_stats_update(STATS score) {
    
    mvwprintw(win_stats, 3, 3, "Points %05d", score.points);
    mvwprintw(win_stats, 4, 3, "Lines  %05d", score.lines);
    wrefresh(win_stats);

}

void gameover_win() {
    
    int width = 14;
    int heigth = 5;
    int win_gamey, win_gamex;
    getmaxyx(win_border, win_gamey, win_gamex);

    int win_y = (win_gamey - heigth)/2;
    int win_x = (win_gamex - width)/2;
    
    // Clear playfield
    for (int y = 0; y < heigth; y++) {
        for (int x = 0; x < width/2; x++) {
            wattrset(win_playfield, COLOR_PAIR(0));
            mvwaddstr(win_playfield, win_y + y, win_x + x, SQUARE);
            napms(50);
            wrefresh(win_playfield);
        }
    }

    win_gameover = derwin(win_border, heigth, width, win_y, (win_gamex - width)/2);
    wattron(win_gameover, COLOR_PAIR(9));
    box(win_gameover, 0, 0);
    char text[] = "GAME OVER!";
    for (int x = 0; x < 10; x++) {
        mvwprintw(win_gameover, 2, 2 + x, "%c", text[x]);
        wrefresh(win_gameover);
        napms(100);
    }
    
}

void game_win_delete(Matrix game_matrix) {
    
    for (int i = 0; i < PLAYFIELD_Y; i++)
        free(game_matrix[i]);
    free(game_matrix);
    
    delwin(win_playfield);
    delwin(win_gameover);
    delwin(win_border);
    delwin(win_stats);
    delwin(win_next);
    delwin(win_controls);
    delwin(win_pause);
    delwin(win_game);
    refresh();
}

void init_colors() {

    start_color();

    init_color(21, 1000, 1000, 0); // Brigth Yellow
    init_color(51, 1000, 647, 0); // Orange

    init_pair(0, COLOR_BLACK, COLOR_BLACK); // Black
    init_pair(1, COLOR_BLACK, COLOR_CYAN); // I
    init_pair(2, COLOR_BLACK, 21); // O
    init_pair(3, COLOR_BLACK, COLOR_MAGENTA); // T
    init_pair(4, COLOR_BLACK, COLOR_BLUE); // J
    init_pair(5, COLOR_BLACK, 51); // L
    init_pair(6, COLOR_BLACK, COLOR_GREEN); // S
    init_pair(7, COLOR_BLACK, COLOR_RED); // Z
    init_pair(8, COLOR_WHITE, COLOR_WHITE); // Border
    init_pair(9, COLOR_RED, COLOR_BLACK); // Gameover

}

void debug_colors() {
    for (int i = 1; i < 8; i++) {
        wattron(win_game, COLOR_PAIR(i));
        mvwaddstr(win_game, i, 1, SQUARE);
        wattroff(win_game, COLOR_PAIR(i));
    }
}