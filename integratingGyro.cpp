/*
Integrating data from the gyro. 

TODO: Some sort of criti section control?
Victoria Wu
*/


#include "headerFiles/debug.h"
#include "headerFiles/phidget_setup_buffer.h"	//spatial_setup()
#include "headerFiles/pVector.h"
#include "headerFiles/mathy.h"

#include <iostream>
#include <deque>

//mutexes
pthread_mutex_t mutex;	//used when writing to the deque

using namespace std;

int main()	{
	double TAU = 300;
	//THIS IS OUR INTIAL ORIENTATION
	pVector initial(0,0,0);
	pVector current(0,0,0);

	//Creating/Initializing Spatial Handle
	//-----------------------------------
	CPhidgetSpatialHandle spatial = 0;
	CPhidgetSpatial_create(&spatial);
	
	//Setting up queues
	//	dataQ - the Q that phidget events go to
	//	integQ - a Q of three points, used for simpson's iteration
	//-----------------------------------
	spatial::PhidgetRawDataQ* dataQueue = new spatial::PhidgetRawDataQ();
	spatial::PhidgetRawDataQ::iterator it = dataQueue->begin();
	spatial::PVectorQ* integQueue = new spatial::PVectorQ();
	integQueue->resize(3);

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
	int dataRate = 16;	
	spatial::spatial_setup(spatial, dataQueue, dataRate);
	
	double alpha = TAU/(TAU+dataRate);

	//doing the first i data points
	for(int i = 0; i<1000; i++)	
	{
		//Getting data from Phidget
		//-----------------------------------
		while(dataQueue->empty())	{}

		pthread_mutex_lock(&mutex);
		CPhidgetSpatial_SpatialEventData* newest = spatial::copy(*it);
		dataQueue->pop_front();
		it = dataQueue->begin();
		pthread_mutex_unlock(&mutex);

		//Convert data to pVector, rotate to initial reference frame
		//-----------------------------------
		spatial::SpatialPVector newestP(*newest);
		#ifdef DEBUG
			cout << endl << "Before Rotation" << endl;
		#endif
		
		rotatePOV(newestP.acceleration, current);
	 	rotatePOV(newestP.angularRate, current);

	 	#ifdef DEBG
	 		newestP.print();
	 		cout << endl << "After rotation" <<endl;
	 	#endif

		//Adding newest point to integQueue for simpson's integration
		//-----------------------------------
		integQueue->push_back(*newest);
		integQueue->pop_front();
		//print(*newest);

		//Simpson's integration
		//-----------------------------------
		integrateGyro(integQueue, current);
        
		#ifdef DEBUG
			cout << endl << "BEFORE FILTERED" <<endl;
			current.print();		
		#endif

		//Filtering
		//-----------------------------------
        filter(integQueue->at(2).acceleration, current, alpha);				
		
		#ifdef DEBUG
			cout << endl << "AFTER FILTERED" <<endl;
			current.print();	
			cout << endl;	
		#endif
	}


	//Odds and Ends.
	//-----------------------------------
	CPhidget_close((CPhidgetHandle)spatial);
	CPhidget_delete((CPhidgetHandle)spatial);

	pthread_mutex_destroy(&mutex);
	return 0;
}
