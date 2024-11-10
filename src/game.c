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

    STATISTICS score;
    score.points = 0, score.lines = 0;
    int current = 0, next = 1;

    game_win_create();
    Matrix game_matrix = game_playfield_matrix(PLAYFIELD_Y, PLAYFIELD_X);
    
    int ch;
    int rotation = 0, prev_rot;
    int y = 4, x = 3;
    nodelay(win_game, TRUE);

    game_draw_piece(game_matrix, y, x, current, rotation, 0);
    game_update_next(next, 0);
    while (ch != 'q') {
        ch = wgetch(win_game);
        // mvwprintw(win_game, 1, 1, "Y: %02d", y);
        // mvwprintw(win_game, 2, 1, "X: %02d", x);
        switch(ch) {

            case (' '):
                game_draw_piece(game_matrix, y, x, current, rotation, 1);
                current++;
                current = current % 7;
                next = (next + 1) % 7;
                game_draw_piece(game_matrix, y, x, current, rotation, 0);
                game_update_next(next, rotation);
                break;

            case KEY_UP:
                if (game_check_position(y, x, current, rotation + 1 == 4 ? 0 : rotation + 1)) {
                    game_draw_piece(game_matrix, y, x, current, rotation, 1);
					++rotation == 4 ? rotation = 0 : 0;
					game_draw_piece(game_matrix, y, x, current, rotation, 0);
                } else {
                    game_draw_piece(game_matrix, y, x, current, rotation, 1);
                    while(game_check_position(y, x, current, rotation + 1 == 4 ? 0 : rotation + 1) == 0) {
                        if (x < PLAYFIELD_X/2) x++;
                        if (x > 0) x--;
                        if (y > 20) y--;
                    }
					++rotation == 4 ? rotation = 0 : 0;
					game_draw_piece(game_matrix, y, x, current, rotation, 0);
                }
                break;

            case KEY_LEFT:
                if (game_check_position(y , x - 1, current, rotation)) {
                    game_draw_piece(game_matrix, y, x, current, rotation, 1);
                    game_draw_piece(game_matrix, y, --x, current, rotation, 0);
                }
                break;

            case KEY_RIGHT:
                if (game_check_position(y , x + 1, current, rotation)){
                    game_draw_piece(game_matrix, y, x, current, rotation, 1);
                    game_draw_piece(game_matrix, y, ++x, current, rotation, 0);
                }
                break;

            case KEY_DOWN:
                if (game_check_position(y + 1, x, current, rotation)){
                    game_draw_piece(game_matrix, y, x, current, rotation, 1);
                    game_draw_piece(game_matrix, ++y, x, current, rotation, 0);
                }
                break;
        default: break;
        }
        game_playfield_update(game_matrix);
    }

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
    win_controls = subwin(win_game, CONTROL_Y, CONTROL_X, (win_gamey - CONTROL_Y)/2 + 6, (win_gamex - BORDER_Y)/2 - CONTROL_X + 1);
    box(win_controls, 0, 0);

    char * controls[] = {
        "<-  Move Left",
        "->  Move Right",
        "v   Move Down",
        "^   Rotate",
        "_   Change ",
        "Q   Quit Game"
    };

    mvwaddstr(win_controls, 1, 5, "CONTROLS");
    for (int txt = 0; txt < 6; txt++)
        mvwaddstr(win_controls, 3 + txt, 2, controls[txt]);
    
    // Create Next Block Window
    win_next = subwin(win_game, 9, 18, (win_gamey - 9)/2 - 2, (win_gamex - BORDER_Y)/2 + BORDER_X + 5);
    box(win_next, 0, 0);
    mvwaddstr(win_next, 1, 4, "NEXT PIECE");

    // Initialise the statistics window
    win_stats = subwin(win_game, 7, 18, (win_gamey - 7)/2 - 3, (win_gamex - BORDER_Y)/2 - CONTROL_X + 1);
    if (win_stats == NULL) exit(2);
    box(win_stats, 0, 0);
    mvwaddstr(win_stats, 1, 4, "STATISTICS");
    game_stats_update((STATISTICS){.lines = 0, .points = 0});

    wrefresh(win_game);

}

Matrix game_playfield_matrix(int height, int width) {

    Matrix game_matrix = (int **)calloc(height, sizeof(int *));
    for (int i = 0; i < height; i++)
        game_matrix[i] = (int *)calloc(width, sizeof(int));

    return game_matrix;

}

void game_draw_piece(Matrix game_matrix, int y, int x, int type, int orientation, int del) {

    POSITION pos;

    for (int i = 0; i < 4; i++) {
        pos = tetraminoes[type][orientation][i];
        if (del) game_matrix[y + pos.y][x + pos.x] = 0;
        else game_matrix[y + pos.y][x + pos.x] = type + 1;
    }

}

void game_update_next(int type, int orientation) {

    POSITION pos;
    int y = 3, x = 5;

    // Reset 
    wattrset(win_next, COLOR_PAIR(0));
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            mvwaddstr(win_next, y + i, x + j*2, SQUARE);

    wrefresh(win_next);
    for (int i = 0; i < 4; i++) {
        pos = tetraminoes[type][orientation][i];
        wattrset(win_next, COLOR_PAIR(type + 1));
        mvwaddstr(win_next, y + pos.y, x + pos.x*2, SQUARE);
    }
    wrefresh(win_next);

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

int game_check_position(int y, int x, int type, int orientation) {
    
    POSITION pos;
    for (int i = 0; i < 4; i++) {
        pos = tetraminoes[type][orientation][i];
        // mvwprintw(win_game, 3 + i, 1, "pos[%d]: y = %02d & x = %02d", i, y + pos.y, x + pos.x);
        // LEFT || RIGHT || UP
        if (x + pos.x < 0 || x + pos.x >= PLAYFIELD_X || y + pos.y > PLAYFIELD_Y - 1) {
            // mvwprintw(win_game, 8, 1, "Allowed: FALSE");
            return 0;
        }
    }
    // mvwprintw(win_game, 8, 1, "Allowed: TRUE ");
    return 1;
}

bool game_check_row(int row) {
    return TRUE;
}

void game_stats_update(STATISTICS score) {
    
    mvwprintw(win_stats, 3, 3, "Points %05d", score.points);
    mvwprintw(win_stats, 4, 3, "Lines  %05d", score.lines);
    wrefresh(win_stats);

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

}

void game_win_delete(Matrix game_matrix) {
    free(game_matrix);
    delwin(win_playfield);
    delwin(win_border);
    delwin(win_stats);
    delwin(win_next);
    delwin(win_controls);
    delwin(win_pause);
    delwin(win_game);
};

void debug_colors() {
    for (int i = 1; i < 8; i++) {
        wattron(win_game, COLOR_PAIR(i));
        mvwaddstr(win_game, i, 1, SQUARE);
        wattroff(win_game, COLOR_PAIR(i));
    }
}