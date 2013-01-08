#ifndef mathy
#define mathy

//i can't get to these ... :(
/*
namespace mathy	{
	pVector zeroVector(0,0,0);
	pVector XUnitVector(1,0,0); 
};
*/
#include "phidget_setup_buffer.h"

double simpsonIteration(double const pt[], int const ust[]);
void integrateGyro(spatial::PVectorQ* data, pVector& current);
void rotatePOV(pVector  & vec, pVector & rot);
pVector orientation(pVector & angle);
pVector filter(pVector & accel, pVector & gyrot, double alpha);

#endif
