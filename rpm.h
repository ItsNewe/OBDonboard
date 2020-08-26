#include <curses.h>
#include <cstring>
#include <string>

class Rpm {
public:
    void updateRPM(WINDOW *down, WINDOW *up);
private:
    int currentRPM=0;
    int restoreRPM=0;
};