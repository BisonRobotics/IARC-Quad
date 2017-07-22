#ifndef __JJC__PIXHAWKCOMMANDS__HPP__
#define __JJC__PIXHAWKCOMMANDS__HPP__

#include "RS232_GenericController.h"   // SerialController functions
#include "MavlinkMessageInterface.hpp" // MavMsgUx:: functions
#include <common/mavlink.h>

namespace PixHawkHelp {

bool printCommandName = true;

SerialController* pixhawkController = 0; // quick error check for serial terminal

// true: command successful, false: command not successful
bool waitForCmdAck(bool echoMsgContents = true, bool echoAllMsgs = true) {
    static mavlink_message_t message;
    bool retVal = false; //
    for(;;) {
        if(MavMsgUx::checkMessagesOnPort(pixhawkController, &message)) {
            if(message.msgid == MAVLINK_MSG_ID_COMMAND_ACK) {
                mavlink_command_ack_t commandAckMessage;
                mavlink_msg_command_ack_decode(&message, &commandAckMessage);

                // 0 indicates successful execution of previous command
                if(commandAckMessage.result == 0)
                    retVal = true;

                if(echoMsgContents)
                    MavMsgUx::printMessageContents(message);

                return retVal;
            } else {
                if(echoAllMsgs)
                    MavMsgUx::printMessageContents(message);
            }
        }
    }
}

mavlink_command_long_t packCommand(
        int system_id_pixhawk,
        int component_id_pixhawk,
        uint16_t mavlink_command) {

    mavlink_command_long_t com = {0};
    com.target_system    = system_id_pixhawk;
    com.target_component = component_id_pixhawk;
    com.command          = mavlink_command;
    com.confirmation     = true;

    return com;
}

void sendCommand(
        mavlink_command_long_t* com,
        int system_id_cpu,
        int component_id_cpu) {

    mavlink_message_t message;
    static uint8_t mavlink_buffer[MAVLINK_MAX_PACKET_LEN];

    mavlink_msg_command_long_encode(system_id_cpu, component_id_cpu, &message, com);
    uint16_t len = mavlink_msg_to_send_buffer(mavlink_buffer, &message);

    // quick error check
    if(pixhawkController != 0)
        pixhawkController->writeBuffer((char*)mavlink_buffer, len);
}

void printCommand(string commandName) {
    if(printCommandName) {
        cout << "==============================================" << endl;
        cout << commandName << endl;
        cout << "==============================================" << endl << endl;
    }
}

} // end of namespace PixHawkHelp

