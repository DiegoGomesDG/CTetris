#include "menu.h"
#include "info.h"

WINDOW * win_info;

/* Display the information */
void win_info_display() {

    /* Create window */
    getmaxyx(stdscr, COLS, LINES);
    win_info = newwin(COLS - BORDER, LINES - BORDER, BORDER/2, BORDER/2);
    if (win_info == NULL) {
        wprintw(stdscr, "Error creating win_info");
        exit(-1);
    }

    int win_infox = 0, win_infoy = 0;
    getmaxyx(win_info, win_infoy, win_infox);
    
    /* Display first page */
    pages(win_info, 0, win_infox, win_infoy);

    int input;
    int page = 0;
    
    while (1) {
        input = wgetch(win_info);
        switch (input) {
            case 27:
                werase(win_info);
                wrefresh(win_info);
                delwin(win_info);
                return;
            case ' ':
                page++;
                if (page > 2) page = 0;
                break;
        }
        
        werase(win_info);
        pages(win_info, page, win_infox, win_infoy);

    }
    
} 

/* Center the title and apply bold and underline */
void centered_title(WINDOW * win, int row, char * str) {
    wattron(win, A_UNDERLINE | A_BOLD);
    center_string(win, row, str);
    wattroff(win, A_UNDERLINE | A_BOLD);
}

/* Allows to write text in the window without cutting words */
void text_wrapper(WINDOW * win, int row, int width, char * str) {
    
    int y = 0;
    int len = strlen(str);
    char * buffer = (char *)malloc((width + 1)*sizeof(char));
    int cursor = 0, fcursor = width;
    int flag = 1;

    while (flag) {
        
        if(fcursor > len) {
            fcursor = len;
            flag = 0;
        }

        while (!isspace(str[fcursor]) && flag)
            fcursor--;

        // Copy text from x to cursor
        int i = 0;
        for (; cursor <= fcursor && i < width; cursor++, i++) {
            buffer[i] = str[cursor];
        }

        buffer[i] = '\0';
        mvwprintw(win, row + y, 0, "%s", buffer);
        y++; fcursor += width;
    }

    free(buffer);
}

/* Define the Pages */
void pages(WINDOW * win, int page, int width, int heigth) {

    if (page == 0) {
        centered_title(win, 0, " ABOUT ");
        text_wrapper(win, 2, width, "  This program was created by the student Diego Davidovich Gomes as the homework project for the Basics of Programming 1 subject at Budapest University of Technology and Economics, Computer Engineering BSc, 1st Semester, Academic Year 2024/25, under the supervision of the laboratory guide, Professor Dr. Horváth Gábor. The purpose of this project was to apply foundational programming concepts, including algorithm design, structured programming, and problem-solving, in the creation of a complex functional and interactive program. The game is implemented in the C programming language, utilizing the ncurses library for terminal-based graphics and user interaction, available for MacOS and Linux.");
        center_string(win, heigth - 2, "SPACE - Switch Page ");
        center_string(win, heigth - 1, "ESC - Exit");
        wrefresh(win);
        return;
    }

    if (page == 1) {
        centered_title(win_info, 0, " QUICK GUIDE ");
        text_wrapper(win_info, 2, width, "Tetris is a classic game, originally created in 1985. In Tetris, you should complete lines by moving differently shaped pieces tetrominoes, which slowly descend into the playing field. When a line is completed, the line disappers and the pieces on top drop one rank, and you get points. Multiple lines can be completed at once, which grants more points. The game ends when the uncleared lines reach the top and a piece gets locked outside of the playfield. The longer the player can delay the gameover, the higher their score will be.");

        center_string(win, heigth - 2, "SPACE - Switch Page ");
        center_string(win, heigth - 1, "ESC - Exit");
        wrefresh(win);
        return;
    }

    if (page == 2) {
        centered_title(win_info, 0, " CONTROLS ");
        center_string(win_info, 2, "<-  Move Left");
        center_string(win_info, 3, "->  Move Right");
        center_string(win_info, 4, "v   Move Down");
        center_string(win_info, 5, "_   Hard Drop");
        center_string(win_info, 6, "^   Rotate   ");
        center_string(win_info, 7, "P   Pause    ");
        center_string(win_info, 8, "Q   Quit Game");

        center_string(win, heigth - 2, "SPACE - Switch Page ");
        center_string(win, heigth - 1, "ESC - Exit");
        wrefresh(win);
        return;
    }
}

