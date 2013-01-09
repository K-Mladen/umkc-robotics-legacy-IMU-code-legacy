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

	//Creating/Initializing Spatial Handle
	CPhidgetSpatialHandle spatial = 0;
	CPhidgetSpatial_create(&spatial);
	
	//A message queue of sorts. FIFO
	spatial::PhidgetRawDataQ* dataQueue = new spatial::PhidgetRawDataQ();
	spatial::PhidgetRawDataQ::iterator it = dataQueue->begin();

	//A q of 3 PVectors for the Simpson's integration.
	spatial::PVectorQ* integQueue = new spatial::PVectorQ();
//	spatial::PVectorQ::iterator integ = integQueue->begin();
	integQueue->resize(3);

	//Initializing Mutex
	if(pthread_mutex_init(&mutex, NULL) != 0)	{
		printf("mutex init failed");
		//erm if it doesn't work??
		return 1;
	}
	else	{
		printf("mutex init success \n");
	}

	//THIS IS OUR INTIAL ORIENTATION
	pVector initial(0,0,0);
	pVector current(0,0,0);

	//Open spatial, start pushing data to dataHolder
						//DataRate must be between 4mS and 1S
						//dataRate is an "averaging time" - data is averaged over x mS, and sent every x mS
	int dataRate = 16;	//data at rates faster then 8ms will be delivered to events as an array of data.
	spatial::spatial_setup(spatial, dataQueue, dataRate);
	
	double alpha = TAU/(TAU+dataRate);
	//INTEGRATING! YEAH! 

	//doing the first i data points
	for(int i = 0; i<1000; i++)	
	{
				
		//Getting data from Phidget
		while(dataQueue->empty())	{}

		pthread_mutex_lock(&mutex);
		CPhidgetSpatial_SpatialEventData* newest = spatial::copy(*it);
		dataQueue->pop_front();
		it = dataQueue->begin();
		pthread_mutex_unlock(&mutex);

		//Convert data to pVector, rotate to initial reference frame
		spatial::SpatialPVector newestP(*newest);
		rotatePOV(newestP.acceleration, current);
	 	rotatePOV(newestP.angularRate, current);

		//Adding newest point to integQueue for simpson's integration
		integQueue->push_back(*newest);
		integQueue->pop_front();
		//print(*newest);

		//Simpson's integration
		integrateGyro(integQueue, current);
        
		#ifdef DEBUG
			cout << endl << "BEFORE FILTERED" <<endl;
			current.print();		
		#endif

        filter(integQueue->at(2).acceleration, current, alpha);				
		
		#ifdef DEBUG
			cout << endl << "AFTER FILTERED" <<endl;
			current.print();	
			cout << endl;	
		#endif
	}


	//printf("Closing...\n");
	CPhidget_close((CPhidgetHandle)spatial);
	CPhidget_delete((CPhidgetHandle)spatial);

	for(it = dataQueue->begin(); it != dataQueue->end(); ++it)	{
	// 	print(*it);
	}

	pthread_mutex_destroy(&mutex);
	return 0;
}
