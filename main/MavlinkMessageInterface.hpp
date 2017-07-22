#ifndef __JJC__MAVLINK__MESSAGE__INTERFACE__HPP__
#define __JJC__MAVLINK__MESSAGE__INTERFACE__HPP__

#include "RS232_GenericController.h"
#include <common/mavlink.h>
#include <iostream>

using namespace std;

namespace MavMsgUx {

bool checkMessagesOnPort(SerialController* sc, mavlink_message_t* message) {
    mavlink_status_t status;

    if(sc == 0)
        cout << "INVALID SERIAL PORT" << endl;

    if(sc->hasData() > 0) {
        char c;
        do {
            while(sc->hasData() == 0) { ; } // wait for the next character
            sc->readBuffer(&c, 1);
        } while(mavlink_parse_char(MAVLINK_COMM_0, c, message, &status) == 0);

        return true;
    }

    return false;
}

void printMessageContents(mavlink_message_t& message) {
    switch(message.msgid) {
        case MAVLINK_MSG_ID_HEARTBEAT: // 0
            {
                mavlink_heartbeat_t hbMessage;
                mavlink_msg_heartbeat_decode(&message, &hbMessage);
                cout << "Heartbeat Message received:" << endl;

                cout << "\tVersion: " << (int)hbMessage.mavlink_version << endl;
                cout << "\tStatus: ";
                switch(hbMessage.system_status) {
                    case MAV_STATE_UNINIT:
                        cout << "UNINIT" << endl; break;
                    case MAV_STATE_BOOT:
                        cout << "BOOT" << endl; break;
                    case MAV_STATE_CALIBRATING:
                        cout << "CALIBRATING" << endl; break;
                    case MAV_STATE_STANDBY:
                        cout << "STANDBY" << endl; break;
                    case MAV_STATE_ACTIVE:
                        cout << "ACTIVE" << endl; break;
                    case MAV_STATE_CRITICAL:
                        cout << "CRITICAL" << endl; break;
                    case MAV_STATE_EMERGENCY:
                        cout << "EMERGENCY" << endl; break;
                    case MAV_STATE_POWEROFF:
                        cout << "POWEROFF" << endl; break;
                    default:
                        cout << "unknown" << endl;
                        break;
                }
                cout << endl;
            }
            break;
        case MAVLINK_MSG_ID_SYS_STATUS: // 1
            {
                mavlink_sys_status_t sysStatusMessage;
                mavlink_msg_sys_status_decode(&message, &sysStatusMessage);
                cout << "Sys Status received" << endl;

                cout << "\tSystem load: " << float(sysStatusMessage.load) / 10.0 << "% " << endl;

                cout << endl;
            }
            break;
        case MAVLINK_MSG_ID_PARAM_VALUE: // 22
            {
                mavlink_param_value_t paramValueMessage;
                mavlink_msg_param_value_decode(&message, &paramValueMessage);
                cout << "Param Value Message received" << endl;

                char paramid[17] = {0};
                for(int i = 0; i < 16; i++)
                    paramid[i] = paramValueMessage.param_id[i];

                cout << "\tParam ID: " << paramid << endl; // definitely null-terminated
                cout << "\tParam Value: " << paramValueMessage.param_value << endl;

                cout << endl;
            }
            break;
        case MAVLINK_MSG_ID_RAW_IMU: // 27
            {
                mavlink_raw_imu_t rawImuMessage;
                mavlink_msg_raw_imu_decode(&message, &rawImuMessage);
                cout << "IMU Message received" << endl;

                cout << "\tTimestamp: " << rawImuMessage.time_usec << endl;
                cout << "\tAcceleration" << endl;
                cout << "\t\tx: " << rawImuMessage.xacc << endl;
                cout << "\t\ty: " << rawImuMessage.yacc << endl;
                cout << "\t\tz: " << rawImuMessage.zacc << endl;

                cout << endl;
            }
            break;
        case MAVLINK_MSG_ID_SCALED_PRESSURE: // 29
            {
                mavlink_scaled_pressure_t scaledPressureMessage;
                mavlink_msg_scaled_pressure_decode(&message, &scaledPressureMessage);
                cout << "Scaled Pressure Message received" << endl;

                cout << "\tBoot timestamp: " << scaledPressureMessage.time_boot_ms << endl;
                cout << "\tAbsolute pressure: " << double(scaledPressureMessage.press_abs) / 100.0 << endl;
                cout << "\tTemperature: " << double(scaledPressureMessage.temperature) / 100 << endl;

                cout << endl;

            }
            break;
        case MAVLINK_MSG_ID_GLOBAL_POSITION_INT:
            {
                mavlink_global_position_int_t globalPositionIntMessage;
                mavlink_msg_global_position_int_decode(&message, &globalPositionIntMessage);
                cout << "Global Position Int Message received" << endl;

                cout << "\tLocation: " << endl;
                cout << "\t\tlat: " << double(globalPositionIntMessage.lat) / 10000000.0 << endl;
                cout << "\t\tlon: " << double(globalPositionIntMessage.lon) / 10000000.0 << endl;
                cout << "\t\talt: " << double(globalPositionIntMessage.alt) / 1000.0 << "m" << endl;

                cout << endl;
            }
            break;
        case MAVLINK_MSG_ID_COMMAND_ACK: // 77
            {
                mavlink_command_ack_t commandAckMessage;
                mavlink_msg_command_ack_decode(&message, &commandAckMessage);
                cout << "Command Ack Message received" << endl;

                cout << "\tCmd: ";
                switch(commandAckMessage.command) {
                    case MAV_CMD_COMPONENT_ARM_DISARM:
                        cout << "ARM_DISARM" << endl;
                        break;
                    case MAV_CMD_DO_CHANGE_SPEED:
                        cout << "DO_CHANGE_SPEED" << endl;
                        break;
                    case MAV_CMD_NAV_GUIDED_ENABLE:
                        cout << "NAV_GUIDED_ENABLE" << endl;
                        break;
                    default:
                        cout << (int)commandAckMessage.command << endl;
                        break;
                }

                cout << "\tResult: ";
                switch(commandAckMessage.result) {
                    case 0:
                        cout << "ACCEPTED" << endl;
                        break;
                    case 1:
                        cout << "TEMPORARILY DENIED/REJECTED" << endl;
                        break;
                    case 2:
                        cout << "DENIED" << endl;
                        break;
                    case 3:
                        cout << "UNSUPPORTED" << endl;
                        break;
                    case 4:
                        cout << "FAILED" << endl;
                        break;
                    case 5:
                        cout << "IN PROGRESS" << endl;
                        break;
                    default:
                        cout << "OTHER: " << (int)commandAckMessage.result << endl;
                        break;
                }

                cout << endl;
            }
            break;
        case MAVLINK_MSG_ID_POSITION_TARGET_LOCAL_NED: // 84
            {
                mavlink_position_target_local_ned_t ptlnMessage;
                mavlink_msg_position_target_local_ned_decode(&message, &ptlnMessage);
                cout << "Position Target Local NED Message received" << endl;

                cout << endl;
            }
            break;
        case MAVLINK_MSG_ID_OPTICAL_FLOW: // 100
            {
                mavlink_optical_flow_t opticalFlowMessage;
                mavlink_msg_optical_flow_decode(&message, &opticalFlowMessage);
                cout << "Optical Flow Message received" << endl;

                // maintain a running tally of pixel movements
                static int dx = 0;
                static int dy = 0;

                dx += opticalFlowMessage.flow_x / 10;
                dy += opticalFlowMessage.flow_y / 10;

                cout << "\tTimestamp: " << opticalFlowMessage.time_usec << endl;
                cout << "\tSensor ID: " << opticalFlowMessage.sensor_id << endl;
                cout << "\tPixel flow:\n";
                cout << "\t\tx: " << opticalFlowMessage.flow_x / 10 << endl;
                cout << "\t\ty: " << opticalFlowMessage.flow_y / 10 << endl;
                cout << "\tFlow quality: " << (int)opticalFlowMessage.quality << endl;
                cout << "\tDistance to ground: " << opticalFlowMessage.ground_distance << endl;
                cout << "\tTotal distance traveled:\n";
                cout << "\t\tx: " << float(dx) / 10.0f << endl;
                cout << "\t\ty: " << float(dy) / 10.0f << endl;

                cout << endl;
            }
            break;
        case MAVLINK_MSG_ID_SCALED_IMU2: // 116
            {
                mavlink_scaled_imu2_t scaledImu2Message;
                mavlink_msg_scaled_imu2_decode(&message, &scaledImu2Message);
                cout << "Scaled IMU2 Message received" << endl;

                cout << "\tBoot timestamp: " << scaledImu2Message.time_boot_ms << endl;
                cout << "\tAcceleration:\n";
                cout << "\t\tx: " << scaledImu2Message.xacc << endl;
                cout << "\t\ty: " << scaledImu2Message.yacc << endl;
                cout << "\t\tz: " << scaledImu2Message.zacc << endl;
                cout << "\tMagnetic Field:\n";
                cout << "\t\tx: " << scaledImu2Message.xmag << endl;
                cout << "\t\ty: " << scaledImu2Message.ymag << endl;
                cout << "\t\tz: " << scaledImu2Message.zmag << endl;

                cout << endl;
            }
            break;
        case MAVLINK_MSG_ID_POWER_STATUS: // 125
            {
                mavlink_power_status_t powerStatusMessage;
                mavlink_msg_power_status_decode(&message, &powerStatusMessage);
                cout << "Power Status Message received" << endl;

                cout << "\t5v rail: " << float(powerStatusMessage.Vcc) / 1000.0 << endl;
                cout << "\tServo rail voltage: " << float(powerStatusMessage.Vservo) / 1000.0 << endl;

                cout << endl;

            }
            break;
        case MAVLINK_MSG_ID_DEBUG_VECT: // 250
            {
                mavlink_debug_vect_t debugVectMessage;
                mavlink_msg_debug_vect_decode(&message, &debugVectMessage);
                cout << "Debug Vect Message received" << endl;

                cout << "\tTimestamp: " << debugVectMessage.time_usec << endl;
                cout << "\tName: " << debugVectMessage.name << endl;
                cout << "\tDebug vector:\n";
                cout << "\t\tx: " << debugVectMessage.x << endl;
                cout << "\t\ty: " << debugVectMessage.y << endl;
                cout << "\t\tz: " << debugVectMessage.z << endl;

                cout << endl;
            }
            break;
        case MAVLINK_MSG_ID_STATUSTEXT: // 253
            {
                mavlink_statustext_t statusTextMessage;
                mavlink_msg_statustext_decode(&message, &statusTextMessage);
                cout << "Status Text Message received" << endl;

                cout << "\tSeverity: " << (int)statusTextMessage.severity << endl;
                static char msgText[51] = {0};
                for(int i = 0; i < 50; i++)
                    msgText[i] = statusTextMessage.text[i];
                cout << "\tText: " << msgText << endl;

                cout << endl;
            }
            break;
        default:
            cout << "Message ID: " << (int)message.msgid << endl;
            break;
    }
}

} // end of namespace MavMsgUx

#endif // __JJC__MAVLINK__MESSAGE__INTERFACE__HPP__
