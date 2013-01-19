/*
Helper methods to transition from Phidget Events to SpatialPVectors.


*/


#ifndef PHIDGET_SETUP_BUFFER_H
#define PHIDGET_SETUP_BUFFER_H

#include <stdio.h>
#include <deque>

//#ifdef MACOS
#include <Phidget21/phidget21.h>	//mac os
//#endif
//#ifndef MACOS
//#include <phidget21.h>			//linux
//#endif

#include <iostream>

//extern int event;


namespace spatial	{ 

	

	typedef std::deque<CPhidgetSpatial_SpatialEventData> PhidgetRawDataQ; 

	//data rate in milliseconds, must be between 4ms and 1s
	int spatial_setup(CPhidgetSpatialHandle &spatial, std::deque<CPhidgetSpatial_SpatialEventData>* raw, int dataRate );
	int fake_spatial_setup(CPhidgetSpatialHandle &spatial, std::deque<CPhidgetSpatial_SpatialEventData>* raw, int dataRate );

	void print(CPhidgetSpatial_SpatialEventData& data);
	CPhidgetSpatial_SpatialEventData* copy(CPhidgetSpatial_SpatialEventData& spatial);
	int elapsedTime(CPhidgetSpatial_SpatialEventData& spatial);

	//takes 
	void eat(std::deque<CPhidgetSpatial_SpatialEventData>* dataQueue, spatial::PhidgetRawDataQ::iterator it);
	void copyGyAc(spatial::PhidgetRawDataQ::iterator it);

	//used for testing
	//fakes a gyro packet of {1,0,0}, time in microseconds
	void fakeGyro(CPhidgetSpatial_SpatialEventData &data, int time, double xVal, double yVal, double zVal);	
}

/*PHIDGET_SETUP_BUFFER_H*/
#endif	
