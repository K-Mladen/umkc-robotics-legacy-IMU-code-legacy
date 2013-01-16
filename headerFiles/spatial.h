/*
Helper methods to transition from Phidget Events to SpatialPVectors.


*/


#ifndef PHIDGET_SETUP_BUFFER_H
#define PHIDGET_SETUP_BUFFER_H

#include <stdio.h>
#include <deque>

//#include <isthismac.h> //#define s MACOS on mac, if MACOS isn't already defined
//#ifdef MACOS
#include <Phidget21/phidget21.h>	//mac os
//#endif
//#ifndef MACOS
//#include <phidget21.h>			//linux
//#endif

#include <iostream>
#include "pVector.h"


//extern int event;


namespace spatial	{ 

	struct SpatialPVector	{
		int elapsed; 
		pVector acceleration, angularRate, magneticField;
		SpatialPVector(): elapsed(0), 
			acceleration(pVector(0.0203066, -0.0137503, 1.000767)), 
			angularRate(pVector(-0.241803, 0.02563817, -0.3029)), 
			magneticField(pVector())	{}
		
		SpatialPVector(CPhidgetSpatial_SpatialEventData &data) {
			//why isn't this working - undefined symbols D:
			//elapsed = elapsedTime(data);
			elapsed = data.timestamp.seconds*US_PER_SECOND + data.timestamp.microseconds;
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

	void set(SpatialPVector& newestP, CPhidgetSpatial_SpatialEventData &data);
	//Zeroes offset in the Gyro
	void zeroGyro(SpatialPVector &data);
	void zeroAcc(SpatialPVector &data);

	//used for testing
	//fakes a gyro packet of {1,0,0}
	void fakeGyro(SpatialPVector &data, int time);
}

/*PHIDGET_SETUP_BUFFER_H*/
#endif	
