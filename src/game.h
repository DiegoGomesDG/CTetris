#ifndef GAME_H
#define GAME_H

#include <ncurses.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>

#define SQUARE "  " // One space creates a rectangle, two a square
#define PLAYFIELD_Y 24 // Actual heigth
#define PLAYFIELD_X 10 // Actual width
#define WIN_PLAYFIELD_X 2*PLAYFIELD_X // Visual width 
#define BORDER_Y PLAYFIELD_Y + 1 // Heigth of the border
#define BORDER_X WIN_PLAYFIELD_X + 4 // Width of the border
#define CONTROL_Y 12 // Heigth of the controls window
#define CONTROL_X 18 // Width of the controls

WINDOW * win_game; // Main Game Window
WINDOW * win_border; // Defines the border of the playfield
WINDOW * win_playfield; // Actual playfield window
WINDOW * win_next; // Window that shows the next block
WINDOW * win_controls; // Window that display the commands
WINDOW * win_stats; // Window that display the STATS
WINDOW * win_pause; // TO DO: When 
WINDOW * win_gameover; // Shows gameover

typedef struct STATS {
    unsigned int points;
    unsigned int lines;
} STATS;

typedef struct position {
    unsigned int y;
    unsigned int x;
} POSITION;

typedef int** Matrix;

extern POSITION tetraminoes[7][4][4];

void game_play();
void game_win_create();
Matrix game_playfield_matrix(int height, int width);
void game_playfield_update(Matrix game_matrix);
void game_draw_piece(int y, int x, int type, int orientation, int del);
void game_stats_update(STATS score);
bool game_check_position(Matrix game_matrix, int y, int x, int type, int orientation);
void game_update_next(int type);
int game_drop_piece (Matrix game_matrix, int type);
void game_lock_piece(Matrix game_matrix, int y, int x, int type, int orientation);
bool game_check_row(Matrix game_matrix, int row);
void game_remove_row(Matrix game_matrix, int row);
STATS game_check_playfield(Matrix game_matrix, STATS score);
bool game_check_gameover(Matrix game_matrix);
void gameover_win();
// Save points
// Pause Game
// Save points
void init_colors();
void debug_colors();
void game_win_delete(Matrix game_array);

#endif