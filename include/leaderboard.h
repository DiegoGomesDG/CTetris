#ifndef LEADERBOARD_H
#define LEADERBOARD_H

#include <ncurses.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

WINDOW * win_scoreboard;
WINDOW * win_table;

/* game data struct a*/
typedef struct game_data {
    int points;
    int lines;
    struct tm t;     
} GAME_DATA;

/* linked list node struct */
typedef struct node {
    struct game_data data;
    struct node *next;
} NODE;

void scoreboard();
void print_headers(WINDOW * win);
void print_row();

#endif