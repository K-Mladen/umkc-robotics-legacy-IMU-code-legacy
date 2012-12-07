/*
Integrating data from the gyro. 


*/

#include "phidget_setup_buffer.h"	//spatial_setup()
#include <iostream>
//#include <stdio.h>
#include <deque>

int event = 0;

using namespace std;

int main()	{


	//Creating/Initializing Spatial Handle
	CPhidgetSpatialHandle spatial = 0;
	CPhidgetSpatial_create(&spatial);
	
	//Pointer to spatial event data; is continually updated by spatial
	//CPhidgetSpatial_SpatialEventData* dataHolder = new CPhidgetSpatial_SpatialEventData;
	CPhidgetSpatial_SpatialEventData d2;

	//Deque of data points. most recent is last one.
	deque<CPhidgetSpatial_SpatialEventData>* dataQueue = new deque<CPhidgetSpatial_SpatialEventData>();
	deque<CPhidgetSpatial_SpatialEventData>::iterator it;

	//How big our buffer is
	dataQueue->resize(5);


	for(it = dataQueue->begin(); it != dataQueue->end(); ++it)	{
	 	//cout << it << endl;
	}
	


	//Open spatial, start pushing data to dataHolder
						//DataRate must be between 4mS and 1S
						//dataRate is an "averaging time" - data is averaged over x mS, and sent every x mS
	int dataRate = 16;	//data at rates faster then 8ms will be delivered to events as an array of data.
	spatial::spatial_setup(spatial, dataQueue, dataRate);


	
	int dataPoints = 1000000;
	int index =0;


	//Busy loop until we have collected enough events.
	//while(event < dataPoints)	{
	while(event < dataPoints) 	{	
		
	}


	//printf("Closing...\n");
	CPhidget_close((CPhidgetHandle)spatial);
	CPhidget_delete((CPhidgetHandle)spatial);

	return 0;
}
