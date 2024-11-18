#ifndef INFO_H
#define INFO_H

#include "menu.h"
#include <stdio.h>
#include <string.h>
#include <ncurses.h>

#define BORDER 2

WINDOW * win_info;

void centered_title(WINDOW * win, int row, char * str);
void text_wrapper(WINDOW * win, int * row, int width, char * str);
void pages(WINDOW * win, int page, int width);
void win_info_display();

#endif