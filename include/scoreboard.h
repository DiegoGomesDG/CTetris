#ifndef LEADERBOARD_H
#define LEADERBOARD_H

#include "menu.h" // Import center_string
#include <ncurses.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

/* game data struct a*/
/* 999999 99999 yyyy-mm-dd hh-mm-ss*/
typedef struct game_data {
    int rank;
    int points;
    int lines;
    struct tm t;     
} GAME_DATA;

/* linked list node struct */
typedef struct node {
    struct game_data data;
    struct node *next;
} NODE;

/* Window Functions */
void scoreboard();
void print_sort_title(WINDOW * win, int row, int title);
void print_headers(WINDOW * win, int row);
void print_row(WINDOW * win, GAME_DATA data, int row, int place);
void scoreboard_colors();

/* File I/O Functions */
NODE * node_create(NODE * head, GAME_DATA data);
void sort_file(WINDOW * win, char * filename, char * out);
int sort_compare(const void* a, const void* b);


#endif