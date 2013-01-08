#ifndef mathy
#define mathy

//i can't get to these ... :(
/*
namespace mathy	{
	pVector zeroVector(0,0,0);
	pVector XUnitVector(1,0,0); 
};
*/

double simpsonIteration(double const pt[], int const ust[]);
void integrateGyro(spatial::PVectorQ* data, pVector& current);
pVector rotatePOV(pVector const & vec, pVector const & rot);
pVector orientation(pVector const & angle);
pVector filter(pVector const & accel, pVector const & gyrot, double alpha);

#endif
