/*
Integrating data from the gyro. 

TODO: Some sort of criti section control?
Victoria Wu

*/

#include "headerFiles/phidget_setup_buffer.h"	//spatial_setup()
#include "headerFiles/pVector.h"
#include "headerFiles/mathy.h"

#include <iostream>
#include <pthread.h>	#mutex
//#include <stdio.h>
#include <deque>

//mutexes
pthread_mutex_t mutex;	//used when writing to the deque

using namespace std;

int main()	{


	//Creating/Initializing Spatial Handle
	CPhidgetSpatialHandle spatial = 0;
	CPhidgetSpatial_create(&spatial);
	
	//A message queue of sorts. FIFO
	spatial::PhidgetRawDataQ* dataQueue = new spatial::PhidgetRawDataQ();
	spatial::PhidgetRawDataQ::iterator it = dataQueue->begin();

	//A q of 3 PVectors for the Simpson's integration.
	spatial::PVectorQ* integQueue = new spatial::PVectorQ();
	spatial::PVectorQ::iterator integ = integQueue->begin();
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
	cout << "INITIAL ORIENTATION 0 0 0" <<endl;

	//Open spatial, start pushing data to dataHolder
						//DataRate must be between 4mS and 1S
						//dataRate is an "averaging time" - data is averaged over x mS, and sent every x mS
	int dataRate = 16;	//data at rates faster then 8ms will be delivered to events as an array of data.
	spatial::spatial_setup(spatial, dataQueue, dataRate);

	//INTEGRATING! YEAH! 

	//doing the first i data points
	for(int i = 0; i<1000; i++)	{
		
		//busy wait while q is empty
		//better way to do this?? 
		while(dataQueue->empty())	{}

		//SUCCESS. NOT dropping any packets... but at same time not doing anything either 
		pthread_mutex_lock(&mutex);
		//cout << "SIZE :: " << dataQueue->size() <<endl;
		
		CPhidgetSpatial_SpatialEventData* newest = spatial::copy(*it);

		//Dealing with message q
		dataQueue->pop_front();
		it = dataQueue->begin();
		pthread_mutex_unlock(&mutex);

		// HERE!!! and acceleration too 
		//first find vector to about
		spatial::print(*newest);
		spatial::SpatialPVector newestP(*newest);

		//WAIT i am confused are we rotating about the vector formed by the initial starting vector, or the right now/most c
		//so should it be orientation(initial) or orientation(pVector(0,0,0))
		//make a wrapper method?
		rotatePOV(newestP.acceleration, current);
	 	rotatePOV(newestP.angularRate, current);

		cout << endl<<  "Before Rotating" <<endl;
		// orthog.print();
		cout << endl << "After Rotating" << endl;
		newestP.angularRate.print();

		//adding newest pt into integQ, for integration
		integQueue->push_back(*newest);
		integQueue->pop_front();
		//print(*newest);

		//Simpson's integration, given past 3 data points
		//Looping through each axis
		//double delta[3];
		
//		integrate(spatial::PVectorQ* name, pVector& current);
				
		
		/*
		cout << "UPDATED ORIENTATION " << endl;
		current.print();
		cout << endl;
		*/
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
