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
extern pthread_mutex_t mutex;	//used when writing to the deque

using namespace std;

int main()	{

	//Constants
	//-----------------------------------
	
    double alpha = TAU/(TAU+dataRate);	

	pVector initial(0,0,0);				//initial, arbitray orientation
	pVector current(0,0,0);				//current orientation
	pVector delta(0,0,0);				//delta, (debugging)

	spatial::SpatialPVector newestP;	
	//Writing out to file for live graph
	//----------------------------------

	#ifdef DEBUG_LIVE_GRAPH_PHIDGET_RAW
		fstream foutPhidgetRaw;
		foutPhidgetRaw.open("dataPoints/raw_phidget.csv", fstream::out);
		foutPhidgetRaw << "@ Raw Phidget data, non zeroed, avg constant, then zeroed." << endl;
		foutPhidgetRaw << "X Raw Gyro, Y Raw Gyro, Z Raw Gyro," 
				<< "X Avg Raw Gyro, Y Avg Raw Gyro, Z Avg Raw Gyro," << 
			 "X Zeroed Raw, Y Zeroed Raw, Z Zeroed Raw" << endl;
	#endif

	#ifdef DEBUG_LIVE_GRAPH_ROTATION
		fstream foutRotation;
		foutRotation.open("dataPoints/rotatedGyro.csv", fstream::out);
		foutRotation << "@ Rotated gyro data, after zeroing." << endl;
		foutRotation << " X rotated, Y rotated, Z rotated" << endl;
	#endif

	#ifdef DEBUG_LIVE_GRAPH_DELTA
		fstream foutDelta;
		foutDelta.open("dataPoints/gyroDelta.csv", fstream::out);
		foutDelta << "@ delta of gyroscope in global ref frame, after rotation, before filtering." << endl;
		foutDelta << "X delta, Y delta, Z delta " << endl;
	#endif


	#ifdef DEBUG_LIVE_GRAPH_CURRENT_ORIENTATION
		fstream foutCurrentOr;
		foutCurrentOr.open("dataPoints/current_orientation.csv", fstream::out);
		foutCurrentOr << "@ Current Orientation, rotated, then non filtered and filtered." << endl;
		foutCurrentOr << "X Current NF, Y Current NF, Z Current NF, X Current F, Y Current F, Z Current F " << endl;
	#endif


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
	for(int i = 0; i<100000; i++)	
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

		#ifdef DEBUG_RAW_GYRO
			cout << "Raw phidget data" << endl;
			for(int i =0; i< 3; i++)	{
				cout << newest->angularRate[i]  << ","; 	
			}
			cout << endl;
		#endif
		
		#ifdef DEBUG_LIVE_GRAPH_PHIDGET_RAW
			cout << endl << "LIVE GRAPHING raw phidget" << endl;
			for(int i =0; i< 3; i++)	{
				foutPhidgetRaw << newest->angularRate[i]  << ","; 	
			}
			for(int i =0; i< 3; i++)	{
				foutPhidgetRaw << GYRO_OFFSET[i] << ",";
			}
		#endif

		//Convert data to pVector, rotate to initial reference frame
		//-----------------------------------
		spatial::set(newestP, *newest);	//TESTED AND WORKING TAKE 2

		#ifdef DEBUG_ZERO_GYRO
			cout << endl << "Before Zeroing" << endl;
			spatial::print(newestP);
		#endif

		spatial::zeroGyro(newestP);

		#ifdef DEBUG_ZERO_GYRO
			cout << endl << "After Zeroing" << endl;
			spatial::print(newestP);
		#endif

		#ifdef DEBUG_LIVE_GRAPH_PHIDGET_RAW
			for(int i =0; i< 3; i++)	{
				foutPhidgetRaw << newestP.angularRate[i]  << ","; 	
		   	}		
			foutPhidgetRaw << endl;
		#endif

		pVector about = orientation(current);
		
		newestP.acceleration = rotatePOV(newestP.acceleration, about);	//WORKING - currently testing
	 	newestP.angularRate = rotatePOV(newestP.angularRate, about);
	 	newestP.magneticField = rotatePOV(newestP.magneticField, about);

	 	#ifdef DEBUG_LIVE_GRAPH_ROTATION
	 		for(int i =0; i< 3; i++)	{
	 			foutRotation << newestP.angularRate[i] << ","; 
	 		}
	 		foutRotation << endl;
	 	#endif

		//Adding newest point to integQueue for simpson's integration
		//-----------------------------------

		integQueue->push_back(newestP);
		integQueue->pop_front();
		
		//Simpson's integration
		//-----------------------------------
		delta = integrateGyro(integQueue, current);


		#ifdef DEBUG_LIVE_GRAPH_DELTA
			for(int i =0; i< 3; i++)	{
				foutDelta << delta.component(i) << ",";				
			}
			foutDelta << endl;
		#endif

		#ifdef DEBUG_INTEGRATE
			cout<< endl << "AFTER INTEGRATION, current orientation" <<endl;
			current.print();
			cout << endl;
		#endif

		//Filtering
		//-----------------------------------

		#ifdef DEBUG_FILTERING
			cout << endl << "BEFORE FILTERED" <<endl;
			current.print();		
		#endif

		#ifdef DEBUG_LIVE_GRAPH_CURRENT_ORIENTATION
			cout << endl << "LIVE GRAPHING unfiltered current orientation" << endl;
			for(int i =0; i< 3; i++)	{
				foutCurrentOr << current.component(i) << ",";
				cout << current.component(i) << ","; 	
			}
		#endif
		
        filter(integQueue->at(2).magneticField, current, alpha);				
		



		#ifdef DEBUG_LIVE_GRAPH_CURRENT_ORIENTATION
			cout << endl << "LIVE GRAPHING filtered current orientation" << endl;
			for(int i =0; i< 3; i++)	{
				foutCurrentOr << current.component(i) << ",";
				cout << current.component(i) << ","; 	
			}
			foutCurrentOr << endl;
		#endif

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
	
	#ifdef DEBUG_LIVE_GRAPH_CURRENT_ORIENTATION
		foutCurrentOr.close();
	#endif

	#ifdef DEBUG_LIVE_GRAPH_PHIDGET_RAW
		foutPhidgetRaw.close();
	#endif

	#ifdef DEBUG_LIVE_GRAPH_ROTATION
		foutRotation.close();
	#endif

	#ifdef DEBUG_LIVE_GRAPH_DELTA
		foutDelta.close();
	#endif

	return 0;
}
 