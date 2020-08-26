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
            currentRPM += 1; //Add to active rpm
            wprintw(up, "+ ");
            break;
        case KEY_LEFT:
            currentRPM -= 1;
            restoreRPM += 1;
            wprintw(up, "- ");
            break;
    }
    if (currentRPM >= 6 * 5) { //If actively in the redline, restore redline
        wattron(down, COLOR_PAIR(3));
    }
    mvwprintw(down, 1, 1, std::string(currentRPM, '|').c_str());
    if (currentRPM >= 6 * 5) { //If actively in the redline, restore redline
        wattroff(down, COLOR_PAIR(3));
    }
    if(restoreRPM){
        //Switch to inactive RPM color pair to replace active RPMs
        wattroff(down, COLOR_PAIR(2));

        if(currentRPM>=6*5){ //If actively in the redline, restore redline
            wattron(down, COLOR_PAIR(2));
        }

        wprintw(down, std::string( restoreRPM, '|').c_str());

        //Restore active RPM color pair
        if(currentRPM>=6*5){
            wattroff(down, COLOR_PAIR(2));
        }
        wattron(down, COLOR_PAIR(2));
    }
}

int main()
{
    WINDOW *up, *down;

    signal(SIGINT, signalHandler);
    initscr();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);
    start_color();

    //Create color pairs for tachometer
    init_pair(2, COLOR_RED, COLOR_BLACK); //Redline
    init_pair(3, COLOR_WHITE, COLOR_RED); //"Active" RPM

    up = subwin(stdscr, LINES/2, COLS, 0, 0);
    down = subwin(stdscr, LINES/2, COLS, LINES/2, 0);

    box(up, ACS_VLINE, ACS_HLINE);
    box(down, ACS_VLINE, ACS_HLINE);

    const char *msg = "Bonjour Golf :)";
    mvwprintw(up, LINES/2, (COLS / 2) - (strlen(msg) / 2), msg);

    //6*5 white RPM range + 1.5*5 redzone
    mvwprintw(down, 1, 1, std::string( 6*5, '|').c_str());
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
