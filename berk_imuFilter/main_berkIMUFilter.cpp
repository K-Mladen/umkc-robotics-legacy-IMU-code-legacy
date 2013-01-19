
#include <iostream>
#include "imuFilter.h"
#include "spatial.h"




//Sampling phidget and updating filter , 16mS
#define DATA_RATE 16
//Convert from radians to degrees.
#define toDegrees(x) (x * 57.2957795)


double gyroscopeErrorRate = .01;

IMUfilter imuFilter(DATA_RATE, gyroscopeErrorRate);


//Used with phidget.
extern pthread_mutex_t mutex;



using namespace std;

int main()	{

	double orientation[3];

	//Creating/Initializing Spatial Handle
	//-----------------------------------
	CPhidgetSpatialHandle spatial = 0;
	CPhidgetSpatial_create(&spatial);
	
	//Setting up queues
	//	dataQ - the Q that phidget events go to
	//-----------------------------------
	spatial::PhidgetRawDataQ* dataQueue = new spatial::PhidgetRawDataQ();
	spatial::PhidgetRawDataQ::iterator it = dataQueue->begin();

	//Initializing Mutex
	//-----------------------------------
	if(pthread_mutex_init(&mutex, NULL) != 0)	{
		printf("mutex init failed");
		//erm if it doesn't work??
		return 1;
	}
	else	{
		printf("mutex init success \n");
	}

	spatial::spatial_setup(spatial, dataQueue, DATA_RATE);

	while(1)	{
		//takes phiget data, calls updateFilter
		spatial::eat(dataQueue, it);

		orientation[0] = toDegrees(imuFilter.getRoll());
		orientation[1] = toDegrees(imuFilter.getPitch());
		orientation[2] = toDegrees(imuFilter.getYaw());

		cout << "Updated Orientation" <<endl;
		cout << "Roll: " << orientation[0] << endl;
		cout << "Pitch: " << orientation[1] << endl;
		cout << "Yaw: " << orientation[2]<< endl;
		cout << "-------------------------------" << endl;

	}

	//Odds and Ends.
	//-----------------------------------
	CPhidget_close((CPhidgetHandle)spatial);
	CPhidget_delete((CPhidgetHandle)spatial);

	pthread_mutex_destroy(&mutex);


	std::cout << "Good bye!" << std::endl;
	return 0;
}