#include "scoreboard.h"

/* ---------- Display Leaderboard Functions ---------- */

/* Main scoreboard function - creates the windows, calls the functions and takes keyboard input */
void scoreboard() {

    /* Check for Size */
    getmaxyx(stdscr, LINES, COLS);
    if (LINES < 19|| COLS < 49) {
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

    /* Allow for keyboard inputs, initialise colors and color pairs */
    keypad(win_scoreboard, TRUE);
    start_color();
    init_pair(1, COLOR_BLACK, COLOR_CYAN); // Header
    init_pair(2, COLOR_RED, COLOR_BLACK); // Alert Red

    /* Sort the scores files and get number of entries */
    bool status = TRUE;
    int entries = 0;
    status = sort_file(win_scoreboard, DATE_SORTED_FILE, POINTS_SORTED_FILE, &entries);
    if (status == FALSE) {
        napms(5000); // Allowing time for user to see error
        win_delete(win_scoreboard); // Erase the window and exit
        return;
    }

    /* Print the table according to sorting */
    int middle = getmaxy(win_scoreboard)/2;
    int sort = 0;
    int input = ' ';

    /* Loop while ESC is not pressed or the file is not deleted */
    while(input != 27 && input != 'Y') {
        
        werase(win_scoreboard);
        print_table(win_scoreboard, sort, entries);
        wrefresh(win_scoreboard);

        input = wgetch(win_scoreboard);

        switch(input) {
            case KEY_LEFT: /* Change sorting type using <- */
                sort--;
                if (sort < 0) sort = 3;
                break;
            case KEY_RIGHT: /* Change sorting type using ->*/
                sort++;
                if (sort > 3) sort = 0;
                break;
            case 'd': /* Option to delete files and display confirmation message */
                wattron(win_scoreboard, COLOR_PAIR(2));
                center_string(win_scoreboard, middle - 7, "Press Shift + Y to Confirm");
                wattroff(win_scoreboard, COLOR_PAIR(2));
                wrefresh(win_scoreboard);
                napms(5000); /* 5 Second displaying the message */
                break;
            case 27:
                break;
        }

        /* Case the file was deleted*/
        if (input == 'Y') {
            wattron(win_scoreboard, COLOR_PAIR(2));
            remove(DATE_SORTED_FILE);
            remove(POINTS_SORTED_FILE);
            center_string(win_scoreboard, middle - 7, "Files deleted");
            wrefresh(win_scoreboard);
            napms(5000);
            break;
        }
    }

    /* Close the Window */
    win_delete(win_scoreboard);
    
}

/* Prints Title, Header and Rows Sorted according to user choice */
void print_table(WINDOW * win, int sort, int len) {

    /* Get center of the table */
    int v_center = getmaxy(win)/2;

    /* Print Table Title */
    print_sort_title(win, v_center - 7, sort);

    /* Print Table Header with Animation */
    print_headers(win, v_center - 5);

    /* Load the Sorted Data and Print the Rows */
    NODE * list = NULL;
    list = load_sorted_list(list, sort, len);
    if(list == NULL) return;
    NODE * ptr = list;

    for (int y = 0; y < 10 && ptr != NULL; y++) {
        print_row(win, ptr->data, v_center - 4 + y);
        ptr = ptr->next;
        
    }

    /* Print Controls */
    center_string(win, v_center + 7, "<- ->   Change Sorting");
    center_string(win, v_center + 8, "D     Delete Files");
    center_string(win, v_center + 9, "ESC  Exit Scoreboard");

    /* Delete Linked List */
    linked_list_delete(list);
}

/* Print the Title of the Table according to the Sorting */
void print_sort_title(WINDOW * win, int row, int sort) {

    if (sort > 4) return;
    wmove(win, 0, row);
    wclrtoeol(win);

    char * sort_types[] = {
        "Top Scores", /* 0 */
        "Worse Scores", /* 1 */
        "Most Recent Games", /* 2 */
        "Oldest Games" /* 3 */
    };

    wattron(win, A_BOLD | A_UNDERLINE);
    center_string(win, row, sort_types[sort]);
    wattroff(win, A_BOLD | A_UNDERLINE);

}

/* Print formatted table Headers */
void print_headers(WINDOW * win, int row) {
    
    wattron(win, A_BOLD | COLOR_PAIR(1));
    center_string(win, row, " RANK   POINTS   LINES      DATE        TIME   \n");
    wattroff(win, A_BOLD | COLOR_PAIR(1));

}

/* Get a data, transform into a string and print it centered into a row */
void print_row(WINDOW * win, GAME_DATA data, int row) {

    /* "00    999999   99999   YYYY-MM-DD   HH:MM:SS" */
    char entry[50];
    sprintf(entry, "%02d    %06d    %03d    %04d-%02d-%02d   %02d:%02d:%02d", data.rank,
    data.points, data.lines, data.t.tm_year, data.t.tm_mon, data.t.tm_mday, data.t.tm_hour, data.t.tm_min, data.t.tm_sec);
    center_string(win, row, entry);

}

/* Erase the Leaderboard and terminate Window */
void win_delete(WINDOW * win) {
    werase(win);
    werase(stdscr);
    refresh();
    wrefresh(win);
    delwin(win);
}

/* ---------- File I/O Functions ---------- */

/* Create the linked list if needed and add element to the end of the list */
NODE * linked_list_create(NODE * head, GAME_DATA data) {

    /* Create first node if head is NULL */
    if (head == NULL) {

        NODE * new = (NODE *)calloc(1, sizeof(NODE)); // Allocate node
        if (new == NULL) return NULL;

        /* Populate the Node */
        new->data = data; // Assign data
        new->next = NULL; // Next will be NULL
        head = new; // Head will point to the new node

        return head; // Return address to the first node

    }

    /* Add node to the end of the list */
    NODE * ptr = NULL; // Add auxiliary pointer
    for(ptr = head; ptr->next != NULL; ptr = ptr->next); // Go to the before the end node

    /* Create the node to the next node */
    ptr->next = (NODE *)calloc(1, sizeof(NODE));
    if (ptr->next == NULL) return NULL;

    /* Populate the Node */
    ptr->next->data = data; /* Assign Data */
    ptr->next->next = NULL; /* Next points to NULL */

    return head; 

}

/* Delete a Linked List recursively */
void linked_list_delete(NODE * head) {

    /* Return NULL if the head is NULL */
    if(head == NULL) return;
    
    /* Recursively delete the next node */
    linked_list_delete(head->next);
    
    /* Delete current node */
    free(head);

}

/* Loads file into a linked list with max length 10, either the first 10 or the last 10 entries */
NODE * load_sorted_list(NODE * head, int sort, int len) {

    GAME_DATA buffer;
    int count = 0;

    /* -------------------- Sorted by Points -------------------- */
    if (sort == 0 || sort == 1) {
        FILE * file = fopen(POINTS_SORTED_FILE, "r");
        if (file == NULL) return NULL;

        /* Top Scores - Load the First 10 Entries */
        if (sort == 0) {
            while(fscanf(file, "%d %d %d %d %d %d %d %d %d", &(buffer.rank), &(buffer.points), &(buffer.lines), &(buffer.t.tm_year), &(buffer.t.tm_mon), &(buffer.t.tm_mday), &(buffer.t.tm_hour), &(buffer.t.tm_min), &(buffer.t.tm_sec)) == 9) {
                head = linked_list_create(head, buffer);
                count++;
                if(count > 10) break;
            }
        }

        /* Worse Scores - Load the Last 10 Entries */
        if (sort == 1) {
            fseek(file, 0, SEEK_END);
            while(!fseek(file, -36L, SEEK_CUR)) { /* Go backwards 36 bytes in the file */
                if (fscanf(file, "%d %d %d %d %d %d %d %d %d", &(buffer.rank), &(buffer.points), &(buffer.lines), &(buffer.t.tm_year), &(buffer.t.tm_mon), &(buffer.t.tm_mday), &(buffer.t.tm_hour), &(buffer.t.tm_min), &(buffer.t.tm_sec)) == 9) {
                    head = linked_list_create(head, buffer);
                    count++;
                    fseek(file, -35L, SEEK_CUR); /* Go backwards 35 bytes after fscanf */
                }
                
                if(count > 10) break;
            }
        }

        fclose(file);
    }

    /* -------------------- Sorted by Date -------------------- */

    if (sort == 2 || sort == 3) {
        FILE * file = fopen(DATE_SORTED_FILE, "r");
        if (file == NULL) return NULL;

        /* Oldest Games - Load the first 10 Entries */
        if (sort == 2) {
            while(fscanf(file, "%d %d %d %d %d %d %d %d", &(buffer.points), &(buffer.lines), &(buffer.t.tm_year), &(buffer.t.tm_mon), &(buffer.t.tm_mday), &(buffer.t.tm_hour), &(buffer.t.tm_min), &(buffer.t.tm_sec)) == 8) {
                buffer.rank = len - count;
                head = linked_list_create(head, buffer);
                count++;
                if(count > 10) break;
            }
        }

        /* Newest Games - Load the last 10 Entries */
        if (sort == 3) {
            fseek(file, 0, SEEK_END);
            while(!fseek(file, -33L, SEEK_CUR)) { /* Go backwards 33 bytes in the file */
                if (fscanf(file, "%d %d %d %d %d %d %d %d", &(buffer.points), &(buffer.lines), &(buffer.t.tm_year), &(buffer.t.tm_mon), &(buffer.t.tm_mday), &(buffer.t.tm_hour), &(buffer.t.tm_min), &(buffer.t.tm_sec)) == 8) {
                    buffer.rank = count + 1; /* Add date rank */
                    head = linked_list_create(head, buffer);
                    count++;
                    fseek(file, -32L, SEEK_CUR); /* Go backwards 32 bytes after fscanf */
                }
                
                if(count > 10) break;
            }
        }

        fclose(file);
    }

    return head;

}

/* Comparison function for qsort */
int sort_compare(const void* a, const void* b) {
    return ((GAME_DATA *)b)->points - ((GAME_DATA *)a)->points;
}

/* Sorts the scores files in a descending order according to the score */
bool sort_file(WINDOW * win, char * filename, char * out, int * len) {

    /* Coordinate to print error messages */
    wattron(win, A_BOLD);
    int error_y = getmaxy(win)/2;

    /* Open file that saves the scores */
    FILE * input = fopen(filename, "r");
    if (input == NULL) {
        center_string(win, error_y, "No File Available");
        center_string(win, error_y + 1, "Play some Tetris :)");
        wrefresh(win);
        return FALSE;
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
        return FALSE;
    }

    /* Scan each entry into a list of size list_length */
    fseek(input, 0, SEEK_SET);
    GAME_DATA buffer;
    buffer.rank = 0;

    GAME_DATA * arr = (GAME_DATA *)malloc(list_length * sizeof(GAME_DATA));
    if(arr == NULL) {
        center_string(win, error_y, "Error allocating space");
        wrefresh(win);
        return FALSE;
        }

    /* Loop and copy buffer into array */
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

    /* QSORT the list */
    FILE * output = fopen(out, "w");
    if (output == NULL) {
        center_string(win, error_y, "Error creating sorted file");
        wrefresh(win);
        return FALSE;
    }

    qsort(arr, list_length, sizeof(GAME_DATA), sort_compare);

    /* Write the Sorted List into a File */
    for(int e = 0; e < list_length; e++) {
        fprintf(output, "%02d %06d %05d %04d %02d %02d %02d %02d %02d\n", e + 1, arr[e].points, arr[e].lines, arr[e].t.tm_year, arr[e].t.tm_mon, arr[e].t.tm_mday, arr[e].t.tm_hour, arr[e].t.tm_min, arr[e].t.tm_sec);
    }

    /* Return the length/number of entries */
    *len = list_length;

    /* Close files and free memory*/
    wattroff(win, A_BOLD);
    fclose(output);
    free(arr);
    return TRUE;
}