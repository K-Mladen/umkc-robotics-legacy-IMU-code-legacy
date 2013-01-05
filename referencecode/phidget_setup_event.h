/*
Setting up stuff for the Phidget.

TODO:
Take care of overflow errors.
Documentation on data rate.
*/

#include <stdio.h>
#include <Phidget21/phidget21.h>	//mac os
//#include <phidget21.h>			//linux
#include <iostream>

using namespace std;

namespace spatial	{ 

	int spatial_setup(CPhidgetSpatialHandle &spatial, CPhidgetSpatial_SpatialEventData* raw, int dataRate);

}

//callback that will run if the Spatial is attached to the computer
int CCONV AttachHandler(CPhidgetHandle spatial, void *userptr)
{
	int serialNo;
	CPhidget_getSerialNumber(spatial, &serialNo);
	//printf("Spatial %10d attached!", serialNo);

	return 0;
}

//callback that will run if the Spatial is detached from the computer
int CCONV DetachHandler(CPhidgetHandle spatial, void *userptr)
{
	int serialNo;
	CPhidget_getSerialNumber(spatial, &serialNo);
	printf("Spatial %10d detached! \n", serialNo);

	return 0;
}

//callback that will run if the Spatial generates an error
int CCONV ErrorHandler(CPhidgetHandle spatial, void *userptr, int ErrorCode, const char *unknown)
{
	printf("Error handled. %d - %s \n", ErrorCode, unknown);
	return 0;
}

//callback that will run at datarate
//data - array of spatial event data structures that holds the spatial data packets that were sent in this event
//count - the number of spatial data event packets included in this event
int CCONV SpatialDataHandler(CPhidgetSpatialHandle spatial, void *userptr, CPhidgetSpatial_SpatialEventDataHandle *data, int count)
{

	//Copying over data into external user pointer
	//--------------------
	CPhidgetSpatial_SpatialEventData* dataHolder = (CPhidgetSpatial_SpatialEventData*) userptr;

	//copying timestamp
	dataHolder->timestamp.seconds = data[0]->timestamp.seconds;
	dataHolder->timestamp.microseconds = data[0]->timestamp.microseconds;

	//copying stuff
	for(int i =0; i < 3; i++)	{
		dataHolder->acceleration[i] = data[0]->acceleration[i];
		dataHolder->angularRate[i] = data[0]->angularRate[i];
		dataHolder->magneticField[i] = data[0]->magneticField[i];
	}
	
	

	//Printing out (time in microseconds, acceleration in x axis)

	cout << "DATA from PHIDGET" <<endl;
	cout << "=---------------------------------------------- " <<endl;
	int elapsed = data[0]->timestamp.seconds*1000000 + data[0]->timestamp.microseconds;
	cout << elapsed << endl;
	cout << data[0]->acceleration[0] << " " <<  data[0]->acceleration[1] << " " <<  data[0]->acceleration[2]  << endl;
	cout << data[0]->angularRate[0] <<  " " << data[0]->angularRate[1] << " " << data[0]->angularRate[2] << endl;	
	cout << data[0]->magneticField[0] <<  " " << data[0]->magneticField[1] << " " << data[0]->magneticField[2] << endl;	

	cout <<endl;


	cout << "DATA Copied" <<endl;
	cout << "=---------------------------------------------- " <<endl;
	int copiedElapsed = data[0]->timestamp.seconds*1000000 + data[0]->timestamp.microseconds;
	cout << copiedElapsed << endl;
	cout << dataHolder->acceleration[0] << " " <<  dataHolder->acceleration[1] << " " <<  dataHolder->acceleration[2]  << endl;
	cout << dataHolder->angularRate[0] <<  " " << dataHolder->angularRate[1] << " " << dataHolder->angularRate[2] << endl;	
	cout << dataHolder->magneticField[0] <<  " " << dataHolder->magneticField[1] << " " << dataHolder->magneticField[2] << endl;	

	cout <<endl;

/*
	printf("Acceleration> x: %6f  y: %6f  z: %6f\n", data[0]->acceleration[0], data[0]->acceleration[1], data[0]->acceleration[2]);
	printf("Angular Rate> x: %6f  y: %6f  z: %6f\n", data[0]->angularRate[0], data[0]->angularRate[1], data[0]->angularRate[2]);
	//	if(data[i]->magneticField[0] > 1)
	printf("Magnetic Field> x: %6f  y: %6f  z: %6f\n", data[0]->magneticField[0], data[0]->magneticField[1], data[0]->magneticField[2]);
	printf("Timestamp> seconds: %d -- microseconds: %d\n", data[0]->timestamp.seconds, data[0]->timestamp.microseconds);
*/

	return 0;
}

