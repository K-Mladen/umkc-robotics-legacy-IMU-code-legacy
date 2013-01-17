#include <iostream>
#include <fstream>
#include <deque>

#include "headerFiles/config.h" //includes debud directives and global constants 
#include "headerFiles/spatial.h"	//spatial_setup()
#include "headerFiles/pVector.h" //pvector class
#include "headerFiles/mathy.h"	 //math related methods,eg rotation integration

using namespace std;

int main ()	{
	
	spatial::PVectorQ* integQueue = new spatial::PVectorQ();
	integQueue->resize(3);

	spatial::SpatialPVector newestP;
	pVector current(0,0,0);
	pVector delta(0,0,0);

	fstream fout;
	fout.open("dataPoints/testingIntegration.csv", fstream::out);
	fout << "@ given a gyro rate, testing integration" << endl;
	fout << "X gyro, Y Gyro, Z Gyro, X Or Delta, Y Or Delta, Z Or Delta, X Current Or, Y Current Or, Z Current Or" << endl;
	spatial::fakeGyro(newestP, 0, 1,1,1);	
	
	for(int i =0; i< 100; i++)	{
		
		cout << "------------------" << i <<endl;
		fout << "------------------" << i << endl;

		spatial::fakeGyro(newestP, i*1000000, 1, 1, 1);	
		integQueue->push_back(newestP);
		integQueue->pop_front();
		//spatial::print(newestP);

		//print current state of integQueue
		for(int k =0; k< 3; k++)	{
			cout << "time: " << integQueue->at(k).elapsed << endl;
			fout << "time: " << integQueue->at(k).elapsed << endl;
			for(int m =0; m < 3; m++)	{
				cout << " gyro: " << integQueue->at(k).angularRate[m] << " ";
				fout << " gyro: " << integQueue->at(k).angularRate[m] << " ";
			}
			cout << endl;
			fout << endl;
		}


		//Simpson's integration
		//-----------------------------------
		if(i%2 ==1 && i > 2)	{	//i < 2 to get rid of first two inaccurate data points
			fout << "INTEGRATING" <<endl;
			cout << "INTEGRATING " <<endl;
			delta = integrateGyro(integQueue, current);
		}

		for(int k = 0; k < 3; k++)	{
			fout << "Delta: " << delta.component(k) << ",";
			cout << "Delta: " << delta.component(k) << ",";
		}
		fout <<endl;
		cout <<endl;

		for(int k = 0; k < 3; k++)	{
			fout << "Current: " <<current.component(k) << ",";
			cout << "Current: " <<current.component(k) << ",";
		}
		fout << endl;
		cout << endl;

			
	
	}

	fout.close();
}