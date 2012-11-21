/*
A tester file for the phidget.

*/

#include "phidget_setup_event.h"	//spatial_setup()
#include <iostream>
#include <stdio.h>

using namespace std;

int main()	{


	//Creating/Initializing Spatial Handle
	CPhidgetSpatialHandle spatial = 0;
	CPhidgetSpatial_create(&spatial);
	
	//Pointer to spatial event data; is continually updated by spatial
	CPhidgetSpatial_SpatialEventData* dataHolder = new CPhidgetSpatial_SpatialEventData;

	//Open spatial, start pushing data to dataHolder
						//DataRate must be between 4mS and 1S
						//dataRate is an "averaging time" - data is averaged over x mS, and sent every x mS
	int dataRate = 16;	//data at rates faster then 8ms will be delivered to events as an array of data.
	spatial::spatial_setup(spatial, dataHolder, dataRate);


	int event = 0;
	int dataPoints = 1000000;
	int index =0;

	//Busy loop until we have collected enough events.
	while(event < dataPoints)	{
		event++;
	}


	//printf("Closing...\n");
	CPhidget_close((CPhidgetHandle)spatial);
	CPhidget_delete((CPhidgetHandle)spatial);

	return 0;
}
