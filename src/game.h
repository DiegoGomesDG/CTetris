#ifndef GAME_H
#define GAME_H

#include <ncurses.h>
#include <stdlib.h>

#define SQUARE "  "
#define PLAYFIELD_Y 24
#define PLAYFIELD_X 20
#define BORDER_Y PLAYFIELD_Y + 1
#define BORDER_X PLAYFIELD_X + 4
#define CONTROL_Y 11
#define CONTROL_X 18

WINDOW * win_game;
WINDOW * win_border;
WINDOW * win_playfield;
WINDOW * win_next;
WINDOW * win_controls;
WINDOW * win_stats;
WINDOW * win_pause;

typedef struct points {
    unsigned int stats;
    unsigned int lines;
} POINTS;

void game_play();
void game_win_create();
// Delete Line
// Spawn Piece
// Lock Piece
// Clear Lines
// Check Gameover
// Save points
void init_colors();
void debug_colors();
void game_win_delete();

#endif