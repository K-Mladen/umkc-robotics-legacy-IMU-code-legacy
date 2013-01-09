/*
Setting up stuff for the Phidget.

TODO:
Take care of overflow errors.
Documentation on data rate.
*/


#ifndef PHIDGET_SETUP_BUFFER_H
#define PHIDGET_SETUP_BUFFER_H

#include <stdio.h>
#include <deque>
#include <Phidget21/phidget21.h>	//mac os
//#include <phidget21.h>			//linux
#include <iostream>
#include "pVector.h"


//extern int event;


namespace spatial	{ 

	struct SpatialPVector	{
		int elapsed; 
		pVector acceleration, angularRate, comp;
		SpatialPVector(): elapsed(0), acceleration(pVector()), angularRate(pVector()), comp(pVector())	{}
		
		SpatialPVector(CPhidgetSpatial_SpatialEventData &data) {
			//why isn't this working - undefined symbols D:
			//elapsed = elapsedTime(data);
			elapsed = data.timestamp.seconds*1000000 + data.timestamp.microseconds;
			acceleration.set(data.acceleration);
			angularRate.set(data.angularRate);
			comp.set(data.magneticField);
		}
/*
		rotate(SpatialPVector& spatial, pVector about)	{

		}	*/
	};

	typedef deque<CPhidgetSpatial_SpatialEventData> PhidgetRawDataQ; 
	typedef deque<SpatialPVector> PVectorQ;

	int spatial_setup(CPhidgetSpatialHandle &spatial, deque<CPhidgetSpatial_SpatialEventData>* raw, int dataRate );
	void print(CPhidgetSpatial_SpatialEventData& data);
	CPhidgetSpatial_SpatialEventData* copy(CPhidgetSpatial_SpatialEventData& spatial);
	int elapsedTime(CPhidgetSpatial_SpatialEventData& spatial);
	int elapsedTime(SpatialPVector& vec) {return vec.elapsed;};
}

/*PHIDGET_SETUP_BUFFER_H*/
#endif	