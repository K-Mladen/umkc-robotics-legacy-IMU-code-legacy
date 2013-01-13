umkc-robotics-2012-odometry
===========================
Sandbox for the odometry team to make our code and make it all nice and shiny and working.
--

headerFiles/
-------------
Headers and their implementation


dataPoints/
-------------
some raw, and not-so-raw, data for pre-alpha/alpha testing of code snippets

referencecode/
-------------
basically, trash, but still here just in case



Notes

Working with phidget21 in ...
Mac
gcc example.c -o example -framework Phidget21 

Linux
gcc example.c -o example -lphidget21

*NOTE* = The timestamp for phidget Events counts from when phidget is attached. 



Kalman explanation MK needs to maybe read. http://tom.pycke.be/mav/71/kalman-filtering-of-imu-data
--actually, list of reference material MK needs to go through & read
--I don't remember--
--I don't remember--
ROS tut, continue it.



MK-- To whom it may concern, I created a ROS project for compiling this within the ROS sandbox. to find it, log into the umkc/right machine and enter roscd odom
