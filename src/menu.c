#include "menu.h"

int menu() {

    int winx, winy;
    int key, menuoption = 0;
    getmaxyx(stdscr, winy, winx);
    
    WINDOW * menu = subwin(stdscr, WIN_MENUY, WIN_MENUX, (winy - WIN_MENUY)/2, (winx - WIN_MENUX)/2);
    if (menu == NULL) {
        waddstr(stdscr, "Error!");
        exit(1);
    }

    // Activate
    keypad(menu, TRUE);
    noecho();
    
    // Options
    char * options[] = {
        " Play ",
        " Scoreboard ",
        " Info ",
        " Exit "
    };
    
    // Draw the box
    box(menu, 0, 0);

    // Add TETRIS title, print menu with first highlighted
    center_string(menu, 0, "TETRIS");
    menu_highlight_option(menu, options, menuoption);
    
    // Highlight Option and Choose the Option
    do {
        key = wgetch(menu);
        switch (key) {
            case KEY_DOWN: 
                menuoption++;
                if (menuoption > MENU_OPTION_COUNT - 1) menuoption = 0;
                break;
            case KEY_UP:
                menuoption--;
                if (menuoption < 0) menuoption = MENU_OPTION_COUNT - 1;
                break;
            default: break;
        }
        menu_highlight_option(menu, options, menuoption);
    } while (key != '\n');
    
    delwin(menu);
    return menuoption;
}

void menu_highlight_option(WINDOW * menu, char * option[], int optnum) {

    for (int c = 0; c < MENU_OPTION_COUNT; c++) {
        if (c == optnum) 
            wattron(menu, A_STANDOUT);
        center_string(menu, (2 + c*2), *(option + c));
        wattroff(menu, A_STANDOUT);
    }
    wrefresh(menu);

}

void center_string (WINDOW * win, int row, char * str) {
    
    int winy, winx;
    getmaxyx(win, winy, winx);

    int str_length = strlen(str);
    int center = winx - str_length;
    mvwaddstr(win, row, center/2, str);

}