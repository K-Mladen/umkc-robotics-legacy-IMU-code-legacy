#ifndef accrotation
#define accrotation

#include "acc.h"


accfield accrotation(accphidget,adjust)
{

//acceleration in robot's reference frame (rf) is acceleration in phidget's rf
// minus arc length of phidget's travel due to rotation of robot
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




}
#endif
