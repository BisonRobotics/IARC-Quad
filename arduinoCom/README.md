For developing code to talk to arduinos over serial and talking to all the ultrasonic sensors.

## Build instructions 

To build the program to test serial communcation go to build folder and run "make test_sonic" this will build the the arduinoSomic programm under src/test and rs232 dependnancy. 

To run arduinoSonic program go to build/bin and run "./test_sonic /port/for/arduino" replace /port/for/arduino with the location where the arduino is connected usually under /dev/.
example: "./test_sonic /dev/ttyASM0" or "./test_sonic /dev/ttyUSB0" 

the code that runs on the arduino is under arduino code. -- that's a redundant statement. 
