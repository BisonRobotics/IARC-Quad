#include <iostream>
#include <mutex>
#include <vector>
#include <unistd.h>
#include <sys/time.h>

#include "RS232_GenericController.h"
#include "PixhawkCommands.hpp"

using namespace std;

// mavlink libraries
#include <common/mavlink.h>

#define USE_PIXHAWK
//#define USE_PX4
//#define USE_ARDUPILOT

#ifdef USE_PIXHAWK
#define ARG_INDEX_PIXHAWK 1
#endif // USE_PIXHAWK

#ifdef USE_PX4
#define ARG_INDEX_PX4 1
#endif // USE_PX4

#ifdef USE_ARDUPILOT
#define ARG_INDEX_ARDUPILOT 1
#endif // USE_ARDUPILOT

bool printAllMessageID = false;
vector<uint8_t> PixHawkTypes = {
    MAVLINK_MSG_ID_HEARTBEAT,
    MAVLINK_MSG_ID_SYS_STATUS,
    MAVLINK_MSG_ID_SCALED_PRESSURE,
    MAVLINK_MSG_ID_RAW_IMU,
    MAVLINK_MSG_ID_SCALED_IMU2,
    MAVLINK_MSG_ID_PARAM_VALUE,
    MAVLINK_MSG_ID_STATUSTEXT,
    MAVLINK_MSG_ID_COMMAND_ACK,
    MAVLINK_MSG_ID_POSITION_TARGET_LOCAL_NED,
};

vector<uint16_t> ArduPilotTypes = {
    MAVLINK_MSG_ID_GLOBAL_POSITION_INT,
    //MAVLINK_MSG_ID_OPTICAL_FLOW,
    //MAVLINK_MSG_ID_COMMAND_ACK,
};

uint64_t get_usec_timestamp(void) {
    static struct timeval time_stamp;
    gettimeofday(&time_stamp, NULL);
    return ((time_stamp.tv_sec * 1000000) + time_stamp.tv_usec);
}

