#include "menu.h"
#include "game.h"

int main(int argc, char * argv[]) {
    
    initscr();
    noecho();
    curs_set(0);

    int option = menu();
    if (option == 0) {
        game_window_create();
    }

    endwin();
    return 0; 

}