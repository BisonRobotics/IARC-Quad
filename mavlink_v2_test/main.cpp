#include <iostream>
#include <fstream>
#include <stdlib.h>

#include "../mavlib/v2/common/mavlink.h"
#include "../mavlib/MavlinkMessageInterface.hpp"

using namespace std;

const int system_id    = 0;
const int component_id = 0;

int main(int argc, char* argv[]) {
	if(argc != 2) {
		cerr << "Usage: " << argv[0] << " <port name>" << endl;
		exit(1);
	}

    ifstream pixhawk_2(argv[1], ios::binary | ios::in);

    mavlink_message_t mav_mesg;
    mavlink_status_t  mav_stat;

    while(1) {
        uint8_t byte;
        pixhawk_2.read((char*)&byte, 1);

        if(mavlink_frame_char(0, byte, &mav_mesg, &mav_stat)) {
            cout << "Msgid: " << mav_mesg.msgid <<  endl;
					printMessageContents(mav_mesg);
        }
    }

    return 0;
}
