//
// Created by Newe on 01/10/2020.
//

#include "serialCom.h"

serialCom::serialCom(const char *port) {
	this->sPort = open(port, O_RDWR);
	if (sPort < 0) {
		throw std::runtime_error(fmt::format("Error {} while connecting to serial: {}", errno, strerror(errno)));
	}

	// Read in existing settings, and handle any error
	// NOTE: This is important! POSIX states that the struct passed to tcsetattr()
	// must have been initialized with a call to tcgetattr() overwise behaviour
	// is undefined
	if (tcgetattr(sPort, &tty) != 0) {
		throw std::runtime_error(fmt::format("Error %i from tcgetattr: %s\n", errno, strerror(errno)));
	}

	this->tty.c_cflag &= ~PARENB; //No parity
	this->tty.c_cflag &= ~CSTOPB; //1 stop bit
	this->tty.c_cflag |= CS8; //8 bits per byte
	this->tty.c_cflag &= ~CRTSCTS; //Disable flow control
	this->tty.c_cflag |= CREAD | CLOCAL; // Turn on READ & ignore ctrl lines (CLOCAL = 1)
	this->tty.c_lflag &= ~ICANON; //Disable canonical mode

	this->tty.c_lflag &= ~ECHO; // Disable echo
	this->tty.c_lflag &= ~ECHOE; // Disable erasure
	this->tty.c_lflag &= ~ECHONL; // Disable new-line echo
	this->tty.c_lflag &= ~ISIG; // Disable interpretation of INTR, QUIT and SUSP

	this->tty.c_iflag &= ~(IXON | IXOFF | IXANY); // Turn off s/w flow ctrl
	this->tty.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | ICRNL | IGNCR); // Disable any special handling of received bytes, receive raw data

	this->tty.c_oflag &= ~OPOST; // Prevent special interpretation of output bytes
	this->tty.c_oflag &= ~ONLCR; // Prevent conversion of newline to carriage RL feed

	this->tty.c_cc[VTIME] = 20;  // Wait for up to 1s (10 deciseconds), returning as soon as data is received.
	this->tty.c_cc[VMIN] = 0;

	// Set in/out baud rate to be 9600
	cfsetispeed(&this->tty, B9600);
	cfsetospeed(&this->tty, B9600);

	//Save termios settings
	if (tcsetattr(sPort, TCSANOW, &tty) != 0) {
		printf("Error %i from tcsetattr: %s\n", errno, strerror(errno));
	}
	
	sendMessage("ATE0\r", 0); //Disable echo
	sendMessage("ATH0\r", 0); //Disable headers
	sendMessage("ATS0\r", 0); //No spaces
	sendMessage("ATAL\r", 0); //Allow long messages
	sendMessage("ATTP0\r", 0); //Detect best OBD protocol
}

void serialCom::writeDevice(const char *msg) {
	write(this->sPort, msg, sizeof(msg));
}

std::string serialCom::sendMessage(const char *msg, int responseSize) {
	memset(&this->readBuf, 0, sizeof(this->readBuf)); //Zero out buffer

	write(this->sPort, msg, sizeof(msg)+1);
	int n = read(this->sPort, &this->readBuf, sizeof(readBuf));


	return std::string(readBuf).substr(0, n); //Convert char array to string
}

void serialCom::closeDevice() {
	close(this->sPort);
	free(&this->tty);
}

std::string serialCom::cleanUpSerialFrame(std::string data){
	//Clean raw data string
	for (auto i = data.cbegin(); i!=data.cend(); i++){
		if(*i==' '){
			data.erase(i);
		}
	}

	//rawData.erase(std::remove(rawData.begin(), rawData.end(), ' '), rawData.end());
	return data;
}
