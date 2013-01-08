#ifndef mathy
#define mathy

//i can't get to these ... :(
/*
namespace mathy	{
	pVector zeroVector(0,0,0);
	pVector XUnitVector(1,0,0); 
};
*/

double simpsonIteration(double pt[], int ust[]);
void integrateGyro(spatial::PVectorQ* data, pVector& current);
pVector rotatePOV(pVector vec, pVector rot);
pVector orientation(pVector angle);
pVector filter(pVector accel, pVector gyrot, double alpha);

#endif
