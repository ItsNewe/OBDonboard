#include "fuelCompCalculator.h"

FuelCompCalculator::FuelCompCalculator(serialCom *s)
{
	this->s = s;
}

double FuelCompCalculator::getInstantFuel()
{
	std::string speedRaw = s->sendMessage("010D\r", 0);
	std::string mafRaw = s->sendMessage("0110\r", 0);

	speedRaw = s->cleanUpSerialFrame(speedRaw);
	mafRaw = s->cleanUpSerialFrame(mafRaw);

	std::string paramA = mafRaw.substr(9, 2);  //Get first data byte
	std::string paramB = mafRaw.substr(11, 2); //Get second data byte
	int speed = stoi(speedRaw.substr(9, 2));

	double maf = (256*std::stoul(paramA, nullptr, 16)+std::stoul(paramB, nullptr, 16))/100;
	double result = (3600*maf)/(9069.90*speed);
	return result;
}