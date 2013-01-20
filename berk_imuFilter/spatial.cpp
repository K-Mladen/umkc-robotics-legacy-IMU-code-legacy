#include "config.h"
#include "spatial.h"
#include "phidget_Stuff.h"
#include "imuFilter.h"

//Convert from degrees to radians.
#define toRadians(x) (x * 0.01745329252)


extern IMUfilter imuFilter;

//Gyroscope reading in rad/s.
double gyro[3];
//Acc reading in rad/s
double acc[3];

using namespace std;

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


void spatial::print(CPhidgetSpatial_SpatialEventData& data)	{
	
	int elapsed = elapsedTime(data);
	
	std::cout  << elapsed << "\t";
	//std::cout << "Acc " << data.acceleration[0] << " " <<  data.acceleration[1] << " " <<  data.acceleration[2]  << endl;
	std::cout << data.angularRate[0] <<  "\t" << data.angularRate[1] << "\t" << data.angularRate[2] << std::endl;	
	//std::cout << "Mag " << data.magneticField[0] <<  " " << data.magneticField[1] << " " << data.magneticField[2] << endl;	
	std::cout <<std::endl;
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

void spatial::eat(std::deque<CPhidgetSpatial_SpatialEventData>* dataQueue, spatial::PhidgetRawDataQ::iterator it)	{


	while(dataQueue->empty())	{}
	
	pthread_mutex_lock(&mutex);
	it = dataQueue->begin();
	spatial::copyGyAc(it);
	dataQueue->pop_front();
	pthread_mutex_unlock(&mutex);


	imuFilter.updateFilter(gyro[0], gyro[1], gyro [2], acc[0], acc[1], acc[2]);
	imuFilter.computeEuler();
}

void spatial::copyGyAc(spatial::PhidgetRawDataQ::iterator it)	{
	for(int i =0; i< 3; i++)	{
		gyro[i] = toRadians(it->angularRate[i]); 
		acc[i] = it->acceleration[i]; 
	} 

}

int spatial::spatial_setup(CPhidgetSpatialHandle &spatial, std::deque<CPhidgetSpatial_SpatialEventData>* raw, int dataRate)	{
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

	std::cout << "Spatial setup complete" << std::endl;

	return 0;

}
