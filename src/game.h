#ifndef GAME_H
#define GAME_H

#include <ncurses.h>
#include <stdlib.h>
#include <time.h>

#define SQUARE "  "
#define PLAYFIELD_Y 24
#define PLAYFIELD_X 10
#define WIN_PLAYFIELD_X 2*PLAYFIELD_X
#define BORDER_Y PLAYFIELD_Y + 1
#define BORDER_X WIN_PLAYFIELD_X + 4
#define CONTROL_Y 11
#define CONTROL_X 18

WINDOW * win_game; // Main Game Window
WINDOW * win_border; // Defines the border of the playfield
WINDOW * win_playfield; // Actual playfield window
WINDOW * win_next; // Window that shows the next block
WINDOW * win_controls; // Window that display the commands
WINDOW * win_stats; // Window that display the statistics
WINDOW * win_pause; // TO DO: When 

typedef struct statistics {
    unsigned int points;
    unsigned int lines;
} STATISTICS;

typedef struct position {
    unsigned int y;
    unsigned int x;
} POSITION;

typedef int** Matrix;

extern POSITION tetraminoes[7][4][4];

void game_play();
void game_win_create();
Matrix game_playfield_matrix(int height, int width);
void game_draw_piece(int y, int x, int type, int orientation, int del);
void game_stats_update(STATISTICS score);
bool game_check_position(Matrix game_matrix, int y, int x, int type, int orientation);
void game_update_next(int type);
int game_drop_piece (Matrix game_matrix, int type);
void game_lock_piece(Matrix game_matrix, int y, int x, int type, int orientation);
// Lock Piece
bool game_check_row(int row);
// Delete Line
// Clear Lines
// Check Gameover
// Save points
// Pause Game
// Save points
void init_colors();
void debug_colors();
void game_win_delete(Matrix game_array);

#endif