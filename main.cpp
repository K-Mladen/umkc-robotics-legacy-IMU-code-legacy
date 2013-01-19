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

	int timestamp = 0;					//time of current data event in milliSec (debugging)
	double zeroedGyro[3];				//remember zeroed gyro, to keep track of difference
	double zeroedAcc[3];

	CPhidgetSpatial_SpatialEventData* newest;

	spatial::SpatialPVector newestP;	
	//Writing out to file for live graph
	//----------------------------------

	#ifdef DEBUG_LIVE_GRAPH_PHIDGET_RAW_GYRO
		fstream foutPhidgetRawGyro;
		foutPhidgetRawGyro.open("dataPoints/raw_phidget_gyro.csv", fstream::out);
		foutPhidgetRawGyro << "@ Raw Gyro data, non zeroed, avg constant, then zeroed." << endl;
		foutPhidgetRawGyro << "Timestamp" <<
			"X Raw Gyro, 		Y Raw Gyro, 	Z Raw Gyro," << 
			"X Avg Raw Gyro, 	Y Avg Raw Gyro, Z Avg Raw Gyro," << 
			"X Zeroed Raw, 		Y Zeroed Raw, 	Z Zeroed Raw," << endl;
	#endif

	#ifdef DEBUG_LIVE_GRAPH_PHIDGET_RAW_ACC
		fstream foutPhidgetRawAcc;
		foutPhidgetRawAcc.open("dataPoints/raw_phidget_acc.csv", fstream::out);
		foutPhidgetRawAcc << "@ Raw acc data, non zeroed, avg constant, then zeroed." << endl;
		foutPhidgetRawAcc << "Timestamp, " <<
			"X Raw Acc, 		Y Raw Acc, 		Z Raw Acc," << 
			"X Avg Raw Acc, 	Y Avg Raw Acc, 	Z Avg Raw Acc," <<
			"X Zeroed/Rot Acc, 	Y Zeroed/Rot Acc, Z Zeroed/Rot Acc" << 	endl;
	#endif

	#ifdef DEBUG_LIVE_GRAPH_ROTATION
		fstream foutRotation;
		foutRotation.open("dataPoints/rotatedGyro.csv", fstream::out);
		foutRotation << "@ Rotated gyro data, after zeroing." << endl;
		foutRotation << "Timestamp" <<
						"X Gyro rotated, 	Y Gyro rotated, Z Gyro rotated," << 
						"X Diff RotGy, 	Y Diff RotGy, 	Z Diff RotGy, " <<
						"X Diff RotAcc, Y Diff RotAcc, 	Z Diff RotAcc, " << endl;
	#endif

	#ifdef DEBUG_LIVE_GRAPH_DELTA
		fstream foutDelta;
		foutDelta.open("dataPoints/gyroDelta.csv", fstream::out);
		foutDelta << "@ delta of gyroscope in global ref frame, after rotation, before filtering." << endl;
		foutDelta << "Timestamp, X delta, Y delta, Z delta " << endl;
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
		foutRotMatrix << 	"Timestamp "<<
							"[0][0], [0][1], [0][2], " << 
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
#ifdef DEBUG_FAKE_GYRO
	spatial::fake_spatial_setup(spatial, dataQueue, dataRate);
#else
	spatial::spatial_setup(spatial, dataQueue, dataRate);
