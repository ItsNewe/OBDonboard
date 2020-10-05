//
// Created by Newe on 01/10/2020.
//

#ifndef OBDONBOARD_SERIALCOM_H
#define OBDONBOARD_SERIALCOM_H

#include <cstdio>
#include <fcntl.h> // Contains file controls like O_RDWR
#include <cerrno> // Error integer and strerror() function
#include <termios.h> // Contains POSIX terminal control definitions
#include <unistd.h> // write(), read(), close()
#include <cstring>
#include <string>
#include <vector>
#include <curses.h>

class serialCom {
private:
    int sPort;
    struct termios tty;
public:
    char readBuf[260];
    explicit serialCom(const char *port);
    std::string sendMessage(const char *msg, int responseSize);
    void writeDevice(const char *msg);
    void closeDevice();
};


#endif //OBDONBOARD_SERIALCOM_H
