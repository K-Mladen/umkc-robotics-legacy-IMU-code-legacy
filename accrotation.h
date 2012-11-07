#ifndef accrotation
#define accrotation

#include "acc.h"


accfield accrotation(accphidget,adjust)
{

//acceleration in robot's reference frame (rf) is acceleration in phidget's rf
// minus arc length of phidget's travel due to rotation of robot.
//The arc length is determined in perpendicular  components that are also 
// perpendicular to the direction of the component we are considering, and the
// components are combined using the pythagorean theorem
accrob.x = accphidget.x - sqrt(accphidget.roty*accphidget.roty*  //square root
								adjust.z*adjust.z +				 //(roty*z)^2+
								accphidget.rotz*accphidget.rotz* //(rotz*y)^2
								adjust.y*adjust.y)		
accrob.y = accphidget.y - sqrt(accphidget.rotx*accphidget.rotx*
								adjust.z*adjust.z + 
								accphidget.rotz*accphidget.rotz
								adjust.x*adjust.x
accrob.z = accphidget.z - sqrt(accphidget.rotx*accphidget.rotx*
								adjust.y*adjustt.y +
								accphidget.roty*accphidget.roty*
								adjust.z*adjust.z

//Now we have the xyz accelerations in the robot's reference frame(rf). we need
// them relative to the field, ie in the global rf
//for this we construct a rotation matrix.
//This rotation matrix was built using quaternions and lots of algerbra that
// I'm hopefully going to post somewhere for you to look up if you're
// interested.

//first, I will define the rotational acceleration  vector, which points in
// the direction about which the robot's angular acceleration is happening.
// The x, y, and z components of this vector are given to us by the IMU,
// specifically its gyroscope, in the robot's reference frame
//We translate these to the global reference frame by adding the rotational
// acceleration vector and the angular position vector which we get from the 
// IMU's compass, or from having added the change in position of our robot
// since the last time it was "zeroed"

/////*****global rot acc vector = rob rot acc vector + rob rad pos vector

//Next, we need the magnitude of the rotational acceleration vector, and
// the components of its unit vector

/////******magnitude = square root of squares of components theta, phi, & gamma
/////******unit.x = theta/magnitude ... but theta is rotx
/////******unit.y = roty/magnitude
/////******unit.z = rotz/magnitude

//Now that we have all these, we define a, b, c, and d, from the quaternion
// a + bi + cj + dk 
//The following is from a rotation matrix constructed by setting
//global.(xi+yj+zk)= (a + bi + cj + dk)rob.(xi + yj + zk)(a + bi + cj + dk)
//where a = sin(angle of rotation/2) and
// (b,c,d) = cos(angle of rotation/2)*(unit vector axis of rotation)

tmpcos = cos(magnitude/2);
a = sin(magnitude/2);
b = tmpcos*unit.x;
c = tmpcos*unit.y;
d = tmpcos*unit.z;

//we define some variables to cut down on math the cpu has to do

//finally we apply the calculations from our rotation matrix.
}
#endif
