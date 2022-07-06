#include <curses.h>
#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include <csignal>
#include <stdexcept> //std::runtime_err
#include <iomanip>
#include "serialCom.h"
#include "obd.h"

void signalHandler(int s){
	endwin();
}

int writeConfToFile(std::string* s={}, int* r={}){
	/*
	* HEX FILE: FORMAT LL[D*L]
	* L=2 bytes for the length
	* D=L bytes for the data
	* data to be stored: dev name, refresh rate, obd protocol?
	*/
	printf("entered config lkoop\n");
	std::ofstream file;
	file.open("config.obdo");

	if(file.is_open()){

		fmt::print("{} : {}\n{} : {}\n", s->length(), *s, sizeof(*r), *r);

		file << std::hex << s->length()*2;
		
		for(char& c : *s){
			file << std::hex << (int)c;
		}

		char buf[sizeof(*r)] = {};
		memcpy(&buf, r, sizeof(*r));
		file << std::hex << sizeof(*r) << std::setfill('0') << std::setw(sizeof(*r)) << std::hex << buf;

		
	}
	
	file.close();
	return 0;
}

//Clion liting stuff, ignore
#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"
int main(int argc, char *argv[])
{
	//Handle cli arguments
	std::string serialDeviceName="/dev/serial0";
	int dataRefreshRate=100;
	int v;
	writeConfToFile(&serialDeviceName, &dataRefreshRate);

	while((v = getopt(argc, argv, "d:r:s")) != -1){
		switch(v){
		case 'd':
			serialDeviceName=optarg;
			break;
		case 'r':
			dataRefreshRate=std::stoi(optarg);
			break;
		case 's':
			writeConfToFile(&serialDeviceName, &dataRefreshRate);
			break;
		}
	}
	
	WINDOW *up, *down, *up2;

	//Create new serial device
	serialCom *s={};

	//Check if values were already populated by cli args, else set defaults
	if(serialDeviceName.empty()){ serialDeviceName="/dev/serial0"; }
	if(!dataRefreshRate){ dataRefreshRate=100; }

	try{
		s = new serialCom(serialDeviceName.c_str()); //Set serial port to be used here
	}catch(std::exception &e){
		std::cout << e.what() << std::endl;
		return -1;
	}
	

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

	//6*5 white RPM range + 1.5*5 redzone
	mvwprintw(down, 1, 1, std::string( 30, '|').c_str());
	wattron(down, COLOR_PAIR(2));
	wprintw(down, std::string( 1.5*5, '|').c_str());
	wattroff(down, COLOR_PAIR(2));
	wrefresh(up);
	wrefresh(down);

	int stopC=0;
	timeout(500);

	auto *obd = new Obd(s);

	do{
		int rpm = obd->updateRPM(down, up);
//		double maf = obd->getInstantFuel();

//		char temp[12]= {0};
//		snprintf(temp, sizeof(maf), "%.2f", maf);
//		mvwprintw(up2, 2, 2, strcat(temp, " l/100km"));
		mvwprintw(up, 1, 1, std::to_string(rpm).c_str());

		wrefresh(up);
		wrefresh(up2);
		wrefresh(down);
	} while(stopC!=KEY_UP);

	endwin();
	free(up);
	free(down);

}
#pragma clang diagnostic pop
