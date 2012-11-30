/*
A tester file for the phidget.
polls phidget for data.

SCRAPPED. Can't access timestamp providedd by phidget.
This will mean even MORE inaccurate results if use ros time...
*/

#include "phidget_setup_poll.h"
#include <iostream>
#include "odomvar.h"
#include <stdio.h>

using namespace std;



int main()	{

	//Declare a spatial handle
	CPhidgetSpatialHandle spatial = 0;
	//create the spatial object
	CPhidgetSpatial_create(&spatial);

	
	spatial::spatial_setup(spatial);

	//Polling for data.
	int dataPoints = 1000;
	int index = 0;

	CPhidgetSpatial_SpatialEventData data;
	
	//Getting data rate. Apparently, spatial defaults to 8ms.
	int rate;	//rate in ms
	CPhidgetSpatial_getDataRate(spatial, &rate);

	cout << "Current data rate is : "<< rate <<endl;
	
	//Polls for data

	while(index < dataPoints)	{
		CPhidgetSpatial_getAcceleration(spatial, 0, &(data.acceleration[0]));	
		CPhidgetSpatial_getAcceleration(spatial, 1, &(data.acceleration[1]));	
		CPhidgetSpatial_getAcceleration(spatial, 2, &(data.acceleration[2]));	

		cout << "Getting data point " << index <<endl;
		cout << "X Axis acceleration: " << data.acceleration[0] <<endl;	
		cout << "Y Axis acceleration: " << data.acceleration[1] <<endl;	
		cout << "Z Axis acceleration: " << data.acceleration[2] <<endl;	
	
		printf("Timestamp> seconds: %d -- microseconds: %d\n", data.timestamp.seconds, data.timestamp.microseconds);
		cout << "----------------------------------------------" <<endl <<endl;
		++index;
	}


	//printf("Closing...\n");
	CPhidget_close((CPhidgetHandle)spatial);
	CPhidget_delete((CPhidgetHandle)spatial);

	//cout << raw <<endl;
	//cout << "returned " << n <<endl;
	//cout << "QED" <<endl;
	return 0;
}
