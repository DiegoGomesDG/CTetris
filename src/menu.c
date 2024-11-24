#include "menu.h"

/* Generates the menu, display the options and returns the choosed option */
int menu() {

    int winx, winy;
    int key, menuoption = 0;
    getmaxyx(stdscr, winy, winx);
    
    // Creates the subwindow
    WINDOW * win_menu = subwin(stdscr, WIN_MENUY, WIN_MENUX, (winy - WIN_MENUY)/2, (winx - WIN_MENUX)/2);
    if (win_menu == NULL) {
        waddstr(stdscr, "Error!");
        exit(-1);
    }

    // Activate keypad input,
    keypad(win_menu, TRUE);
    noecho(); // Disables the character appearing in the screen
    
    // Options
    char * options[] = {
        " Play ",
        " Scoreboard ",
        " Info ",
        " Exit "
    };

    do {
    // Draw box around the window
    box(win_menu, 0, 0);

    // Add TETRIS title, print menu with first highlighted and commands
    center_string(win_menu, 0, "TETRIS");
    menu_highlight_option(win_menu, options, menuoption);
    
    int y_center = getmaxy(stdscr)/2; 
    center_string(stdscr, y_center + 6, "Use ARROW keys to navigate");
    center_string(stdscr, y_center + 7, "Press ENTER to select");
    refresh();

    // Highlight Options and Choose the Option using arrow keys
    
        getmaxyx(stdscr, winy, winx);
        key = wgetch(win_menu);
        switch (key) {
            /* Select one option up */
            case KEY_DOWN: 
                menuoption++;
                if (menuoption > MENU_OPTION_COUNT - 1) menuoption = 0;
                break;
            /* Select one option down */
            case KEY_UP:
                menuoption--;
                if (menuoption < 0) menuoption = MENU_OPTION_COUNT - 1;
                break;
            case KEY_RESIZE:
                werase(win_menu);
                wrefresh(win_menu);
                erase();
                mvwin(win_menu, (winy - WIN_MENUY)/2, (winx - WIN_MENUX)/2);
                break;
            default: break;
        }
        menu_highlight_option(win_menu, options, menuoption);
    } while (key != '\n');
    
    werase(win_menu);
    werase(stdscr);
    keypad(win_menu, FALSE);
    delwin(win_menu);
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
    
    int winx;
    winx = getmaxx(win);

    int str_length = strlen(str);
    int center = winx - str_length;
    mvwaddstr(win, row, center/2, str);

}