/*******************************************************************************
*  simpsonIteration -- implements a step in simpsons rule. sum them to get     *
*    an integral                                                               *
*                                                                              *
*  rotationPOV -- finds vector in 'global POV', given robot's POV and angle    *
*    between the POV's                                                         *
*                                                                              *
*  orientation -- wrapper; gets robot's orientation given the ^  angle         *
*                                                                              *
*  filter -- implements complementary filter                                   *
*                                                                              *
*******************************************************************************/

#include <cmath>
#include "pVector.h"


//pt[i] is the value of the ith element
//ust[i] is the time (in us) at which the ith element was sampled
double simpsonIteration(double pt0, double pt1, double pt2,
    int ust0, int ust1, int ust2)
{
  /*microseconds to seconds*/
  double t0 = ust0/1000000.0,
         t1 = ust1/1000000.0,
         t2 = ust2/1000000.0;

  /*make sure time interval is constant*/
  if ((t1 - t0)==(t2 - t1))
  {
    /*if so, do simpsons rule normally*/
    return ((t1-t0)/3*(pt0 + 4*pt1 + pt2));
  } else {
    /*Otherwise, split simpsons rule into smaller parts, and add them*/
    double result0, result1;
    result0 = (t1-t0)/3*(pt0+2*pt1);
    result1 = (t2-t1)/3*(2*pt1+pt2);
    return (result0+result1);
  }
}





//vec is vector we're rotating (in phidget's POV)
//rot is vector we're rotating about (the vector orthogonal to the plane made
//	by the initial orientation vector and the current orientation vector)
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


pVector orientation(pVector angle)
{
  return rotatePOV((pVector)(1,0,0),((pVector)(0,0,0) - angle));
}


pVector filter(pVector accel, pVector gyrot, double alpha)
{
  /*current time step t2 (so T=t-1 is t1 and T=t-2 is t0)*/
  return (pVector)(
    anglet2.set(X, accel.component(X)*(1-ALPHA)+ALPHA*gyrot.component(X),
    anglet2.set(Y, accel.component(Y)*(1-ALPHA)+ALPHA*gyrot.component(Y),
    anglet2.set(Z, accel.component(Z)*(1-ALPHA)+ALPHA*gyrot.component(Z),
  );
}
