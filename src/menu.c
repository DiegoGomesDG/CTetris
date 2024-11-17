#include "menu.h"

/* Generates the menu, display the options and returns the choosed option */
int menu() {

    int winx, winy;
    int key, menuoption = 0;
    getmaxyx(stdscr, winy, winx);
    
    // Creates the subwindow
    WINDOW * menu = subwin(stdscr, WIN_MENUY, WIN_MENUX, (winy - WIN_MENUY)/2, (winx - WIN_MENUX)/2);
    if (menu == NULL) {
        waddstr(stdscr, "Error!");
        exit(-1);
    }

    // Activate keypad input,
    keypad(menu, TRUE);
    noecho(); // Disables the character appearing in the screen
    
    // Options
    char * options[] = {
        " Play ",
        " Scoreboard ",
        " Info ",
        " Exit "
    };
    
    // Draw box around the window
    box(menu, 0, 0);

    // Add TETRIS title, print menu with first highlighted
    center_string(menu, 0, "TETRIS");
    menu_highlight_option(menu, options, menuoption);
    
    // Highlight Options and Choose the Option using arrow keys
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
    werase(menu);
    delwin(menu);
    return menuoption;
}

/* Auxiliary function responsible for highlighting the current option */
void menu_highlight_option(WINDOW * menu, char * option[], int optnum) {

    for (int c = 0; c < MENU_OPTION_COUNT; c++) {
        if (c == optnum) 
            wattron(menu, A_STANDOUT);
        center_string(menu, (2 + c*2), *(option + c));
        wattroff(menu, A_STANDOUT);
    }
    wrefresh(menu);

}

/* Auxiliary function which prints the text centered to the window*/
void center_string(WINDOW * win, int row, char * str) {
    
    int winy, winx;
    getmaxyx(win, winy, winx);

    int str_length = strlen(str);
    int center = winx - str_length;
    mvwaddstr(win, row, center/2, str);

}