
#include <iostream>
#include "imuFilter.h"
#include "spatial.h"
#include <fstream>




//Sampling phidget and updating filter , 16mS
#define DATA_RATE 16
//Convert from radians to degrees.
#define toDegrees(x) (x * 57.2957795)
//convert milliseconds to seconds
#define milliToSeconds(x) (x/1000.0)


double gyroscopeErrorRate = .01;

IMUfilter imuFilter(milliToSeconds(DATA_RATE), gyroscopeErrorRate);


//Used with phidget.
extern pthread_mutex_t mutex;



using namespace std;

int main()	{

	fstream foutCurrentOr;
	foutCurrentOr.open("currentOrientation.txt", fstream::out);
	foutCurrentOr << "@Current Orientation (Roll pitch yaw) in degrees" <<endl;
	foutCurrentOr << "Roll, pitch, Yaw" <<endl;

	fstream foutRotMatrix;
	foutRotMatrix.open("rotMatrix.txt", fstream::out);


	double orientation[3];
	double rotMatrix[3][3];

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

	spatial::spatial_setup(spatial, dataQueue, DATA_RATE);

	while(1)	{
		//takes phiget data, calls updateFilter
		spatial::eat(dataQueue, it);

		//getting stuff
		orientation[0] = toDegrees(imuFilter.getRoll());
		orientation[1] = toDegrees(imuFilter.getPitch());
		orientation[2] = toDegrees(imuFilter.getYaw());

		imuFilter.getRotationMatrix(rotMatrix);

		//printing out stuff
		cout << "Rotation matrix" <<endl;
		for(int i =0; i< 3; i++)	{
			cout << "[ ";
			for(int k =0; k<3; k++)	{
				cout << rotMatrix[i][k] << " ";
			}
			cout << "]" << endl;
		}

		cout << "Updated Orientation" <<endl;
		cout << "Roll: " << orientation[0] << endl;
		cout << "Pitch: " << orientation[1] << endl;
		cout << "Yaw: " << orientation[2]<< endl;
		cout << "-------------------------------" << endl;

		//writing stuff to file
		for(int i =0; i<3; i++)	{
			foutCurrentOr <<orientation[i] << ",";
		}
		foutCurrentOr<<endl;

		for(int i =0; i < 3; i++ )	{
			for(int k=0; k<3; k++)	{
				foutRotMatrix << rotMatrix[i][k] << ",";
			} 
		}
		foutRotMatrix <<endl;
	}

	//Odds and Ends.
	//-----------------------------------
	CPhidget_close((CPhidgetHandle)spatial);
	CPhidget_delete((CPhidgetHandle)spatial);

	pthread_mutex_destroy(&mutex);

	foutCurrentOr.close();
	foutRotMatrix.close();


	std::cout << "Good bye!" << std::endl;
	return 0;
}