namespace PixHawkCmd {

void Arm(
        int system_id_pixhawk,
        int component_id_pixhawk,
        int system_id_cpu,
        int component_id_cpu) {

    PixHawkHelp::printCommand("ARM");

    mavlink_command_long_t com = {0};
    com.target_system    = system_id_pixhawk;
    com.target_component = component_id_pixhawk;
    com.command          = MAV_CMD_COMPONENT_ARM_DISARM;
    com.confirmation     = true;

    com.param1 = (int)true; // arm

    PixHawkHelp::sendCommand(&com, system_id_cpu, component_id_cpu);
}

void Disarm(
        int system_id_pixhawk,
        int component_id_pixhawk,
        int system_id_cpu,
        int component_id_cpu) {

    PixHawkHelp::printCommand("DISARM");

    mavlink_command_long_t com = {0};
    com.target_system    = system_id_pixhawk;
    com.target_component = component_id_pixhawk;
    com.command          = MAV_CMD_COMPONENT_ARM_DISARM;
    com.confirmation     = true;

    com.param1 = (int)false; // disarm

    PixHawkHelp::sendCommand(&com, system_id_cpu, component_id_cpu);
}

void SetPositionTargetLocalNED(
        int system_id_pixhawk,
        int component_id_pixhawk) {

    PixHawkHelp::printCommand("SET POSITION TARGET LOCAL NED");

    mavlink_set_position_target_local_ned_t sp;
    sp.type_mask = 0x3000;
    sp.coordinate_frame = MAV_FRAME_LOCAL_NED;

    sp.x = 0.0;
    sp.y = 0.0;
    sp.z = -1.0;

    sp.vx = 0.0;
    sp.vy = 0.0;
    sp.vz = 0.0;

    sp.afx = 0.0;
    sp.afy = 0.0;
    sp.afz = 0.0;

    uint8_t mavlink_buffer[MAVLINK_MAX_PACKET_LEN];

    mavlink_message_t message;
    mavlink_msg_set_position_target_local_ned_encode(system_id_pixhawk, component_id_pixhawk, &message, &sp);
    uint16_t len = mavlink_msg_to_send_buffer(mavlink_buffer, &message);

    // quick error check
    if(PixHawkHelp::pixhawkController != 0)
        PixHawkHelp::pixhawkController->writeBuffer((char*)mavlink_buffer, len);
}

void DoChangeSpeed(
        int system_id_pixhawk,
        int component_id_pixhawk,
        int system_id_cpu,
        int component_id_cpu) {

    PixHawkHelp::printCommand("DO CHANGE SPEED");

    mavlink_command_long_t com = {0};
    com.target_system    = system_id_pixhawk;
    com.target_component = component_id_pixhawk;
    com.command          = MAV_CMD_DO_CHANGE_SPEED;
    com.confirmation     = true;

    com.param1 = 0; // airspeed, not ground speed
    com.param2 = -1; // dont change speed
    com.param3 = 100.0f; // throttle percentage
    com.param4 = 0.0; // absolute

    PixHawkHelp::sendCommand(&com, system_id_cpu, component_id_cpu);
}

void DoSetMode(
        int system_id_pixhawk,
        int component_id_pixhawk,
        int system_id_cpu,
        int component_id_cpu) {

    PixHawkHelp::printCommand("DO SET MODE");

    mavlink_command_long_t com = PixHawkHelp::packCommand(
            system_id_pixhawk, component_id_pixhawk,
            MAV_CMD_DO_SET_MODE);

    com.param1 = MAV_MODE_FLAG_SAFETY_ARMED;

    PixHawkHelp::sendCommand(&com, system_id_cpu, component_id_cpu);
}

/*
void TakeoffLocal(
        int system_id_pixhawk,
        int component_id_pixhawk,
        int system_id_cpu,
        int component_id_cpu) {

    PixHawkHelp::printCommand("TAKEOFF LOCAL");

    mavlink_command_long_t com = PixHawkHelp::packCommand(
            system_id_pixhawk, component_id_pixhawk,
            MAV_CMD_NAV_TAKEOFF_LOCAL);

    com.param1 = 0.0f; // pitch angle
    // param2 is empty
    com.param3 = 0.2f; // takeoff ascend rate
    com.param4 = 0.0f; // yaw angle
    com.param5 = 0.0f; // y-axis position
    com.param6 = 0.0f; // x-axis position
    com.param7 = -2.0f; // z-axis position

    PixHawkHelp::sendCommand(&com, system_id_cpu, component_id_cpu);
}
*/

void NavGuidedEnable(
        int system_id_pixhawk,
        int component_id_pixhawk,
        int system_id_cpu,
        int component_id_cpu) {

    PixHawkHelp::printCommand("NAV GUIDED ENABLE");

    mavlink_command_long_t com = PixHawkHelp::packCommand(
            system_id_pixhawk, component_id_pixhawk,
            MAV_CMD_NAV_GUIDED_ENABLE);

    com.param1 = (int)true; // enable

    PixHawkHelp::sendCommand(&com, system_id_cpu, component_id_cpu);
}

/*
void DoMotorTest(
        int system_id_pixhawk,
        int component_id_pixhawk,
        int system_id_cpu,
        int component_id_cpu,
        int motor_number,
        float throttle,
        float timeout) {

    mavlink_command_long_t com = PixHawkHelp::packCommand(
            system_id_pixhawk, component_id_pixhawk,
            MAV_CMD_DO_MOTOR_TEST);

    com.param1 = motor_number;
    com.param2 = 1; // pwm
    com.param3 = throttle;
    com.param4 = timeout;

    PixHawkHelp::sendCommand(&com, system_id_cpu, component_id_cpu);

}
*/

} // end of namespace PixHawkCmd

#endif // __JJC__PIXHAWKCOMMANDS__HPP__
