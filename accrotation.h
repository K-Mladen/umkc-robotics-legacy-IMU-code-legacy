//Some reference:   http://en.wikipedia.org/wiki/Quaternions_and_spatial_rotation
//   materials  :   http://content.gpwiki.org/index.php/OpenGL:Tutorials:Using_Quaternions_to_represent_rotation
#ifndef accrotation
#define accrotation

#include "acc.h"


accfield accrotation(accphidget,adjust)
{
//get compass vector from compass or gyroscope --or passed in
//translate accphidget to accrob, using adjust --these are structs; from acc.h maybe
//get magnitude and {unit components if unit components are necessary}
//notation note:magnitude = t, cos = c, sin = s, unit components=ax,ay,az
/* These don't quite work... for example, ^ isn't an operator. but I guess this is what a more optimized matrix
looks like
accglobal.x = (c(t)+(ax^2)(1-c(t)))*accrob.x + (ax*ay*(1-c(t))-az*s(t))*accrob.y + (ax*az(1-c(t))+ay*s(t))*accrob.z;
accglobal.y = (ay*ax*(1-c(t))+az*s(t))*accrob.x + (c(t)+(ay^2)(1-c(t)))*accrob.y + (ay*az(1-c(t))-ax*s(t))*accrob.z;
accglobal.z = (az*ax*(1-c(t))-ay*s(t))*accrob.x + (az*ay*(1-c(t))+ax*s(t))*accrob.y + (c(t)+(az^2)(1-c(t))*accrob.z;
*/

return accglobal;
}
#endif
