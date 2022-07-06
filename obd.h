#include <curses.h>
#include <cstring>
#include <string>
#include <chrono>
#include <thread>
#include "serialCom.h"

class Obd{
public:
	Obd(serialCom *s);

	//RPM
	int updateRPM(WINDOW *down, WINDOW *up);
	int currentRPM=0;
	int restoreRPM=0;

	//Fuel
	double getInstantFuel();
	int getVSS();
	
private:
	serialCom *s={};

	//Fuel
	unsigned int maf;
};