int main(int argc, char* argv[]) {
    if(argc < 2) {
        throw new runtime_error("serial port not specified");
    }

#ifdef USE_PIXHAWK
    // setup a serial port connection for PixHawk
    SerialController sc_pixhawk(argv[ARG_INDEX_PIXHAWK]);
    sc_pixhawk.set_BaudRate(B57600);
    sc_pixhawk.set_WordSize(SC::WORDSIZE::_8);
    sc_pixhawk.set_Parity(  SC::PARITY::OFF);
    sc_pixhawk.set_StopBits(SC::STOPBITS::_1);
    sc_pixhawk.start();

    PixHawkHelp::pixhawkController = &sc_pixhawk;
#endif // USE_PIXHAWK

#ifdef USE_PX4
    // setup a serial port connection for PX4
    SerialController sc_px4(argv[ARG_INDEX_PX4]);
    sc_px4.set_BaudRate(B115200);
    sc_px4.set_WordSize(SC::WORDSIZE::_8);
    sc_px4.set_Parity(  SC::PARITY::OFF);
    sc_px4.set_StopBits(SC::STOPBITS::_1);
    sc_px4.start();
#endif // USE_PX4

#ifdef USE_ARDUPILOT
    // setup a serial port connection for ArdoPilot
    SerialController sc_ardupilot(argv[ARG_INDEX_ARDUPILOT]);
    sc_ardupilot.set_BaudRate(B57600);
    sc_ardupilot.set_WordSize(SC::WORDSIZE::_8);
    sc_ardupilot.set_Parity(  SC::PARITY::OFF);
    sc_ardupilot.set_StopBits(SC::STOPBITS::_1);
    sc_ardupilot.start();
#endif // USE_ARDUPILOT

    // MavLink message stuff
    mavlink_message_t message;
    mavlink_status_t  status;

//#ifdef USE_PIXHAWK
    int system_type_pixhawk    = MAV_TYPE_QUADROTOR;
    int autopilot_type_pixhawk = MAV_AUTOPILOT_GENERIC;
    int system_id_pixhawk    = 2;
    int component_id_pixhawk = 1;
//#endif // USE_PIXHAWK

    // base computer identifying information
    int system_id_cpu    = 1;
    int component_id_cpu = 1;

    int state_delay_time = 1000000;

    // implement a simple state machine
    const int qSTATE_None               = 0;
    const int qSTATE_Arm                = 1;
    const int qSTATE_Disarm             = 4;
    const int qSTATE_SetLocalNED        = 6;
    const int qSTATE_TakeoffLocal       = 7;
    const int qSTATE_DoSetMode          = 8;

    // starting state
    int QUAD_STATE = qSTATE_Arm;

    // start looping for ardupilot
#ifdef USE_ARDUPILOT
    PixHawkHelp::pixhawkController = &sc_ardupilot;
    //PixHawkCmd::SetPositionTargetLocalNED(system_id_pixhawk, component_id_pixhawk);
    PixHawkCmd::Arm(system_id_pixhawk, component_id_pixhawk, system_id_cpu, component_id_cpu);
    PixHawkHelp::waitForCmdAck(true, false);

    for(;;) { }

    uint64_t last_time, current_time, tallied_time = 0;
    uint64_t time_to_land = 0;
    current_time = get_usec_timestamp();
    last_time = current_time;

    bool doNED = true;

    for(;;) {
        current_time = get_usec_timestamp();
        tallied_time += (current_time - last_time);
        time_to_land += (current_time - last_time);
        last_time = current_time;

        if(tallied_time > 200000 && doNED) { // 5x/sec
            PixHawkCmd::SetPositionTargetLocalNED(system_id_pixhawk, component_id_pixhawk);
            //PixHawkCmd::DoChangeSpeed(system_id_pixhawk, component_id_pixhawk, system_id_cpu, component_id_cpu);
            //PixHawkCmd::TakeoffLocal(system_id_pixhawk, component_id_pixhawk, system_id_cpu, component_id_cpu);
            tallied_time -= 200000;
        }

        if(time_to_land > 10000000) {
            PixHawkCmd::Disarm(system_id_pixhawk, component_id_pixhawk, system_id_cpu, component_id_cpu);
            PixHawkHelp::waitForCmdAck(true, false);
            doNED = false;
            time_to_land -= 200000;
        }

        if(MavMsgUx::checkMessagesOnPort(&sc_ardupilot, &message)) {
            for(unsigned int i = 0; i < ArduPilotTypes.size(); i++) {
                if(ArduPilotTypes[i] == message.msgid)
                    MavMsgUx::printMessageContents(message);
            }
        }

        //usleep(200000);
    }
#endif // USE_ARDUPILOT

    for(;;) {
        static uint8_t mavlink_buffer[MAVLINK_MAX_PACKET_LEN];

        if(QUAD_STATE == qSTATE_Arm) {
            PixHawkCmd::Arm(system_id_pixhawk, component_id_pixhawk, system_id_cpu, component_id_cpu);
            if(PixHawkHelp::waitForCmdAck(true, false))
                QUAD_STATE = qSTATE_DoSetMode;
            usleep(state_delay_time);
        }

        if(QUAD_STATE == qSTATE_DoSetMode) {
            PixHawkCmd::DoSetMode(system_id_pixhawk, component_id_pixhawk, system_id_cpu, component_id_cpu);
            PixHawkHelp::waitForCmdAck(true, false);

            QUAD_STATE = qSTATE_None;
        }

        if(QUAD_STATE == qSTATE_Disarm) {
            PixHawkCmd::Disarm(system_id_pixhawk, component_id_pixhawk, system_id_cpu, component_id_cpu);

            if(PixHawkHelp::waitForCmdAck(true, false))
                QUAD_STATE = qSTATE_Arm;
            usleep(state_delay_time);
        }

        if(QUAD_STATE == qSTATE_TakeoffLocal) {
            // enable external control
            PixHawkCmd::NavGuidedEnable(system_id_pixhawk, component_id_pixhawk, system_id_cpu, component_id_cpu);
            PixHawkHelp::waitForCmdAck(true, false);

            //PixHawkCmd::TakeoffLocal(system_id_pixhawk, component_id_pixhawk, system_id_cpu, component_id_cpu);
            PixHawkHelp::waitForCmdAck(true, false);

            QUAD_STATE = qSTATE_None;
        }

        if(QUAD_STATE == qSTATE_SetLocalNED) {
            // enable external control
            PixHawkCmd::NavGuidedEnable(system_id_pixhawk, component_id_pixhawk, system_id_cpu, component_id_cpu);
            PixHawkHelp::waitForCmdAck(true, false);

            PixHawkCmd::SetPositionTargetLocalNED(system_id_pixhawk, component_id_pixhawk);
            PixHawkHelp::waitForCmdAck(true, false);
                QUAD_STATE = qSTATE_None;
            //usleep(state_delay_time);
        }

        #ifdef USE_PX4
        if(checkMessagesOnPort(&sc_px4, &message)) {
//            if(message.msgid == MAVLINK_MSG_ID_OPTICAL_FLOW || message.msgid == MAVLINK_MSG_ID_DEBUG_VECT)
                printMessageContents(message);
        }
        #endif // USE_PX4

        #ifdef USE_PIXHAWK
        while(MavMsgUx::checkMessagesOnPort(&sc_pixhawk, &message)) { // print every available message
            if(printAllMessageID) {
                cout << "Message ID: " << (int)message.msgid << endl;
            } else {
                for(unsigned int i = 0; i < PixHawkTypes.size(); i++) {
                    if(PixHawkTypes[i] == message.msgid)
                        MavMsgUx::printMessageContents(message);
                }
            }
        }
        #endif // USE_PIXHAWK
    }
}













