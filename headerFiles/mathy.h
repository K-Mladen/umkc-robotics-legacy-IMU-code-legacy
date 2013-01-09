
/*****************************************************************************
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
#ifndef mathy
#define mathy

//i can't get to these ... :(
/*
namespace mathy	{
	pVector zeroVector(0,0,0);
	pVector XUnitVector(1,0,0); 
};
*/

#include "spatial.h"


double simpsonIteration(double pt[], int ust[]);
void integrateGyro(spatial::PVectorQ* data, pVector& current);
pVector rotatePOV(pVector & vec, pVector & rot);
pVector orientation(pVector & angle);
void filter(pVector & accel, pVector & gyrot, double alpha);

#endif
