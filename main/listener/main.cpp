#include <iostream>
#include <mutex>
#include <vector>
#include <unistd.h>
#include <sys/time.h>

#include "../RS232_GenericController.h"
#include "../PixhawkCommands.hpp"
#include "../TimeClass.h"

using namespace std;

mavlink_message_t message_gps_raw_int;
mavlink_message_t message_highres_imu;

int main(int argc, char* argv[]) {
    SerialController sc(argv[1]);
    sc.set_BaudRate(B57600);
    sc.set_WordSize(SC::WORDSIZE::_8);
    sc.set_Parity(  SC::PARITY::OFF);
    sc.set_StopBits(SC::STOPBITS::_1);
    sc.start();

    mavlink_message_t message;
    TimeClass tc;

    double deltaTime = 0.0;

    for(;;) {
        if(MavMsgUx::checkMessagesOnPort(&sc, &message)) {
            if(message.msgid == MAVLINK_MSG_ID_GPS_RAW_INT) {
                message_gps_raw_int = message;
            } else if(message.msgid == MAVLINK_MSG_ID_HIGHRES_IMU) {
                message_highres_imu = message;
            }
        }

        deltaTime += tc.getElapsedSecondsUpdate();

        if(deltaTime > 0.333) {
            system("clear");
            MavMsgUx::printMessageContents(message_gps_raw_int);
            MavMsgUx::printMessageContents(message_highres_imu);
            deltaTime -= 0.333;
        }
    }

    return 0;

}
