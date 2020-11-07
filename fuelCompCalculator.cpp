#include "fuelCompCalculator.h"

FuelCompCalculator::FuelCompCalculator(serialCom s){
	this->s=s;
}

float FuelCompCalculator::getInstantMaf(){
	std::string rawData = s->sendMessage("0110\r", 0);

    //Clean raw data string
    rawData.erase(std::remove(rawData.begin(), rawData.end(), ' '), rawData.end());
    rawData.erase(std::remove(rawData.begin(), rawData.end(), '\r'), rawData.end());
    rawData.erase(std::remove(rawData.begin(), rawData.end(), '>'), rawData.end());

    std::string paramA = rawData.substr(rawData.length()-4, 2); //Get first data byte
    std::string paramB = rawData.substr(rawData.length()-2, 2); //Get second data byte

    return (256*std::stoul(paramA, nullptr, 16)+std::stoul(paramB, nullptr, 16))/100;
}

int FuelCompCalculator::getVSS(){
	std::string rawData = s->sendMessage("0113\r", 0);

    //Clean raw data string
    rawData.erase(std::remove(rawData.begin(), rawData.end(), ' '), rawData.end());
    rawData.erase(std::remove(rawData.begin(), rawData.end(), '\r'), rawData.end());
    rawData.erase(std::remove(rawData.begin(), rawData.end(), '>'), rawData.end());

	return stoi(rawData.substr(rawData.length()-2, 2)); //Get data byte (last byte)
}