MAIN_FILES =  main.cpp
MAIN_FILES += RS232_GenericController.cpp
MAIN_FILES += PixhawkCommands.hpp
MAIN_FILES += MavlinkMessageInterface.hpp

MAV_INC = mavlink/include/mavlink/v1.0

# interface files
IF_FILES =  RS232_GenericController.cpp
IF_FILES += PixhawkCommands.hpp
IF_FILES += MavlinkMessageInterface.hpp

# standard options
OPTS = -lpthread -std=c++11

all: main listener

main: main.cpp
	g++ -I mavlink/include/mavlink/v1.0 $(MAIN_FILES) -o main -lpthread -std=c++11

listener: listener/main.cpp
	g++ -I$(MAV_INC) $(IF_FILES) listener/main.cpp -o listener/main $(OPTS)

git_submodule:
	git submodule update --init --recursive

.PHONY: listener


