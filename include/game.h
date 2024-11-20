#ifndef GAME_H
#define GAME_H

/* ----- Libraries ----- */
#include <ncurses.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>

/* ----- Definition of Constants ----- */
#define SQUARE "  " // One space creates a rectangle, two a square
#define PLAYFIELD_Y 24 // Actual heigth
#define PLAYFIELD_X 10 // Actual width
#define WIN_PLAYFIELD_X 2*PLAYFIELD_X // Visual width (due to the two space characters)
#define BORDER_Y PLAYFIELD_Y + 1 // Heigth of the border
#define BORDER_X WIN_PLAYFIELD_X + 4 // Width of the border
#define CONTROL_Y 12 // Heigth of the controls window
#define CONTROL_X 18 // Width of the controls
#define SAVE_SCORES "./files/games.txt" // Path for Saving Scores

/* ----- Windows Global Variables ----- */
WINDOW * win_game; // Main Game Window
WINDOW * win_border; // Subwindow that defines the border of the playfield
WINDOW * win_playfield; // Actual playfield subwindow
WINDOW * win_next; // Subwindow which shows the next piece
WINDOW * win_controls; // Subwindow for displaying the commands
WINDOW * win_stats; // Subwindow that display the score/statistics
WINDOW * win_pause; // Subwindow which display options when the user pauses the game
WINDOW * win_gameover; // Subwindow that shows that the game is over

/* ----- Structures and Datatypes ----- */
/* Structure that represents the score/statistics of the game */
typedef struct STATS {
    unsigned int points;
    unsigned int lines;
} STATS;

/* Structure which defines the position in the window 
It is used primarly by the definition of the tetraminoes pieces */
typedef struct position {
    unsigned int y;
    unsigned int x;
} POSITION;

/* Renaming a 2D Integer Array to Matrix */
typedef int** Matrix;

/* Global variable that defines the pieces in game.c */
extern POSITION tetraminoes[7][4][4];

/* ----- Function Prototypes ----- */
/* For a brief description, refer to the comments in game.c above each function definition */
void game_play();
bool game_win_create();
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
int game_pause();
bool game_check_gameover(Matrix game_matrix);
void gameover_win();
void game_save_score(STATS score);
void init_colors();
void debug_colors();
void game_win_delete(Matrix game_array);

#endif