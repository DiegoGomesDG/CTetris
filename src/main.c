#include "menu.h"
#include "game.h"
#include "scoreboard.h"
#include "info.h"

/* PROGRAM MAIN LOOP AND EXECUTION */
int main(void) {
        
    /* Start ncurses */
    initscr(); // Initialise screen
    noecho(); // Don't display input keys
    cbreak(); // Disable line buffering
    curs_set(0); // Hide cursor
    
    /* Execution Loop */
    while (1) {

        int option = menu(); /* Get an option from the user*/
        
        switch (option) {

            case 0: game_play(); /* Play the Game*/
                    break;
            case 1: scoreboard(); /* Load the Scoreboard */
                    break;
            case 2: win_info_display(); /* Display Info */
                    break;
            case 3: endwin(); return 0; /* Exit */
            default: endwin(); return 1;

        }
    }
    
    endwin();
    return 0; 

}