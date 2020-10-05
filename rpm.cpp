#include "rpm.h"

void Rpm::updateRPM(WINDOW *down, WINDOW *up) {
    mvwprintw(up, 1, 1, "entered rpm loop");
    wrefresh(up);
    int k = getch();

    wattron(down, COLOR_PAIR(2));

    switch (k) {
        case KEY_RIGHT:
            if (currentRPM != 37) {
                currentRPM += 1; //Add to active rpm
                wprintw(up, "+ ");
            }
            break;

        case KEY_LEFT:
            if (currentRPM != 0) {
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
            wprintw(down, std::string(restoreRPM, '|').c_str());
            wattroff(down, COLOR_PAIR(2));
        } else {
            wprintw(down, std::string(restoreRPM, '|').c_str());
        }
        restoreRPM -= 1;
    }
}

