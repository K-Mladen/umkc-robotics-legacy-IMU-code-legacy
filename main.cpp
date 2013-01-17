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

#ifdef DEBUG_ROTATION_MATRIX
	#include "animator/rotMatrix.h"
#endif


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

	int timeStamp = 0;					//time of current data event in milliSec (debugging)
	double zeroedGyro[3];				//remember zeroed gyro, to keep track of difference
	double zeroedAcc[3];

	CPhidgetSpatial_SpatialEventData* newest;

	spatial::SpatialPVector newestP;	
	//Writing out to file for live graph
	//----------------------------------

	#ifdef DEBUG_LIVE_GRAPH_PHIDGET_RAW
		fstream foutPhidgetRaw;
		foutPhidgetRaw.open("dataPoints/raw_phidget.csv", fstream::out);
		foutPhidgetRaw << "@ Raw Gyro and Acc data, non zeroed, avg constant, then zeroed." << endl;
		foutPhidgetRaw << 
			"X Raw Gyro, 		Y Raw Gyro, 	Z Raw Gyro," << 
			"X Avg Raw Gyro, 	Y Avg Raw Gyro, Z Avg Raw Gyro," << 
			"X Zeroed Raw, 		Y Zeroed Raw, 	Z Zeroed Raw," << 
			"X Raw Acc, 		Y Raw Acc, 		Z Raw Acc," << 
			"X Avg Raw Acc, 	Y Avg Raw Acc, 	Z Avg Raw Acc," <<
			"X Zeroed/Rot Acc, 	Y Zeroed/Rot Acc, Z Zeroed/Rot Acc" << 	endl;
	#endif

	#ifdef DEBUG_LIVE_GRAPH_ROTATION
		fstream foutRotation;
		foutRotation.open("dataPoints/rotatedGyro.csv", fstream::out);
		foutRotation << "@ Rotated gyro data, after zeroing." << endl;
		foutRotation << "X rotated, 	Y rotated, 		Z rotated," << 
						"X Diff RotGy, 	Y Diff RotGy, 	Z Diff RotGy, " <<
						"X Diff RotAcc, Y Diff RotAcc, 	Z Diff RotAcc, " << endl;
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
		foutCurrentOr << 	"timestamp," << 
							"X Current NF, 	Y Current NF, 	Z Current NF," <<
							"X Current F, 	Y Current F, 	Z Current F " << endl;
	#endif

	#ifdef DEBUG_ROTATION_MATRIX
		fstream foutRotMatrix;
		foutRotMatrix.open("dataPoints/rotation_matrix.csv", fstream::out);
		foutRotMatrix << "@ Rotation Matrix. Used for 3d modeling";
		foutRotMatrix << 	"[0][0], [0][1], [0][2], " << 
							"[1][0], [1][1], [1][2], " << 
							"[2][0], [2][1], [2][2], " <<endl;
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

	

	//GO! 
	while(1)	
	{

		//Get the next two data points (required for simpson's integration)
		for(int i =0; i< 2; i++)	{
			//Getting data from Phidget
			//-----------------------------------
			while(dataQueue->empty())	{}

			//Copied event data is TESTED and WORKING.
			pthread_mutex_lock(&mutex);
	 		it = dataQueue->begin();
			newest = spatial::copy(*it);
			dataQueue->pop_front();
			pthread_mutex_unlock(&mutex);

			#ifdef DEBUG_RAW_GYRO
				cout << "Raw phidget data" << endl;
				for(int i =0; i< 3; i++)	{
					cout << newest->angularRate[i]  << ","; 	
				}
				cout << endl;
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
				//Graphing raw gyro
				for(int i =0; i< 3; i++)	{
					foutPhidgetRaw << newest->angularRate[i]  << ","; 	
				}
				//graphing gyro offset
				for(int i =0; i< 3; i++)	{
					foutPhidgetRaw << GYRO_OFFSET[i] << ",";
				}
				//graphing zeroed raw
				for(int i =0; i< 3; i++)	{
					foutPhidgetRaw << newestP.angularRate[i]  << ","; 	
			   	}
			#endif	

			timeStamp = newestP.elapsed;

			//remember zeroed gyro
			for(int i = 0; i< 3; i++)	{
				zeroedGyro[i] = newestP.angularRate[i];
				zeroedAcc[i] = newestP.acceleration[i];
			}

			//pVector about = orientation(current);
			
			newestP.acceleration = rotatePOV(newestP.acceleration, current);	//WORKING - currently testing
		 	newestP.angularRate = rotatePOV(newestP.angularRate, current);
		 	//newestP.magneticField = rotatePOV(newestP.magneticField, current);
		
		 	#ifdef DEBUG_ROTATION_MATRIX
		 		double rotMatrix[3][3]; 
	 			getRotationMatrix(rotMatrix, newestP.angularRate, current);
	 			for(int i =0; i < 3; i++)	{
	 				for(int k = 0; k < 3; k++)	{
	 					foutRotMatrix << rotMatrix[i][k] << ",";
	 				}
	 			}
	 			foutRotMatrix << endl;
		 	#endif

	//		spatial::zeroAcc(newestP);	//Zeroing of Acc (subtracting gravity) must  be done AFTER rotation???

			
			#ifdef DEBUG_LIVE_GRAPH_PHIDGET_RAW	
			   	//Graphing Acc stuff
				for(int i =0; i< 3; i++)	{
					foutPhidgetRaw << newest->acceleration[i]  << ","; 	
				}
				for(int i =0; i< 3; i++)	{
					foutPhidgetRaw << ACC_OFFSET[i] << ",";
				}
				for(int i =0; i< 3; i++)	{
					foutPhidgetRaw << newestP.acceleration[i]  << ",";	 	
			   	}
				foutPhidgetRaw << endl;
			#endif


		 	#ifdef DEBUG_LIVE_GRAPH_ROTATION
		 		//Graphing rotation of gyro
		 		for(int i =0; i< 3; i++)	{
		 			foutRotation << newestP.angularRate[i] << ","; 
		 		}
		 		//Graphing difference in rotation of gyro and non rotated gyro
		 		for(int i =0; i< 3; i++)	{
		 			foutRotation << newestP.angularRate[i] - zeroedGyro[i] << ","; 
		 		}
		 		//Graphing difference in rotation of acc and non rotated acc
		 		for(int i =0; i< 3; i++)	{
		 			foutRotation << newestP.acceleration[i] - zeroedAcc[i] << ","; 
		 		}
		 		foutRotation << endl;
		 	#endif

			//Adding newest point to integQueue for simpson's integration
			//-----------------------------------
			integQueue->push_back(newestP);
			integQueue->pop_front();
		}


		#ifdef DEBUG_CURRENT_INTEGQUEUE
			for(int k =0; k< 3; k++)	{
				cout << "time: " << integQueue->at(k).elapsed << endl;
				for(int m =0; m < 3; m++)	{
					cout << " gyro: " << integQueue->at(k).angularRate[m] << " ";
				}
				cout << endl;
			}
			cout << "---------------------------"<<endl;
		#endif

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
			foutCurrentOr << timeStamp << ",";
			for(int i =0; i< 3; i++)	{
				foutCurrentOr << current.component(i) << ",";
			}
		#endif
		
        filter(integQueue->at(2).acceleration, current, alpha);				
		



		#ifdef DEBUG_LIVE_GRAPH_CURRENT_ORIENTATION
			for(int i =0; i< 3; i++)	{
				foutCurrentOr << current.component(i) << ",";
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

	#ifdef DEBUG_ROTATION_MATRIX
		foutRotMatrix.close();
	#endif


	return 0;
}
 