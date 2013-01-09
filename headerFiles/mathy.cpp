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
#include "mathy.h"


//pt[i] is the value of the ith element
//ust[i] is the time (in us) at which the ith element was sampled
double simpsonIteration(double pt[],int  ust[])
{
  /*microseconds to seconds*/
  double t0 = ust[0]/1000000.0,
         t1 = ust[1]/1000000.0,
         t2 = ust[2]/1000000.0;

  /*make sure time interval is constant*/
  if ((t1 - t0)==(t2 - t1))
  {
    /*if so, do simpsons rule normally*/
    return ((t1-t0)/3*(pt[0] + 4*pt[1] + pt[2]));
  } else {
    /*Otherwise, split simpsons rule into smaller parts, and add them*/
    double result0, result1;
    result0 = (t1-t0)/3*(pt[0]+2*pt[1]);
    result1 = (t2-t1)/3*(2*pt[1]+pt[2]);
    return (result0+result1);
  }
}

void integrateGyro(spatial::PVectorQ* data, pVector& current)
{
  pVector delta;
  double gyros[3][3];

  for(int i =0; i< 3; i++)  {
    for(int k = 0; k<3; k++)  {
      gyros[i][k] = data->at(i).angularRate[k];
    }
  }

  int times[3] = {data->at(0).elapsed,data->at(1).elapsed,data->at(2).elapsed};
  delta.set(
    simpsonIteration(gyros[0],times),
    simpsonIteration(gyros[1],times),
    simpsonIteration(gyros[2],times)
  );
  current += delta;
  return;
}



//vec is vector we're rotating (in phidget's POV)
//rot is vector we're rotating about (the vector orthogonal to the plane made
//	by the initial orientation vector and the current orientation vector)

//changing-> rot is the current orientation vector??

//changing this to a set method? to change contents of vec, instead of returning new pVector?
void rotatePOV(pVector & vec, pVector & rot)
{

  double rad =  std::atan(1)/45; //(pi/4)/45 = pi/180
  double s = sin(rot.magnitude()*rad),
         c = cos(rot.magnitude()*rad);
  
  double xDir = (c+pow(rot.unitComponent(X),2)*(1-c))*vec.component(X) +
        (rot.unitComponent(X)*rot.unitComponent(Y)*(1-c)
        - rot.unitComponent(Z)*s)*vec.component(Y) +
        (rot.unitComponent(X)*rot.unitComponent(Z)*(1-c)
        + rot.unitComponent(Y)*s)*vec.component(Z);
  
  double yDir = (rot.unitComponent(Y)*rot.unitComponent(X)*(1-c)
      + rot.unitComponent(Z)*s)*vec.component(X) +
      (c+pow(rot.unitComponent(Y),2)*(1-c))*vec.component(Y) +
      (rot.unitComponent(Y)*rot.unitComponent(Z)*(1-c)
      - rot.unitComponent(X)*s)*vec.component(Z);

  double zDir =  (rot.unitComponent(Z)*rot.unitComponent(X)*(1-c)
          - rot.unitComponent(Y)*s)*vec.component(X) +
          (rot.unitComponent(Z)*rot.unitComponent(Y)*(1-c)
          + rot.unitComponent(X)*s)*vec.component(Y) +
          (c+pow(rot.unitComponent(Z),2)*(1-c))*vec.component(Z);

  vec.set(xDir, yDir, zDir);
  return;
/*  pVector newVec
    (
	//X component
	(c+pow(rot.unitComponent(X),2)*(1-c))*vec.component(X) +
	(rot.unitComponent(X)*rot.unitComponent(Y)*(1-c)
	  - rot.unitComponent(Z)*s)*vec.component(Y) +
	(rot.unitComponent(X)*rot.unitComponent(Z)*(1-c)
	  + rot.unitComponent(Y)*s)*vec.component(Z),
	// Y component
	(rot.unitComponent(Y)*rot.unitComponent(X)*(1-c)
	  + rot.unitComponent(Z)*s)*vec.component(X) +
	(c+pow(rot.unitComponent(Y),2)*(1-c))*vec.component(Y) +
	(rot.unitComponent(Y)*rot.unitComponent(Z)*(1-c)
	  - rot.unitComponent(X)*s)*vec.component(Z),
	// Z component
	(rot.unitComponent(Z)*rot.unitComponent(X)*(1-c)
	  - rot.unitComponent(Y)*s)*vec.component(X) +
	(rot.unitComponent(Z)*rot.unitComponent(Y)*(1-c)
	  + rot.unitComponent(X)*s)*vec.component(Y) +
	(c+pow(rot.unitComponent(Z),2)*(1-c))*vec.component(Z)
	);

   return newVec;
*/
}


pVector orientation(pVector & angle)
{
  //note+ need to change to using the constants in mathy.h 

    pVector zero(0.0,0.0,0.0); pVector unitX(1.0,0.0,0.0);
    zero -= angle;
    rotatePOV(unitX,zero);
    return unitX;
  // return rotatePOV(mathy::XUnitVector,(mathy::zeroVector - angle));
}


void filter(pVector & accel, pVector & current, double alpha)
{
  /*current time step t2 (so T=t-1 is t1 and T=t-2 is t0)*/
  pVector angle
  (
    accel.component(X)*(1-alpha)+alpha*current.component(X),
    accel.component(Y)*(1-alpha)+alpha*current.component(Y),
    accel.component(Z)*(1-alpha)+alpha*current.component(Z)
  );
  current = angle;
}
