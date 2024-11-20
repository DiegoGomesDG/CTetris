#ifndef MENU_H
#define MENU_H

/* ----- Libraries ----- */
#include <ncurses.h>
#include <stdlib.h>
#include <string.h>

/* ----- Constants ----- */
#define WIN_MENUY 11
#define WIN_MENUX 18
#define MENU_OPTION_COUNT 4

/* ----- Function Prototype ----- */
int menu();
void menu_highlight_option(WINDOW * menu, char * option[], int optnum);
void center_string(WINDOW * win, int row, char * str);

#endif