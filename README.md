umkc-robotics-2012-odometry
===========================
Sandbox for the odometry team to make our code and make it all nice and shiny and working.
--
MK-- To whom it may concern, I created a ROS project for compiling this within the ROS sandbox. to find it, log into the umkc/right machine and enter roscd odom



rotation.h
------------

OdometryNode.cpp
-------------
Main


referencecode/odometry_roughNode.cpp
-------------
Example code from ROS, publishing to odometry

odomvar.h
-------------
Defines structs for acceleration in phidget, robot, and field POV, as well as
                for position in field POV  TODO: convert to arrays, as well as in areas where it's used

phidget_setup.h
--------------
Setting up the phidget.

spatial_tester.cpp
--------------
Tester file for phidget_setup.h

talker.h
---------------
supposed to be example code for publishing to ROS but is actually blank... TODO:Remove this or fill it in.


Notes

Working with phidget21 in ...
Mac
gcc example.c -o example -framework Phidget21 

Linux
gcc example.c -o example -lphidget21



Kalman explanation MK needs to maybe read. http://tom.pycke.be/mav/71/kalman-filtering-of-imu-data
--actually, list of reference material MK needs to go through & read
--I don't remember--
--I don't remember--
ROS tut, continue it.
