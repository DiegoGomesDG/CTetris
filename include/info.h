#ifndef INFO_H
#define INFO_H

#include "menu.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <ncurses.h>

#define BORDER 4

WINDOW * win_info;

void win_info_display();
void pages(WINDOW * win, int page, int width, int heigth);
void centered_title(WINDOW * win, int row, char * str);
void text_wrapper(WINDOW * win, int row, int width, char * str);

#endif