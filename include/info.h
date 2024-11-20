#ifndef INFO_H
#define INFO_H

/* ----- Libraries ----- */
#include "menu.h" /* Import center_string */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <ncurses.h>

/* ----- Constants ----- */
#define BORDER 4 /* Define the space between text and sides */

/* ----- Function Prototypes ----- */
void win_info_display();
void pages(WINDOW * win, int page, int width, int heigth);
void centered_title(WINDOW * win, int row, char * str);
void text_wrapper(WINDOW * win, int row, int width, char * str);

#endif