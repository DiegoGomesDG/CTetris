#ifndef LEADERBOARD_H
#define LEADERBOARD_H

/* ----- Libraries ----- */
#include "menu.h" // Import center_string
#include <ncurses.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

/* ----- Relative File Path ----- */
#define DATE_SORTED_FILE "./files/games.txt"
#define POINTS_SORTED_FILE "./files/scores_sorted.txt"
#define ROWS 10

/* GAME_DATA struct */
/* 00 999999 99999 yyyy-mm-dd hh-mm-ss*/
typedef struct game_data {
    int rank;
    int points;
    int lines;
    struct tm t;     
} GAME_DATA;

/* Linked List NODE struct */
typedef struct node {
    struct game_data data;
    struct node *next;
} NODE;

/* Window and Print Table Functions Prototype */
void scoreboard();
void print_sort_title(WINDOW * win, int row, int sort);
void print_table(WINDOW * win, int sort, int len);
void print_headers(WINDOW * win, int row);
void print_row(WINDOW * win, GAME_DATA data, int row);
void win_delete(WINDOW * win);

/* File I/O and List Creation Functions Prototype */
NODE * linked_list_create(NODE * head, GAME_DATA data);
void linked_list_delete(NODE * head);
NODE * load_sorted_list(NODE * head, int sort, int len);
bool sort_file(WINDOW * win, char * filename, char * out, int * len);
int sort_compare(const void* a, const void* b);


#endif