umkc-robotics-2012-odometry
===========================
Sandbox for the odometry team to make our code and make it all nice and shiny and working.

accrotation.h
------------

accrotationjunk.h
------------

odometryNode.cpp
-------------

odometry_roughNode.cpp
-------------
Example code from ROS, publishing to odometry

odomvar.h
-------------
Defines structs acceleration 3d, 2d, and velocity in 2d.

phidget_setup.h
--------------
Setting up the phidget.

spatial_tester.cpp
--------------
Tester file for phidget_setup.h

talker.h
---------------



Notes

Working with phidget21 in ...
Mac
gcc example.c -o example -framework Phidget21 -I/Library/Frameworks/Phidget21.framework/Headers

Linux
gcc example.c -o example -lphidget21