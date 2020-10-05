#include <curses.h>
#include <cstring>
#include <string>
#include <csignal>
#include "rpm.h"
#include "serialCom.h"

void signalHandler(int s){
    endwin();
}

//Clion liting stuff, ignore
#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"
int main()
{
    WINDOW *up, *down, *up2;

    auto *s = new serialCom("/dev/pts/4"); //Set serial port to be used here
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

    Rpm *rpm = new Rpm();

    do{
		sleep(1);
        rpm->updateRPM(down, up, s);
        //stopC=getch();
        wrefresh(up);
        wrefresh(up2);
        wrefresh(down);
    } while(1);

    endwin();
    free(up);
    free(down);

}
#pragma clang diagnostic pop
