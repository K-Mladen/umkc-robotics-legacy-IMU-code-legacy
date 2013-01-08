#ifndef mathy
#define mathy

//i can't get to these ... :(
/*
namespace mathy	{
	pVector zeroVector(0,0,0);
	pVector XUnitVector(1,0,0); 
};
*/

double simpsonIteration(double pt0, double pt1, double pt2,
    int ust0, int ust1, int ust2);
pVector rotatePOV(pVector vec, pVector rot);
pVector orientation(pVector angle);
pVector filter(pVector accel, pVector gyrot, double alpha);

#endif
