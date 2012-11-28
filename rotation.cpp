/******************************************************************************
*  Rotate Point of view from robot frame to field frame  --MK								  *
******************************************************************************/

#include <cmath>
#include "pVector.h"

pVector rotatePOV(pVector vec, pVector rot)
{
	
/*	double thesine = sin(rot.getMagnitude()/2),
				 a = cos(rot.getMagnitude()/2),
//				 b = thesine*vec.getUnitComponent(X),
//				 c = thesine*vec.getUnitComponent(Y),
//				 d = thesine*vec.getUnitComponent(Z),
				 ab = a*thesine*vec.getUnitComponent(X),
				 ac = a*thesine*vec.getUnitComponent(Y),
				 ad = a*thesine*vec.getUnitComponent(Z),
				 bc = thesine*vec.getUnitComponent(X)*thesine*vec.getUnitComponent(Y),
				 bd = thesine*vec.getUnitComponent(X)*thesine*vec.getUnitComponent(Z),
				 cd = thesine*vec.getUnitComponent(Y)*thesine*vec.getUnitComponent(Z),
//				 a2 = a*a,
				 b2 = thesine*vec.getUnitComponent(X)*thesine*vec.getUnitComponent(X),
				 c2 = thesine*vec.getUnitComponent(Y)*thesine*vec.getUnitComponent(Y),
				 d2 = thesine*vec.getUnitComponent(Z)*thesine*vec.getUnitComponent(Z); */
	double 	s = sin(rot.getMagnitude()),
			c = cos(rot.getMagnitude());
  	pVector newVec
		(
		/*X component*/
		(c+rot.component(X)*(1-c))*vec.component(X) +
		(rot.component(X)*rot.component(Y)*(1-c)-rot.component(Z)*s)*vec.component(Y) +
		(rot.component(X)*rot.component(Z)*(1-c)+rot.component(Y)*s)*vec.component(Z),
		/*Y component*/
		(rot.component(Y)*rot.component(X)*(1-c)+rot.component(Z)*s)*vec.component(X) +
		(c+rot.component(Y)*(1-c))*vec.component(Y) +
		(rot.component(Y)*rot.component(Z)*(1-c)-rot.component(X)*s)*vec.component(Z),
		/*Z component*/
		(rot.component(Z)*rot.component(X)*(1-c)-rot.component(Y)*s)*vec.component(X) +
		(rot.component(Z)*rot.component(Y)*(1-c)*rot.component(X)*s)*vec.component(Y) +
		(c+rot.component(Z)*(1-c))*vec.component(Z)
		);



		/*X component
		(1-2*(c2+d2))*vec.getComponent(X) +
		 2*(bc-ad)*vec.getComponent(Y) +
		 2*(ac+bd)*vec.getComponent(Z),
		Y component
		 2*(ac+bc)*vec.getComponent(X) +
		(1-2*(b2+d2))*vec.getComponent(Y) + 
		 2*(cd-ab)*vec.getComponent(Z),
		Z component
		 2*(bd-ac)*vec.getComponent(X) +
		 2*(cd+ab)*vec.getComponent(Y) +
		(1-2*(b2+c2))*vec.getComponent(Z)
		);*/
	return newVec;
}
