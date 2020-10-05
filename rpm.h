#include <curses.h>
#include <cstring>
#include <string>
#include <algorithm>
#include <climits>
#include "serialCom.h"


class Rpm {
public:
	Rpm();
    void updateRPM(WINDOW *down, WINDOW *up, serialCom *s);
private:
    unsigned int currentRPM=0;
    unsigned int restoreRPM=0;
};