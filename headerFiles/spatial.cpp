#include "config.h"
#include "spatial.h"
#include  "phidget_Stuff.h"



void spatial::zeroGyro(SpatialPVector &data)	{
	for(int i =0; i< 3; i++)	{
		data.angularRate[i] = data.angularRate[i] - GYRO_OFFSET[i];	
	}
}

void spatial::zeroAcc(SpatialPVector &data)	{
	for(int i =0; i< 3; i++)	{
		data.acceleration[i] = data.acceleration[i] - ACC_OFFSET[i];
	}
}

int spatial::elapsedTimePVec(SpatialPVector& vec)	{
	return vec.elapsed;
}

//Returns elapsed time in Microseconds
int spatial::elapsedTime(CPhidgetSpatial_SpatialEventData& data)	{
	return (int)(data.timestamp.seconds*US_PER_SECOND + data.timestamp.microseconds);
}

CPhidgetSpatial_SpatialEventData* spatial::copy(CPhidgetSpatial_SpatialEventData &other )	{

	CPhidgetSpatial_SpatialEventData* dataHolder = (CPhidgetSpatial_SpatialEventData*)malloc(sizeof(CPhidgetSpatial_SpatialEventData));

	//copying timestamp
	dataHolder->timestamp.seconds = other.timestamp.seconds;
	dataHolder->timestamp.microseconds = other.timestamp.microseconds;

	//copying stuff
	for(int i =0; i < 3; i++)	{
		dataHolder->acceleration[i] = other.acceleration[i];
		dataHolder->angularRate[i] = other.angularRate[i];
		dataHolder->magneticField[i] = other.magneticField[i];
	}

	return dataHolder;
}

void spatial::print(SpatialPVector& data)	{
	cout << "SPATIAL P VECTOR" <<endl;
	cout  << data.elapsed << "\t";
	//cout << "Acc " << data.acceleration[0] << " " <<  data.acceleration[1] << " " <<  data.acceleration[2]  << endl;
	cout << data.angularRate[0] <<  "\t" << data.angularRate[1] << "\t" << data.angularRate[2] << endl;	
	//cout << "Mag " << data.magneticField[0] <<  " " << data.magneticField[1] << " " << data.magneticField[2] << endl;	
	cout <<endl;
}

void spatial::print(CPhidgetSpatial_SpatialEventData& data)	{
	
	int elapsed = elapsedTime(data);
	
	cout  << elapsed << "\t";
	//cout << "Acc " << data.acceleration[0] << " " <<  data.acceleration[1] << " " <<  data.acceleration[2]  << endl;
	cout << data.angularRate[0] <<  "\t" << data.angularRate[1] << "\t" << data.angularRate[2] << endl;	
	//cout << "Mag " << data.magneticField[0] <<  " " << data.magneticField[1] << " " << data.magneticField[2] << endl;	
	cout <<endl;
}

void spatial::set(SpatialPVector& newestP, CPhidgetSpatial_SpatialEventData &data)	{
	newestP.elapsed = data.timestamp.seconds*US_PER_SECOND + data.timestamp.microseconds;
	newestP.acceleration.set(data.acceleration);	
	newestP.angularRate.set(data.angularRate);
	newestP.magneticField.set(data.magneticField);
}

void spatial::fakeGyro(CPhidgetSpatial_SpatialEventData &data, int time, double xVal, double yVal, double zVal)	{
	data.timestamp.microseconds = time;
	data.timestamp.seconds = 0;
	double val[3] = {xVal, yVal, zVal};
	for(int i =0; i<3; i++)	{
		data.acceleration[i] = val[i];
		data.angularRate[i] = val[i];
		data.magneticField[i] = val[i];
	}
}

void spatial::fakeGyro(SpatialPVector &data, int time, double xVal, double yVal, double zVal)	{
	data.elapsed = time;
	double thing[3]= {xVal,yVal,zVal};
	data.acceleration.set(thing);
	data.angularRate.set(thing);
	data.magneticField.set(thing);

}

int spatial::spatial_setup(CPhidgetSpatialHandle &spatial, deque<CPhidgetSpatial_SpatialEventData>* raw, int dataRate)	{
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
	printf("Waiting for spatial to be attached.... \n");

	if((result = CPhidget_waitForAttachment((CPhidgetHandle)spatial, 10000)))
	{
		CPhidget_getErrorDescription(result, &err);
		printf("Problem waiting for attachment: %s\n", err);
		return 0;
	}

	//Display the properties of the attached spatial device
	display_properties((CPhidgetHandle)spatial);

	//Set the data rate for the spatial events
	CPhidgetSpatial_setDataRate(spatial, dataRate);

	cout << "Spatial setup complete" << endl;

	return 0;

}

#ifdef DEBUG_FAKE_GYRO
int spatial::fake_spatial_setup(CPhidgetSpatialHandle &spatial, deque<CPhidgetSpatial_SpatialEventData>* raw, int dataRate)	{
	
	cout << "========================================================= " << endl;
	cout << "FAKE SPATIAL SETUP " << endl;
	cout << "========================================================= " << endl;
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
	CPhidgetSpatial_set_OnSpatialData_Handler(spatial, FAKE_SpatialDataHandler, raw);

	//open the spatial object for device connections_setp
	CPhidget_open((CPhidgetHandle)spatial, -1);

	//get the program to wait for a spatial device to be attached
	printf("Waiting for spatial to be attached.... \n");

	if((result = CPhidget_waitForAttachment((CPhidgetHandle)spatial, 10000)))
	{
		CPhidget_getErrorDescription(result, &err);
		printf("Problem waiting for attachment: %s\n", err);
		return 0;
	}

	//Display the properties of the attached spatial device
	display_properties((CPhidgetHandle)spatial);

	//Set the data rate for the spatial events
	CPhidgetSpatial_setDataRate(spatial, dataRate);

	cout << "Spatial setup complete" << endl;

	return 0;

}
#endif
