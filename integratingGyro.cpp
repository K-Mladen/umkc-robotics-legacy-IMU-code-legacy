/*
Integrating data from the gyro. 

TODO: Some sort of criti section control?
Victoria Wu

*/

#include "phidget_setup_buffer.h"	//spatial_setup()
#include <iostream>
#include <pthread.h>	#mutex
#include "simpsonIteration.h"
//#include <stdio.h>
#include <deque>
//#include <complimentaryFilter.h>  

//int event =0;

//mutexes
pthread_mutex_t mutex;	//used when writing to the deque

using namespace std;

int main()	{


	//Creating/Initializing Spatial Handle
	CPhidgetSpatialHandle spatial = 0;
	CPhidgetSpatial_create(&spatial);
	
	//A message queue of sorts. FIFO
	deque<CPhidgetSpatial_SpatialEventData>* dataQueue = new deque<CPhidgetSpatial_SpatialEventData>();
	deque<CPhidgetSpatial_SpatialEventData>::iterator it = dataQueue->begin();

	//A 3 unit q for the Simpson's integration.
	deque<CPhidgetSpatial_SpatialEventData>* integQueue = new deque<CPhidgetSpatial_SpatialEventData>();
	deque<CPhidgetSpatial_SpatialEventData>::iterator integ = integQueue->begin();
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
	double orientation[3] = {0,0,0};
	cout << "INITIAL ORIENTATION 0 0 0" <<endl;

	//Open spatial, start pushing data to dataHolder
						//DataRate must be between 4mS and 1S
						//dataRate is an "averaging time" - data is averaged over x mS, and sent every x mS
	int dataRate = 16;	//data at rates faster then 8ms will be delivered to events as an array of data.
	spatial::spatial_setup(spatial, dataQueue, dataRate);

	//INTEGRATING! YEAH! 

	for(int i = 0; i<1000; i++)	{
		
		//busy wait while q is empty
		while(dataQueue->empty())	{}

		//SUCCESS. NOT dropping any packets... but at same time not doing anything either 
		pthread_mutex_lock(&mutex);
		//cout << "SIZE :: " << dataQueue->size() <<endl;
		
		CPhidgetSpatial_SpatialEventData* newest = copy(*it);

		//Dealing with message q
		dataQueue->pop_front();
		it = dataQueue->begin();
		pthread_mutex_unlock(&mutex);


		//adding newest pt into integQ, for integration
		integQueue->push_back(*newest);
		integQueue->pop_front();
//		print(*newest);

		//Simpson's integration, given past 3 data points
		//Looping through each axis
		double delta[3];
		for(int i =0; i<3; i++)	{
			double first = integQueue->at(0).angularRate[i];
			double second = integQueue->at(1).angularRate[i];
			double third = integQueue->at(2).angularRate[i];
//			cout << "Getting angular rate" << endl;
//			cout << first << " "  <<second << " " << third << endl;
			
			double time1 = elapsedTime(integQueue->at(0));
			double time2 = elapsedTime(integQueue->at(1));
			double time3 = elapsedTime(integQueue->at(2));
//			cout << " Time points "  <<  endl;
//			cout << time1 << " " << time2 << " " << time3 << endl;
			
			//rotation would go here
			pVector vec()
			rotatePOV()
			
			
			delta[i] = simpsonIteration(first, second, third,time1, time2, time3);			
//			cout << "DELTA == " << delta[i] <<endl;
			
			//ANGLE COMPLEMENTARY FILTER to the RESCUE
			
			
			
			//so now orientation[i] = angle + gyro*dt
			orientation[i] += delta[i]; 
			double alpha = 0.98;
			orientation[i] = alpha*orientation[i] + (1-alpha)*acc[i];
			
		}
		
		
		
		cout << "UPDATED ORIENTATION " << endl;

		for(int i =0; i< 3; i++)	{
			cout << orientation[i] << " "; 		
		}
		cout << endl;

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
