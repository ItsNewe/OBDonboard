#include "obd.h"

Obd::Obd(serialCom *s){
	this->s=s;

	this->currentRPM=0;
	this->restoreRPM=0;
}

//RPM
void Obd::updateRPM(WINDOW *down, WINDOW *up) {
	//TODO: Redline values stay stuck in the beggining area of the bar → bad division??


	std::string rawData = s->sendMessage("010C\r", 0);
	std::this_thread::sleep_for(std::chrono::milliseconds(500));

	
	if(rawData[0]!='7'){
		printf("Invalid serial frame received, aborting"); //Move to serialCom?
		return;
	}

	rawData = s->cleanUpSerialFrame(rawData);

	std::string paramA = rawData.substr(9, 2); //Get first data byte
	std::string paramB = rawData.substr(11, 2); //Get second data byte

	restoreRPM=currentRPM;
	currentRPM=(256*std::stoul(paramA, nullptr, 16)+std::stoul(paramB, nullptr, 16))/4;

	//Since both values are unsigned int, we need to check for any underflow, or else restoreRPM would contain garbage
	restoreRPM=(restoreRPM-currentRPM>0)? restoreRPM-currentRPM: 0;

	mvwprintw(up, 1, 1, std::to_string(currentRPM).c_str());
	wrefresh(up);


	/*In the next block, we will redraw the whole bar, so we need to make use of \r
	*Also, \r deletes everything from the begging of the line, including the border, but that can wait to be fixed
	*/
	std::string c = "\r " + std::string(currentRPM/250, '|');
	

	if (this->currentRPM >= 7000) { //If actively in the redline, change color to indicate that
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

//FUEL

double Obd::getInstantFuel()
{
	std::string speedRaw = s->sendMessage("010D\r", 0);
	std::string mafRaw = s->sendMessage("0110\r", 0);

	speedRaw = s->cleanUpSerialFrame(speedRaw);
	mafRaw = s->cleanUpSerialFrame(mafRaw);

	std::string paramA = mafRaw.substr(9, 2);  //Get first data byte
	std::string paramB = mafRaw.substr(11, 2); //Get second data byte
	int speed = stoi(speedRaw.substr(9, 2));

	double maf = (256*std::stoul(paramA, nullptr, 16)+std::stoul(paramB, nullptr, 16))/100;
	double result = (speed*7.718)/maf;
	return result;
}