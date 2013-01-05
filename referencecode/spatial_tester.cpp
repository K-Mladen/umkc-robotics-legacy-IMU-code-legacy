/*
A tester file for the phidget.

*/

#include "phidget_setup_event.h"
#include <iostream>
#include "odomvar.h"
#include <stdio.h>

using namespace std;

int event;

int main()	{

	double raw=999;

	event =0;
	//Declare a spatial handle
	CPhidgetSpatialHandle spatial = 0;
	//create the spatial object
	CPhidgetSpatial_create(&spatial);

	
	int n = spatial::spatial_setup(spatial);

	int dataPoints = 1000000;
	int index =0;

	//
	while(event < dataPoints)	{

		//CPhidgetSpatial_SpatialEventData.acceleration[0];
		
/*
		printf("Acceleration> x: %6f  y: %6f  x: %6f\n", raw.accel[0], raw.accel[1], raw.accel[2]);
		printf("Angular Rate> x: %6f  y: %6f  x: %6f\n", raw.angRate[0], raw.angRate[1], raw.angRate[2]);
		//	if(data[i]->magneticField[0] > 1)
		printf("Magnetic Field> x: %6f  y: %6f  x: %6f\n", raw.mag[0], raw.mag[1], raw.mag[2]);
		printf("Timestamp> seconds: %d -- microseconds: %d\n", raw.time.seconds, raw.time.microseconds);
		
*/		

		//printf("---------------------------------------------\n");
	}


	//printf("Closing...\n");
	CPhidget_close((CPhidgetHandle)spatial);
	CPhidget_delete((CPhidgetHandle)spatial);

	//cout << raw <<endl;
	//cout << "returned " << n <<endl;
	//cout << "QED" <<endl;
	return 0;
}
