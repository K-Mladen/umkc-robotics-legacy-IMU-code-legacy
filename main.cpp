/*
Integrating data from the gyro. 

TODO: Some sort of criti section control?

*/



#include <iostream>
#include <fstream>
#include <deque>

#include "headerFiles/config.h" //includes debud directives and global constants 
#include "headerFiles/spatial.h"	//spatial_setup()
#include "headerFiles/pVector.h" //pvector class
#include "headerFiles/mathy.h"	 //math related methods,eg rotation integration


//mutexes
pthread_mutex_t mutex;	//used when writing to the deque

using namespace std;

int main()	{

	//Constants
	//-----------------------------------
	int dataRate = 16; 					//milliSeconds
	double TAU = 300;					//used to calculate alpha
    double alpha = TAU/(TAU+dataRate);	

	pVector initial(0,0,0);				//initial, arbitray orientation
	pVector current(0,0,0);				//current orientation

/*	ifstream cfg;
	if(cfg.open("odometryConstants.cfg");)
	{
	cfg 
	>> dataRate
	>> TAU;
	cfg.close();
	} else {
	//default values
	datarate = 16;
	TAU = 300;
	}
*/

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
	spatial::spatial_setup(spatial, dataQueue, dataRate);

	//doing the first i data points
	for(int i = 0; i<1; i++)	
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


		//Convert data to pVector, rotate to initial reference frame
		//-----------------------------------
		spatial::SpatialPVector newestP(*newest);	//TESTED AND WORKING

		#ifdef DEBUG_ROTATION
			cout << endl << "Before Rotation" << endl;
			spatial::print(newestP);
			cout <<endl;
		#endif
		
		cout << "TESTING FROM OUTSIDE, parameters" << endl << "ABOUT" ;
		current.print();
		cout << endl;
		newestP.acceleration = rotatePOV(newestP.acceleration, current);	//NOT WORKING - giving me NAN
	 	newestP.angularRate = rotatePOV(newestP.angularRate, current);

	 	#ifdef DEBUG_ROTATION
	 		cout << endl << "After rotation" <<endl;
	 		spatial::print(newestP);
	 	#endif

		//Adding newest point to integQueue for simpson's integration
		//-----------------------------------

		integQueue->push_back(newestP);
		integQueue->pop_front();
		//print(*newest);


		//Simpson's integration
		//-----------------------------------
		integrateGyro(integQueue, current);
        


		//Filtering
		//-----------------------------------

		#ifdef DEBUG_FILTERING
			cout << endl << "BEFORE FILTERED" <<endl;
			current.print();		
		#endif
        
        filter(integQueue->at(2).acceleration, current, alpha);				
		
		#ifdef DEBUG_FILTERING
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
