#include <curses.h>
#include <cstring>
#include <string>
#include "serialCom.h"

class Rpm{
public:
	Rpm();
    void updateRPM(WINDOW *down, WINDOW *up, serialCom *s);
private:
    int currentRPM=0;
    int restoreRPM=0;
};