#endif
	

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
					cout << newest->angularRate[i]  << "/t"; 	
				}
				cout << endl;
			#endif
			

			//Convert data to pVector, rotate to initial reference frame
			//-----------------------------------
			spatial::set(newestP, *newest);	//TESTED AND WORKING TAKE 2
			timestamp = newestP.elapsed;

			#ifdef DEBUG_ZERO_GYRO
				cout << endl << "Before Zeroing" << endl;
				spatial::print(newestP);
			#endif

			#ifndef DEBUG_FAKE_GYRO		
				spatial::zeroGyro(newestP);	//don't want to zero the fake gyro data.
			#endif

			#ifdef DEBUG_ZERO_GYRO
				cout << endl << "After Zeroing" << endl;
				spatial::print(newestP);
			#endif

			#ifdef DEBUG_LIVE_GRAPH_PHIDGET_RAW_GYRO
				//Timestamp
				foutPhidgetRawGyro << timestamp << "/t";
				//Graphing raw gyro
				for(int i =0; i< 3; i++)	{
					foutPhidgetRawGyro << newest->angularRate[i]  << "/t"; 	
				}
				//graphing gyro offset
				for(int i =0; i< 3; i++)	{
					foutPhidgetRawGyro << GYRO_OFFSET[i] << "/t";
				}
				//graphing zeroed raw
				for(int i =0; i< 3; i++)	{
					foutPhidgetRawGyro << newestP.angularRate[i]  << "/t"; 	
			   	}
			   	foutPhidgetRawGyro <<endl;
			#endif	

			

			//remember zeroed gyro
			for(int i = 0; i< 3; i++)	{
				zeroedGyro[i] = newestP.angularRate[i];
				zeroedAcc[i] = newestP.acceleration[i];
			}

			//pVector about = orientation(current);
			

			#ifdef DEBUG_ROTATION_MATRIX
		 		double rotMatrix[3][3]; 
	 			getRotationMatrix(rotMatrix, newestP.angularRate, current);
	 			for(int i =0; i < 3; i++)	{
	 				for(int k = 0; k < 3; k++)	{
	 					foutRotMatrix << rotMatrix[i][k] << "/t";
	 				}
	 			}
	 			foutRotMatrix << endl;
		 	#endif

		 	newestP.angularRate = rotatePOV(newestP.angularRate, current);	
			
			#ifdef DEBUG_LIVE_GRAPH_PHIDGET_RAW_ACC	
			   	//Timestamp
				foutPhidgetRawAcc << timestamp << "/t";
			   	//Graphing raw acc data
				for(int i =0; i< 3; i++)	{
					foutPhidgetRawAcc << newest->acceleration[i]  << "/t"; 	
				}
				//graphing acc offset
				for(int i =0; i< 3; i++)	{
					foutPhidgetRawAcc << ACC_OFFSET[i] << "/t";
				}
				//graphing rotated?
				for(int i =0; i< 3; i++)	{
					foutPhidgetRawAcc << newestP.acceleration[i]  << "/t";	 	
			   	}
				foutPhidgetRawAcc << endl;
			#endif


		 	#ifdef DEBUG_LIVE_GRAPH_ROTATION
				//Graphing timestamp
		 		foutRotation << timestamp << "/t";
		 		//Graphing rotation of gyro
		 		for(int i =0; i< 3; i++)	{
		 			foutRotation << newestP.angularRate[i] << "/t"; 
		 		}
		 		//Graphing difference in rotation of gyro and non rotated gyro
		 		for(int i =0; i< 3; i++)	{
		 			foutRotation << newestP.angularRate[i] - zeroedGyro[i] << "/t"; 
		 		}
		 		//Graphing difference in rotation of acc and non rotated acc
		 		for(int i =0; i< 3; i++)	{
		 			foutRotation << newestP.acceleration[i] - zeroedAcc[i] << "/t"; 
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
			foutDelta << timestamp << "/t";
			for(int i =0; i< 3; i++)	{
				foutDelta << delta.component(i) << "/t";				
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
			foutCurrentOr << timestamp << "/t";
			//nonfiltered orientation
			for(int i =0; i< 3; i++)	{
				foutCurrentOr << current.component(i) << "/t";
			}
		#endif
		
        current = filter(integQueue->at(2).acceleration, current, alpha);				
		
		#ifdef DEBUG_LIVE_GRAPH_CURRENT_ORIENTATION
			//filtered orientation
			for(int i =0; i< 3; i++)	{
				foutCurrentOr << current.component(i) << "/t";
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

	#ifdef DEBUG_LIVE_GRAPH_PHIDGET_RAW_ACC
		foutPhidgetRawAcc.close();
	#endif

	#ifdef DEBUG_LIVE_GRAPH_PHIDGET_RAW_GYRO
		foutPhidgetRawGyro.close();
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
 
