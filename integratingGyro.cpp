/*
Integrating data from the gyro. 

TODO: Some sort of criti section control?
Victoria Wu

*/

#include "headerFiles/pVector.h"
#include "headerFiles/mathy.h"
#include "phidget_setup_buffer.h"	//spatial_setup()
#include <iostream>
#include <pthread.h>	#mutex
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
	pVector initial = pVector();
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
		
		CPhidgetSpatial_SpatialEventData* newest = copy(*it);

		//Dealing with message q
		dataQueue->pop_front();
		it = dataQueue->begin();
		pthread_mutex_unlock(&mutex);

		// HERE!!! and acceleration too 
		//first find vector to about
		spatial::SpatialPVector newestP(*newest);
		//WAIT i am confused are we rotating about the vector formed by the initial starting vector, or the right now/most c
		//so should it be orientation(initial) or orientation(pVector(0,0,0))
		//make a wrapper method?
		pVector orthog = orientation(initial);
		newestP.acceleration = rotatePOV(newestP.acceleration, orthog);
		newestP.angularRate = rotatePOV(newestP.angularRate, orthog);

		//adding newest pt into integQ, for integration
		integQueue->push_back(*newest);
		integQueue->pop_front();
		//print(*newest);

		//Simpson's integration, given past 3 data points
		//Looping through each axis
		double delta[3];
		double angularRate[3][3];	//[x][y] xth point, yth axis
		double dTime[3];

		double acc[3];
		pVector phidgetPOV[3];

		for(int i =0; i<3; i++)	{
			for(int k = 0; k<3; k++)	{
				angularRate[i][k] = integQueue->at(i).angularRate[k];
	//			cout << "Getting angular rate" << endl;
	//			cout << first << " "  <<second << " " << third << endl;
			}
			pVector p;
			//pVector p(angularRate[i][0], angularRate[i][1], angularRate[i][2]);
			//phidgetPOV[i] = p;

			dTime[i] = elapsedTime(integQueue->at(i));
//			cout << " Time points "  <<  endl;
//			cout << time1 << " " << time2 << " " << time3 << endl;


			//delta[i] = simpsonIteration(first, second, third,time1, time2, time3);			
	//		cout << "DELTA == " << delta[i] <<endl;
				
			//ANGLE COMPLEMENTARY FILTER to the RESCUE
			//so now orientation[i] = angle + gyro*dt
			/*
			orientation[i] += delta[i]; 
			double alpha = 0.98;
			orientation[i] = alpha*orientation[i] + (1-alpha)*acc[i];
			*/	
		}	
			
			
		
		
		
		
		/*
		cout << "UPDATED ORIENTATION " << endl;

		for(int i =0; i< 3; i++)	{
			cout << orientation[i] << " "; 		
		}
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
