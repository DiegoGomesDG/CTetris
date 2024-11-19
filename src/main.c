#include "menu.h"
#include "game.h"
#include "scoreboard.h"
#include "info.h"

int main(int argc, char * argv[]) {

    initscr();
    noecho();
    cbreak();
    curs_set(0);
    
    while (1) {
        int option = menu();
        switch (option) {

        case 0: game_play();
                break;
        case 1: scoreboard();
                break;
        case 2: win_info_display();
                break;
        case 3: endwin(); return 0;
        default: endwin(); return 1;

        }
    }
    
    endwin();
    return 0; 

}