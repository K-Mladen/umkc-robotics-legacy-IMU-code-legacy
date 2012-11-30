/******************************************************************************
*  Rotate Point of view from robot frame to field frame  --MK								  *
******************************************************************************/

#include <cmath>
#include "pVector.h"

pVector rotatePOV(pVector vec, pVector rot)
{
  double rad =  std::atan(1)/45; //(pi/4)/45 = pi/180
  double s = sin(rot.magnitude()*rad),
         c = cos(rot.magnitude()*rad);
  pVector newVec
    (
	/*X component*/
	(c+pow(rot.unitComponent(X),2)*(1-c))*vec.component(X) +
	(rot.unitComponent(X)*rot.unitComponent(Y)*(1-c)
	  - rot.unitComponent(Z)*s)*vec.component(Y) +
	(rot.unitComponent(X)*rot.unitComponent(Z)*(1-c)
	  + rot.unitComponent(Y)*s)*vec.component(Z),
	/*Y component*/
	(rot.unitComponent(Y)*rot.unitComponent(X)*(1-c)
	  + rot.unitComponent(Z)*s)*vec.component(X) +
	(c+pow(rot.unitComponent(Y),2)*(1-c))*vec.component(Y) +
	(rot.unitComponent(Y)*rot.unitComponent(Z)*(1-c)
	  - rot.unitComponent(X)*s)*vec.component(Z),
	/*Z component*/
	(rot.unitComponent(Z)*rot.unitComponent(X)*(1-c)
	  - rot.unitComponent(Y)*s)*vec.component(X) +
	(rot.unitComponent(Z)*rot.unitComponent(Y)*(1-c)
	  + rot.unitComponent(X)*s)*vec.component(Y) +
	(c+pow(rot.unitComponent(Z),2)*(1-c))*vec.component(Z)
	);
  return newVec;
}
