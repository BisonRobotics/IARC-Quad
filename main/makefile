MAIN_FILES =  main.cpp
MAIN_FILES += RS232_GenericController.cpp
MAIN_FILES += PixhawkCommands.hpp
MAIN_FILES += MavlinkMessageInterface.hpp

all: main

main: main.cpp
	g++ -I mavlink/include/mavlink/v1.0 $(MAIN_FILES) -o main -lpthread -std=c++11

git_submodule:
	git submodule update --init --recursive
