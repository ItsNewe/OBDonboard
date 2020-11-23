#include "serialCom.h"
#include <cstring>

class FuelCompCalculator {
public:
    FuelCompCalculator(serialCom *s);
	double getInstantFuel();
	int getVSS();
private:
	serialCom *s;
	unsigned int maf;
};