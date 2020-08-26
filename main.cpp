#include <curses.h>
#include <cstring>
#include <string>
#include <iostream>
#include <csignal>

int currentRPM=0;
int restoreRPM=0;

void signalHandler(int s){
    endwin();
}

void updateRPM(WINDOW *down, WINDOW *up) {
    mvwprintw(up, 1, 1, "entered rpm loop");
    wrefresh(up);
    int k = getch();

    wattron(down, COLOR_PAIR(2));

    switch (k) {
        case KEY_RIGHT:
            if(currentRPM!=37){
            currentRPM += 1; //Add to active rpm
            wprintw(up, "+ ");
            }
            break;

        case KEY_LEFT:
            if(currentRPM!=0) {
                currentRPM -= 1;
                restoreRPM += 1;
                wprintw(up, "- ");
            }
            break;
        case KEY_UP:
            endwin();
            free(up);
            free(down);
            break;
    }

    /*In the next block, we will redraw the whole bar, so we need to make use of \r
    *Also, \r deletes everything from the begging of the line, including the border, but that can wait to be fixed
    */
    std::string c = "\r " + std::string(currentRPM, '|');

    if (currentRPM >= 30) { //If actively in the redline, restore redline
        wattron(down, COLOR_PAIR(3));
        mvwprintw(down, 1, 1, c.c_str());
        wattroff(down, COLOR_PAIR(3));
    } else {
        mvwprintw(down, 1, 1, c.c_str());
    }

    //Restore catches the cursor where the previous block left it: at the end of the active RPM, so no need for \r
    if (restoreRPM) {
        //Switch to inactive RPM color pair to replace active RPMs
        wattroff(down, COLOR_PAIR(2));

        if (currentRPM >= 30) { //If actively in the redline, restore redline
            wattron(down, COLOR_PAIR(2));
            wprintw(down,std::string(restoreRPM, '|').c_str());
            wattroff(down, COLOR_PAIR(2));
        } else {
            wprintw(down, std::string(restoreRPM, '|').c_str());
        }
        restoreRPM-=1;
    }
}

int main()
{
    WINDOW *up, *down, *up2;

    signal(SIGINT, signalHandler);
    initscr();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);
    start_color();

    //Create color pairs for tachometer
    init_pair(2, COLOR_RED, COLOR_BLACK); //Redline
    init_pair(3, COLOR_WHITE, COLOR_RED); //"Active" RPM

    up = subwin(stdscr, LINES/2, COLS/2, 0, 0);
    up2 = subwin(stdscr, 5, COLS/2, 0, COLS/2);
    down = subwin(stdscr, 3, COLS/2+1, LINES/2, 0);

    box(up, ACS_VLINE, ACS_HLINE);
    box(down, ACS_VLINE, ACS_HLINE);
    box(up2, ACS_VLINE, ACS_HLINE);

    const char *msg = "Bonjour Golf :)";
    mvwprintw(up, LINES/2, (COLS / 2) - (strlen(msg) / 2), msg);

    //6*5 white RPM range + 1.5*5 redzone
    mvwprintw(down, 1, 1, std::string( 30, '|').c_str());
    wattron(down, COLOR_PAIR(2));
    wprintw(down, std::string( 1.5*5, '|').c_str());
    wattroff(down, COLOR_PAIR(2));
    wrefresh(up);
    wrefresh(down);

    int stopC=0;
    timeout(-1);

    do{
        updateRPM(down, up);
        wrefresh(up);
        wrefresh(down);
    } while(stopC!=KEY_UP);

    endwin();
    free(up);
    free(down);

}
