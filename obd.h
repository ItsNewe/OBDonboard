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
    void updateRPM(WINDOW *down, WINDOW *up);

    //Fuel
    double getInstantFuel();
	int getVSS();
private:
    serialCom *s={};

    //RPM
    int currentRPM=0;
    int restoreRPM=0;

    //Fuel
	unsigned int maf;
};