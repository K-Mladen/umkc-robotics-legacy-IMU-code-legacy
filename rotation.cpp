/******************************************************************************
*  Rotate Point of view from robot frame to field frame  --MK								  *
******************************************************************************/

#include <cmath>
#include "pVector.h"

pVector rotatePOV(pVector vec, pVector rot)
{
	double thecosine = cos(rot.getMagnitude()/2),
				 a = sin(rot.getMagnitude()/2),
//				 b = thecosine*vec.getUnitComponent(X),
//				 c = thecosine*vec.getUnitComponent(Y),
//				 d = thecosine*vec.getUnitComponent(Z),
				 ab = a*thecosine*vec.getUnitComponent(X),
				 ac = a*thecosine*vec.getUnitComponent(Y),
				 ad = a*thecosine*vec.getUnitComponent(Z),
				 bc = thecosine*vec.getUnitComponent(X)*thecosine*vec.getUnitComponent(Y),
				 bd = thecosine*vec.getUnitComponent(X)*thecosine*vec.getUnitComponent(Z),
				 cd = thecosine*vec.getUnitComponent(Y)*thecosine*vec.getUnitComponent(Z),
//				 a2 = a*a,
				 b2 = thecosine*vec.getUnitComponent(X)*thecosine*vec.getUnitComponent(X),
				 c2 = thecosine*vec.getUnitComponent(Y)*thecosine*vec.getUnitComponent(Y),
				 d2 = thecosine*vec.getUnitComponent(Z)*thecosine*vec.getUnitComponent(Z);
  	pVector newVec
		(
		/*X component*/
		(1-2*(c2+d2))*vec.getComponent(X) +
		 2*(bc-ad)*vec.getComponent(Y) +
		 2*(ac+bd)*vec.getComponent(Z),
		/*Y component*/
		 2*(ac+bc)*vec.getComponent(X) +
		(1-2*(b2+d2))*vec.getComponent(Y) + 
		 2*(cd-ab)*vec.getComponent(Z),
		/*Z component*/
		 2*(bd-ac)*vec.getComponent(X) +
		 2*(cd+ab)*vec.getComponent(Y) +
		(1-2*(b2+c2))*vec.getComponent(Z)
		);
	return newVec;
}
