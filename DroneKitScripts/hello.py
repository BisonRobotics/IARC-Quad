print "Start simulator (SITL)"

import dronekit_sitl
import time

sitl = dronekit_sitl.start_default()
connection_string = sitl.connection_string()

#import Dronekit-Python
from dronekit import connect, VehicleMode

def arm_and_takeoff(target_altitude):
    print "  Prearm checks"
    while not vehicle.is_armable:
        print " Waiting for vehicle to init.."
        time.sleep(1)

    print " Arming motors"
    vehicle.mode = VehicleMode("GUIDED")
    vehicle.armed = True

    while not vehicle.armed:
        print " waiting for arming"
        time.sleep(1)

    print "Taking off"
    vehicle.simple_takeoff(target_altitude)

    while(True):
        print "Altitude: ", vehicle.location.global_relative_frame.alt

        #break if within tolerance
        if vehicle.location.global_relative_frame.alt >= target_altitude * 0.95:
            print "Target altitude reached"
            break

        time.sleep(1)

# connect to the vehicle
print("COnnecting to vehicle on: %s" % (connection_string,))
vehicle = connect(connection_string, wait_ready=True)

# get some vehicle attributes
print "Get some vehicle attribute values:"
print "  GPS: %s" % vehicle.gps_0
print "  Battery: %s" % vehicle.battery
print "  Last Heartbeat: %s" % vehicle.last_heartbeat
print "  Is Armable: %s" % vehicle.is_armable
print "  System status: %s" % vehicle.system_status.state
print "  Mode: %s" % vehicle.mode.name # settable

while True:
    print "Location: " 

arm_and_takeoff(10.0)

#goto_position_target_local_ned(0.0, 0.0, -2.0)

# Close the vehicle object
vehicle.close()
sitl.stop()
print("Completed")








