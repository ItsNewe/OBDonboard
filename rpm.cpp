#include "rpm.h"

Rpm::Rpm(){
	this->currentRPM=0;
	this->restoreRPM=0;
}

void Rpm::updateRPM(WINDOW *down, WINDOW *up, serialCom *s) {
	//TODO: Redline values stay stuck in the beggining area of the bar → bad division??


    std::string rawData = s->sendMessage("010C\r", 0);

    //Clean raw data string
    rawData.erase(std::remove(rawData.begin(), rawData.end(), ' '), rawData.end());
    rawData.erase(std::remove(rawData.begin(), rawData.end(), '\r'), rawData.end());
    rawData.erase(std::remove(rawData.begin(), rawData.end(), '>'), rawData.end());

    std::string paramA = rawData.substr(rawData.length()-4, 2); //Get first data byte
    std::string paramB = rawData.substr(rawData.length()-2, 2); //Get second data byte

    //Ugly cleanup nasty nasty
    restoreRPM=currentRPM;
    currentRPM=(256*std::stoul(paramA, nullptr, 16)+std::stoul(paramB, nullptr, 16))/4;

	//Since both values are unsigned int, we need to check for ↓↓↓ underflow with this operation, else restoreRPM would contain a garbage value
    restoreRPM=(!(restoreRPM-currentRPM>restoreRPM))?(restoreRPM-currentRPM):0;

    mvwprintw(up, 1, 1, std::to_string(currentRPM).c_str());
    wrefresh(up);


    /*In the next block, we will redraw the whole bar, so we need to make use of \r
    *Also, \r deletes everything from the begging of the line, including the border, but that can wait to be fixed
    */
    std::string c = "\r " + std::string(currentRPM/250, '|');
	

    if (this->currentRPM >= 7000) { //If actively in the redline, restore redline
        wattron(down, COLOR_PAIR(3));
        mvwprintw(down, 1, 1, c.c_str());
        wattroff(down, COLOR_PAIR(3));
    } else {
		wattron(down, COLOR_PAIR(2));
        mvwprintw(down, 1, 1, c.c_str());
		wattroff(down, COLOR_PAIR(2));
    }

    //Restore catches the cursor where the previous block left it: at the end of the active RPM, so no need for \r
    if (restoreRPM) {
        //Switch to inactive RPM color pair to replace active RPMs
        wattroff(down, COLOR_PAIR(2));

        if (this->currentRPM >= 6500) { //If actively in the redline, restore redline
            wattron(down, COLOR_PAIR(2));
            wprintw(down, std::string(37.5-this->restoreRPM/250, '|').c_str());
            wattroff(down, COLOR_PAIR(2));
        } else {
            wprintw(down, std::string(this->restoreRPM/250, '|').c_str());
        }
        restoreRPM ==0;
    }
}

