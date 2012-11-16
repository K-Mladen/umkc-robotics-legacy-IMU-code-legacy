umkc-robotics-2012-odometry
===========================
Sandbox for the odometry team to make our code and make it all nice and shiny and working.
-
MK-- I can't seem to get this kalman thing out of my head, and I ended up finding this nice site that 
      breaks everything down nicely for our specific situation which doesn't help any.  We just need to 
      figure out E (the expected value I think) for the noise and it might be a lot easier to set this up
      than it originally seemed.  stupid explanations of how the magic works.
http://tom.pycke.be/mav/71/kalman-filtering-of-imu-data

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
                for position in field POV

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
gcc example.c -o example -framework Phidget21 -I/Library/Frameworks/Phidget21.framework/Headers

Linux
gcc example.c -o example -lphidget21