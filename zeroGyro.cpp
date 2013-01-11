/*
Zeroing out the gyro...



*/

#include <deque>
#include <fstream>

#include "headerFiles/config.h" //includes debud directives and global constants 
#include "headerFiles/spatial.h"	//spatial_setup()
#include "headerFiles/pVector.h" //pvector class

extern pthread_mutex_t mutex;	//used when writing to the deque

int main()	{

	double gyroSum[3] = {0,0,0};
	double gyroOffset[3] = {0,0,0};
	int events = 5000;

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

	//Setting up Phidget
	//-----------------------------------
	spatial::spatial_setup(spatial, dataQueue, dataRate);

	for(int i = 0; i<events; i++)	
	{
		//Getting data from Phidget
		//-----------------------------------
		while(dataQueue->empty())	{}

			//Copied event data is TESTED and WORKING.
		pthread_mutex_lock(&mutex);
		it = dataQueue->begin();
		CPhidgetSpatial_SpatialEventData* newest = spatial::copy(*it);
		dataQueue->pop_front();
		pthread_mutex_unlock(&mutex);

		for(int i =0; i < 3; i++)	{
			gyroSum[i]= gyroSum[i] + newest->angularRate[i];
		}
		
	}


	//Finding our "zeroes"
	//-----------------------------------
	fstream fout;
	fout.open("gyroOffset.txt", fstream::out);
	for(int i =0; i< 3; i++)	{
		gyroOffset[i] = gyroSum[i]/events;
		cout << "Gyro Offset axis " << i << ": " << gyroOffset[i] << endl;
		fout << "Gyro Offset axis " << i << ": " << gyroOffset[i] << endl;
	}	
	fout.close();

	//Odds and Ends.
	//-----------------------------------
	CPhidget_close((CPhidgetHandle)spatial);
	CPhidget_delete((CPhidgetHandle)spatial);

	pthread_mutex_destroy(&mutex);
	return 0;

}
