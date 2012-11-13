/********************************************************************************  This code will find the position based on acceleration data using the 	   **	trapezoidal approximation of an integral.								   ********************************************************************************/

#include "odomvar.h"

cartesianCoordData posFromAcc(accField,posField,TimePhidget)
{
	cartesianCoordData posFieldNew(0,0,0,0,0,0);
	static cartesianCoordData accFieldPrevious(0,0,0,0,0,0);
	static double TimePhidgetPrevious = 0;
	double timeElapsed = TimePhidget - TimePhidgetPrevious;
	posFieldNew.x = timeElapsed*(accFieldPrevious.x + accField.x)/2;
	posFieldNew.y = timeElapsed*(accFieldPrevious.y + accField.y)/2;
	PosFieldNew.z = timeElapsed*(accFieldPrevious.z + accField.z)/2;
	posFieldNew.rotx = timeElapsed*(accFieldPrevious.rotx + accField.rotx)/2;
	posFieldNew.roty = timeElapsed*(accFieldPrevious.roty + accField.roty)/2;
	posFieldNew.roty = timeElapsed*(accFieldPrevious.rotz + accField.rotz)/2;

	return posFieldNew;
}