//Display the properties of the attached phidget to the screen.  
//We will be displaying the name, serial number, version of the attached device, the number of accelerometer, gyro, and compass Axes, and the current data rate
// of the attached Spatial.
int display_properties(CPhidgetHandle phid)
{
	int serialNo, version;
	const char* ptr;
	int numAccelAxes, numGyroAxes, numCompassAxes, dataRateMax, dataRateMin;

	CPhidget_getDeviceType(phid, &ptr);
	CPhidget_getSerialNumber(phid, &serialNo);
	CPhidget_getDeviceVersion(phid, &version);
	CPhidgetSpatial_getAccelerationAxisCount((CPhidgetSpatialHandle)phid, &numAccelAxes);
	CPhidgetSpatial_getGyroAxisCount((CPhidgetSpatialHandle)phid, &numGyroAxes);
	CPhidgetSpatial_getCompassAxisCount((CPhidgetSpatialHandle)phid, &numCompassAxes);
	CPhidgetSpatial_getDataRateMax((CPhidgetSpatialHandle)phid, &dataRateMax);
	CPhidgetSpatial_getDataRateMin((CPhidgetSpatialHandle)phid, &dataRateMin);

	

	printf("%s\n", ptr);
	printf("Serial Number: %10d\nVersion: %8d\n", serialNo, version);
	printf("Number of Accel Axes: %i\n", numAccelAxes);
	printf("Number of Gyro Axes: %i\n", numGyroAxes);
	printf("Number of Compass Axes: %i\n", numCompassAxes);
	printf("datarate> Max: %d  Min: %d\n", dataRateMax, dataRateMin);

	return 0;
}

int spatial::spatial_setup(CPhidgetSpatialHandle &spatial, CPhidgetSpatial_SpatialEventData* raw, int dataRate)	{
	//Code taken from provided example code "Spatial-simple.c"
	int result;
	const char *err;	

	//Set the handlers to be run when the device is plugged in or opened from software, unplugged or closed from software, or generates an error.
	CPhidget_set_OnAttach_Handler((CPhidgetHandle)spatial, AttachHandler, NULL);
	CPhidget_set_OnDetach_Handler((CPhidgetHandle)spatial, DetachHandler, NULL);
	CPhidget_set_OnError_Handler((CPhidgetHandle)spatial, ErrorHandler, NULL);

	//Registers a callback that will run according to the set data rate that will return the spatial data changes
	//Requires the handle for the Spatial, the callback handler function that will be called, 
	//and an arbitrary pointer that will be supplied to the callback function (may be NULL)
	CPhidgetSpatial_set_OnSpatialData_Handler(spatial, SpatialDataHandler, raw);
	//CPhidgetSpatial_set_OnSpatialData_Handler(spatial, SpatialDataHandler, NULL);

	//open the spatial object for device connections
	CPhidget_open((CPhidgetHandle)spatial, -1);

	//get the program to wait for a spatial device to be attached
	//printf("Waiting for spatial to be attached.... \n");
	if((result = CPhidget_waitForAttachment((CPhidgetHandle)spatial, 10000)))
	{
		CPhidget_getErrorDescription(result, &err);
		printf("Problem waiting for attachment: %s\n", err);
		return 0;
	}

	//Display the properties of the attached spatial device
	//display_properties((CPhidgetHandle)spatial);

	//Set the data rate for the spatial events
	CPhidgetSpatial_setDataRate(spatial, dataRate);


	return 0;

}
