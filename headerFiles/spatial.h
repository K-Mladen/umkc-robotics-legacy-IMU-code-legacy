/*
Helper methods to transition from Phidget Events to SpatialPVectors.


*/


#ifndef PHIDGET_SETUP_BUFFER_H
#define PHIDGET_SETUP_BUFFER_H

#include <stdio.h>
#include <deque>

//#include <isthismac.h> //#define's MACOS on mac, if MACOS isn't already defined
//#ifdef MACOS
//#include <Phidget21/phidget21.h>	//mac os
//#else
#include <phidget21.h>			//linux
//#endif

#include <iostream>
#include "pVector.h"


//extern int event;


namespace spatial	{ 

	struct SpatialPVector	{
		int elapsed; 
		pVector acceleration, angularRate, magneticField;
		SpatialPVector(): elapsed(0), acceleration(pVector()), angularRate(pVector()), magneticField(pVector())	{}
		
		SpatialPVector(CPhidgetSpatial_SpatialEventData &data) {
			//why isn't this working - undefined symbols D:
			//elapsed = elapsedTime(data);
			elapsed = data.timestamp.seconds*1000000 + data.timestamp.microseconds;
			acceleration.set(data.acceleration);
			angularRate.set(data.angularRate);
			magneticField.set(data.magneticField);
		}
/*
		rotate(SpatialPVector& spatial, pVector about)	{

		}	*/
	};

	typedef deque<CPhidgetSpatial_SpatialEventData> PhidgetRawDataQ; 
	typedef deque<SpatialPVector> PVectorQ;

	//data rate in milliseconds, must be between 4ms and 1s
	int spatial_setup(CPhidgetSpatialHandle &spatial, deque<CPhidgetSpatial_SpatialEventData>* raw, int dataRate );
	void print(CPhidgetSpatial_SpatialEventData& data);
	void print(SpatialPVector& data);
	CPhidgetSpatial_SpatialEventData* copy(CPhidgetSpatial_SpatialEventData& spatial);
	int elapsedTime(CPhidgetSpatial_SpatialEventData& spatial);
	int elapsedTimePVec(SpatialPVector& vec);

	//Zeroes offset in the Gyro
	void zeroGyro(SpatialPVector &data);
}

/*PHIDGET_SETUP_BUFFER_H*/
#endif	
