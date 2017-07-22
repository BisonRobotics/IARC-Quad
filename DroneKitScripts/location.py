from dronekit import *
import time
import sys

# Connect to the Vehicle
# argv[1] = connection name
vehicle = connect(sys.argv[1], wait_ready=True, baud=57600)
#vehicle = connect("/dev/ttyUSB0", wait_ready=True)

"""
print "Prearm checks"
while not vehicle.is_armable:
    print "Waiting for vehicle to init.."
    time.sleep(1)
"""

print "Arming motors"
vehicle.mode = VehicleMode("GUIDED")
vehicle.armed = True

while not vehicle.armed:
    print "waiting for arming"
    time.sleep(1)

while True:
    print "Location: %s" % vehicle.location.local_frame
    time.sleep(1)
