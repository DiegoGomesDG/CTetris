#include "leaderboard.h"

void scoreboard() {

    /* Create win_scoreboard */
    win_scoreboard = newwin(0,0,0,0);
    if (win_scoreboard == NULL) {
        wprintw(stdscr, "Error creating win_info");
        exit(-1);
    }

    
}

void print_headers(WINDOW * win) {

    

}