/*
Setting up stuff for the Phidget, polling will take place outside.
*/
#include <stdio.h>
#include <Phidget21/phidget21.h>
#include <iostream>
using namespace std;

namespace spatial	{

	int spatial_setup(CPhidgetSpatialHandle &spatial);
}


int spatial::spatial_setup(CPhidgetSpatialHandle &spatial)	{
	//Code taken from provided example code "Spatial-simple.c"
	int result;
	const char *err;

	
	//open the spatial object for device connections
	CPhidget_open((CPhidgetHandle)spatial, -1);

	//get the program to wait for a spatial device to be attached
	printf("Waiting for spatial to be attached.... \n");
	if((result = CPhidget_waitForAttachment((CPhidgetHandle)spatial, 10000)))
	{
		CPhidget_getErrorDescription(result, &err);
		printf("Problem waiting for attachment: %s\n", err);
		return 0;
	}

	return 1;

}
