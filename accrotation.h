//Some reference:   http://en.wikipedia.org/wiki/Quaternions_and_spatial_rotation
//   materials  :   http://content.gpwiki.org/index.php/OpenGL:Tutorials:Using_Quaternions_to_represent_rotation
#ifndef accrotation
#define accrotation

#include "acc.h"
#include <math>


accfield accrotation(accphidget,offset, posangle)
{
//get compass vector from compass or gyroscope --or passed in
//need posangle.magnitude and posangle.unitx=posangle.x/posangle.magnitude

//translate accphidget to accrob, using offset --these are structs; from acc.h maybe
accrob.x = accphidget.x - sqrt(pow(accphidget.roty*offset.z,2) +
								pow(accphidget.rotz*offset.y,2));
accrob.y = accphidget.y - sqrt(pow(accphidget.rotx*offset.z,2) +
								pow(accphidget.rotz*offset.x,2));
accrob.z = accphidget.z - sqrt(pow(accphidget.roty*offset.x,2) +
								pow(accphidget.rotx*offset.y,2));


//calculate intermediate values

//introduce rotation matrix
//[1-2(c2+d2)	2(bc-ad)	2(ac+bd)	]	[old.x]		[new.x]
//|2(ad+bc)		1-2(b2+d2)	2(cd-ab)	|	|old.y|	=	|new.y|
//[2(bc-ad)		2(cd+ab)	1-2(b2+c2)	]	[old.z]		[new.z]

accfield.x = (1-2*(c2+d2))*accrob.x + 2*(bc-ad)*accrob.y + 2*(ac+bd)*accrob.z;
accfield.y = 2*(ad+bc)*accrob.x + (1-2*(b2+d2))*accrob.y + 2*(cd-ab)*accrob.z;
//not needed: accfield.z
//accfield.z = 2*(bd-ac)*accrob.x + 2*(cd+ab)*accrob.y + (1-2*(b2+c2))*accrob.z;

//next, do the same for the angular accelerations



//notation note:magnitude = t, cos = c, sin = s, unit components=ax,ay,az
/* These don't quite work... for example, ^ isn't an operator. but I guess this is what a more optimized matrix
looks like
accglobal.x = (c(t)+(ax^2)(1-c(t)))*accrob.x + (ax*ay*(1-c(t))-az*s(t))*accrob.y + (ax*az(1-c(t))+ay*s(t))*accrob.z;
accglobal.y = (ay*ax*(1-c(t))+az*s(t))*accrob.x + (c(t)+(ay^2)(1-c(t)))*accrob.y + (ay*az(1-c(t))-ax*s(t))*accrob.z;
accglobal.z = (az*ax*(1-c(t))-ay*s(t))*accrob.x + (az*ay*(1-c(t))+ax*s(t))*accrob.y + (c(t)+(az^2)(1-c(t))*accrob.z;
*/

return accfield;
}
#endif
