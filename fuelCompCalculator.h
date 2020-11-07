#include "serialCom.h"
#include <cstring>

class FuelCompCalculator {
public:
    FuelCompCalculator(serialCom s);
	float getInstantMaf();
	int getVSS();
private:
	serialCom s;
	unsigned int maf;
};