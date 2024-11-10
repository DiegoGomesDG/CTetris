#ifndef GAME_H
#define GAME_H

#include <ncurses.h>
#include <stdlib.h>

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
WINDOW * win_pause;

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
Matrix game_playfield_array(int height, int width);
void game_stats_update(STATISTICS score);
void game_playfield_update(Matrix game_array);
void game_draw_piece(Matrix game_array, int y, int x, int type, int orientation, int del);
// Lock Piece
bool game_check_row(int row);
// Delete Line
// Clear Lines
// Check Gameover
// Save points
void init_colors();
void debug_colors();
void game_win_delete();

#endif