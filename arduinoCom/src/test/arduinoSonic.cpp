/*
this file was made by Andrew Vetter.
This progrma is used for testing and deveoplment of the ecnoder driver class.
*/
//#include <Encoder_arduino.h>
#include <iostream>
#include <unistd.h>
#include "../../include/RS232_GenericController.h"

using namespace std;

int16_t pChecksum = 0;

void readChunk(SerialController& sc, char* buf, int n) {
    int bytes_read = 0;

    while(bytes_read < n) {
        bytes_read += sc.readBuffer(buf+bytes_read, n-bytes_read);
    }
}

//compares checksum from arduino with CPU
bool checkDatSum(char* buf, int16_t __char){

    //test if the char we sent match the one returned
    int16_t arCheckSum = *(int16_t*)(buf+7);
    if(__char != *(int16_t*)(buf)){
        return false;
    }
    // makes sure the 2nd byte is a capital L
    if('L' != *(int16_t*)(buf+1)){
        return false;
    }
    // makes sure the 4th byte is a capital R
    if('R' != *(int16_t*)(buf+4)){
        return false;
    }

    //build our checksum
    int16_t cpuCheckSum = __char ^ 'L'^ *(int16_t*)(buf+2) ^ 'R' ^ *(int16_t*)(buf+5) ;

    //if check sum from arduino equals last checksum
    if(arCheckSum != pChecksum){
        return false;
    }
    if(arCheckSum == pChecksum) {
        //update previous checksum
        pChecksum = arCheckSum;
        return true;
    }

}

void StartDatEncoder(SerialController encoders){

    //arduino.set_SerialPort(port);
    encoders.set_BaudRate(B57600);
    encoders.set_Parity(Parity_Off);
    encoders.set_WordSize(WordSize_8);
    encoders.set_StopBits(StopBits_1);
    encoders.start();


}

int main(int argc, char *argv[]) {

    if(argc != 2) {
        cerr << "Usage: " << argv[0] << "<port name>" << endl;
        return 1;
    }

    SerialController arduino(argv[1]);
    StartDatEncoder(arduino);

/*
    SerialController arduino(argv[1]);
    arduino.set_BaudRate(B57600);
    arduino.set_Parity(Parity_Off);
    arduino.set_WordSize(WordSize_8);
    arduino.set_StopBits(StopBits_1);
    arduino.start();
*/
    char buf[8];
    //uint8_t ping = 1;

    cout << "getting data from arduino" << endl;
    char _char = 'a';
    while(1) {
        //if(arduino.is_open()){
        arduino.writeBuffer(&_char, 1);

        for(int i = 0; i < 8; i++){
            buf[i] = 0x00;}

        readChunk(arduino, buf, 8);
//        arduino.readBuffer(buf, 8);

        if(checkDatSum(buf, (int16_t)_char)){

            int16_t right = *(int16_t*)(buf+5);
            int16_t left  = *(int16_t*)(buf+2);

            cout << "Right: " << right << " Left: " << left << endl;
            usleep(100000);
        }
    }

    return 0;
}
