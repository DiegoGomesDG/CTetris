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

WINDOW * win_game; // Main Game Window
WINDOW * win_border; // Defines the border of the playfield
WINDOW * win_playfield; // Actual playfield window
WINDOW * win_next; // Window that shows the next block
WINDOW * win_controls; // Window that display the commands
WINDOW * win_stats; // Window that display the statistics
WINDOW * win_pause;

typedef struct points {
    unsigned int stats;
    unsigned int lines;
} POINTS;

typedef struct position {
    unsigned int y;
    unsigned int x;

} POSITION;

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
        {{0,1},{1,0},{1,1},{2,1}}, /* .#.. */
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