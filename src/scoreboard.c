#include "scoreboard.h"

/* ---------- Display Leaderboard Functions ---------- */

void scoreboard() {

    /* Check for Size */
    getmaxyx(stdscr, LINES, COLS);
    if (LINES < 15|| COLS < 49) {
        attron(A_STANDOUT | A_UNDERLINE);
        mvwprintw(stdscr, (LINES)/2 - 8, (COLS - 30)/2, "Please make the window bigger!");
        attroff(A_STANDOUT | A_UNDERLINE);
        refresh();
        return;
    }

    /* Create win_scoreboard */
    WINDOW * win_scoreboard = newwin(0,0,0,0);
    if (win_scoreboard == NULL) {
        wprintw(stdscr, "Error creating win_info");
        exit(-1);
    }
    
    int y_center = getmaxy(win_scoreboard)/2;
    print_headers(win_scoreboard, y_center - 5);
    wrefresh(win_scoreboard);
    sort_file(win_scoreboard, "files/scores.txt", "files/scores_sorted.txt");
    napms(10000);

    /* Close the Window */
    werase(win_scoreboard);
    wrefresh(win_scoreboard);
    delwin(win_scoreboard);
    
}

void print_sort_title(WINDOW * win, int row, int title) {

    if (title > 3) return;

    char * sort_types[] = {
        " Points (Ascending) ",
        " Points (Descending) ",
        " Most Recent Games",
        " Oldest Games"
    };

    wattron(win, A_BOLD | A_UNDERLINE);
    center_string(win, row, sort_types[title]);
    wattroff(win, A_BOLD | A_UNDERLINE);

}

void print_headers(WINDOW * win, int row) {
    
    wattron(win, A_BOLD | COLOR_PAIR(1));
    center_string(win, row, " RANK   POINTS   LINES      DATE        TIME   ");
    wattroff(win, A_BOLD | COLOR_PAIR(1));
    center_string(win, row+1, " 00    999999   99999   YYYY-MM-DD   HH:MM:SS");
}

void print_row(WINDOW * win, GAME_DATA data, int row, int place) {

    /* "00    999999   99999   YYYY-MM-DD   HH:MM:SS" */
    switch (place) {
        case 1: wattrset(win, A_BOLD | COLOR_PAIR(2));
                break;
        case 2: wattrset(win, A_BOLD | COLOR_PAIR(3));
                break;
        case 3: wattrset(win, A_BOLD | COLOR_PAIR(4));
                break;
        default: break;
    }
    
    mvwprintw(win, 1, row, "%02d   %06d   %05d   %04d-%02d-%02d   %02d:%02d:%02d", data.rank,
    data.points, data.lines, data.t.tm_year, data.t.tm_mon, data.t.tm_mday, data.t.tm_hour, data.t.tm_min, data.t.tm_sec);
    wattrset(win, A_NORMAL);
}

void scoreboard_colors() {
    
    start_color();

    if (can_change_color()) {
        init_color(11, 750, 750, 750); // Silver
        init_color(10, 800, 500, 200);

        init_pair(3, 11, COLOR_BLACK); // 2nd Place
        init_pair(4, 12, COLOR_BLACK); // 3rd Place

    } else {
        init_pair(3, COLOR_WHITE, COLOR_BLACK); // 2nd Place
        init_pair(4, COLOR_RED, COLOR_BLACK); // 3rd Place
    }

    init_pair(1, COLOR_BLACK, COLOR_CYAN); // Header
    init_pair(2, COLOR_YELLOW, COLOR_BLACK); // 1st Place
    
}

/* ---------- File I/O Functions ---------- */

NODE * node_create(NODE * head, GAME_DATA data) {

    NODE * new = (NODE *)calloc(1, sizeof(NODE));
    if (new == NULL) return NULL;

    /* Populate the Node */
    new->data = data; /* Data */
    new->next = head; /* Next points to the node*/
    return new; /* Point to new node */

}

/* Comparison function for qsort */
int sort_compare(const void* a, const void* b) {
    return ((GAME_DATA *)b)->points - ((GAME_DATA *)a)->points;
}

/* Sorts the scores files in a descending order according to the score */
void sort_file(WINDOW * win, char * filename, char * out) {

    /* Get coordinate to print error messages */
    int error_y = getmaxy(win)/2 - 7;

    FILE * input = fopen(filename, "r");
    if (input == NULL) {
        center_string(win, error_y, "No File Available");
        wrefresh(win);
        return;
        }

    /* Counts the number of entries */
    int list_length = 0;
    char ch;

     while ((ch = fgetc(input)) != EOF) {
        if (ch == '\n') { // Newline character indicates a new line
            list_length++;
        }
    }

    /* Check for empty file */
    if(list_length == 0) {
        center_string(win, error_y, "No scores registered! Play some games :)");
        wrefresh(win);
        return;
    }

    /* Scan each entry into a list*/
    fseek(input, 0, SEEK_SET);
    GAME_DATA buffer;
    buffer.rank = 0;

    GAME_DATA * arr = (GAME_DATA *)malloc(list_length * sizeof(GAME_DATA));
    if(arr == NULL) {
        center_string(win, error_y, "Error allocating space");
        wrefresh(win);
        }

    for (int i = 0; i < list_length; i++) {

        if(fscanf(input, "%d %d %d %d %d %d %d %d", &(buffer.points), &(buffer.lines), &(buffer.t.tm_year), &(buffer.t.tm_mon), &(buffer.t.tm_mday), &(buffer.t.tm_hour), &(buffer.t.tm_min), &(buffer.t.tm_sec)) == 8) {
            arr[i] = buffer;
        } else {
            center_string(win, error_y, "Error in Reading the Scores from the File");
            wrefresh(win);
            break;
        }

    }
    fclose(input);

    /* QSORT the list  */
    FILE * output = fopen(out, "w");
    if (output == NULL) {
        center_string(win, error_y, "Error creating sorted file");
        wrefresh(win);
    }

    qsort(arr, list_length, sizeof(GAME_DATA), sort_compare);

    /* Write the Sorted List */
    for(int e = 0; e < list_length; e++) {
        fprintf(output, "%d %d %d %d %d %d %d %d %d\n", e + 1, arr[e].points, arr[e].lines, arr[e].t.tm_year, arr[e].t.tm_mon, arr[e].t.tm_mday, arr[e].t.tm_hour, arr[e].t.tm_min, arr[e].t.tm_sec);
    }

    /* Close files and free memory*/
    fclose(output);
    free(arr);
}