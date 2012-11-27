/******************************************************************************
*  Rotate Point of view from robot frame to field frame  --MK								  *
******************************************************************************/

#include <cmath>
#include "pVector.h"

pVector rotatePOV(pVector vec, pVector rot)
{
	double thecosine = cos(rot.getMagnitude()/2),
				 a = sin(rot.getMagnitude()/2),
//				 b = thecosine*vec.getUnit(X),
//				 c = thecosine*vec.getUnit(Y),
//				 d = thecosine*vec.getUnit(Z),
				 ab = a*thecosine*vec.getUnit(X),
				 ac = a*thecosine*vec.getUnit(Y),
				 ad = a*thecosine*vec.getUnit(Z),
				 bc = thecosine*vec.getUnit(X)*thecosine*vec.getUnit(Y),
				 bd = thecosine*vec.getUnit(X)*thecosine*vec.getUnit(Z),
				 cd = thecosine*vec.getUnit(Y)*thecosine*vec.getUnit(Z),
				 a2 = a*a,
				 b2 = thecosine*vec.getUnit(X)*thecosine*vec.getUnit(X),
				 c2 = thecosine*vec.getUnit(Y)*thecosine*vec.getUnit(Y),
				 d2 = thecosine*vec.getUnit(Z)*thecosine*vec.getUnit(Z);
  return pVector newVec
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
}
