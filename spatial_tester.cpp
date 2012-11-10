/*
A tester file for the phidget.

*/

#include "phidget_setup.h"
#include <iostream>

using namespace std;



int main()	{
	cout << "begin?" <<endl;
	int n = spatial::spatial_setup();
	cout << "returned " << n <<endl;
	cout << "QED" <<endl;
	return 0;
}