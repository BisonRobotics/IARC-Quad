#include "RS232_GenericController.h"

#include <sys/ioctl.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <iostream>
#include <cstdlib>

//#define SC SerialController

SC::SerialController(void) {
    ;
}

SC::SerialController(const char* serialPort) {
    SerialController::set_SerialPort(serialPort);
}

void SC::set_SerialPort(const char* serialPort) {
    memset(&tty, 0, sizeof tty);
    fd = open(serialPort, O_RDWR | O_NOCTTY | O_NDELAY); // pretty standard flags
    if(fd < 0) {
        int e = errno;
        std::cerr << "Error opening file" << std::endl;
        std::cerr << "    Error: " << strerror(e) << std::endl;
        exit(-1);
    }

    if(tcgetattr(fd, &tty) < 0) {
        int e = errno;
        std::cerr << "Error retrieving attributes" << std::endl;
        std::cerr << "    Error: " << strerror(e) << std::endl;
        exit(-1);
    }

    serialPortSet = true;
}

void SC::parityEnable(void) {
    tty.c_cflag |= PARENB;
}

void SC::writeBuffer(char* buffer, int bufSize) {
    write(fd, buffer, bufSize);
}

void SC::readBuffer(char* buffer, int bufSize) {
    read(fd, buffer, bufSize);
}

void SC::set_BaudRate(int baudrate) {
    cfsetispeed(&tty, baudrate);
    cfsetospeed(&tty, baudrate);
}

void SC::set_Parity(SC::PARITY parity) {
    switch(parity) {
        case EVEN:
            tty.c_cflag |= PARENB;
            tty.c_cflag &= ~PARODD;
            break;
        case ODD:
            tty.c_cflag |= PARENB;
            tty.c_cflag |= PARODD;
            break;
        case OFF: // disable the parity bit
            tty.c_cflag &= ~PARENB;
            break;
        default:
            std::cerr << "Invalid parity argument" << std::endl;
            exit(-1); // all error comditions return -1
    }
}

void SC::set_StopBits(SC::STOPBITS stopbits) {
    switch(stopbits) {
        case _1:
            tty.c_cflag &= ~CSTOPB;
            break;
        case _2:
            tty.c_cflag |= CSTOPB;
            break;
        default:
            std::cerr << "Invalid stop bit argument" << std::endl;
            exit(-1);
    }
}

void SC::set_WordSize(SC::WORDSIZE wordsize) {
    switch(wordsize) {
        case _7:
            tty.c_cflag &= ~CSIZE;
            tty.c_cflag |= CS7;
            break;
        case _8:
            tty.c_cflag &= ~CSIZE;
            tty.c_cflag |= CS8;
            break;
        default:
            std::cerr << "Invalid word size argument" << std::endl;
            exit(-1);
    }
}

void SerialController::start(void) {
    if(serialPortSet == false) {
        std::cerr << "Serial port has not been opened" << std::endl;
        exit(-1);
    }

    if(tcsetattr(fd, TCSANOW, &tty) != 0) {
        std::cerr << "Error starting serial communications" << std::endl;
        std::cerr << "    Error code: " << errno << std::endl;
        exit(-1);
    }
}

bool SerialController::set_ModeBlocking(void) {
    if(!serialPortSet) {
        std::cerr << "Error: serial port not started" << std::endl;
        exit(-1);
    }

    int sFlags = fcntl(fd, F_GETFL, 0);

    if(sFlags == -1)
        return false;

    sFlags &= ~O_NONBLOCK;

    if(fcntl(fd, F_SETFL, sFlags) == -1)
        return false;

    // return success
    return true;
}

bool SerialController::set_ModeNonblocking(void) {
    int sFlags = fcntl(fd, F_GETFL, 0);

    if(sFlags == -1)
        return false;

    sFlags |= O_NONBLOCK;

    if(fcntl(fd, F_SETFL, sFlags) == -1)
        return false;

    // return success
    return true;
}

int SerialController::hasData(void) {
    int bAvail;
    ioctl(fd, FIONREAD, &bAvail);
    return bAvail;
}

int SerialController::get_FileDescriptor(void) {
    return fd;
}





















