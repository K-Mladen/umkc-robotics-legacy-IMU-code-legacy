/*******************************************************************************
*  This code will find the position based on acceleration data using the 	   *
*	trapezoidal approximation of an integral.								   *
*******************************************************************************/
//TODO overload addition operator for cartesianCoordData in odomvar.h!

#include "odomvar.h"

cartesianCoordData posFromAcc(accField,posField,timePhidget)
{
	//declare objects
	cartesianCoordData posFieldNew(0,0,0,0,0,0), velField(0,0,0,0,0,0);
	static cartesianCoordData accFieldPrevious(0,0,0,0,0,0),
								velFieldPrevious(0,0,0,0,0,0);
	static double timePhidgetPrevious = 0;
	double timeElapsed = timePhidget - timePhidgetPrevious;

	//math -- acceleration to velocity: Intermediate values ie change in vel.
	velField.x = timeElapsed*(accFieldPrevious.x + accField.x)/2;
	velField.y = timeElapsed*(accFieldPrevious.y + accField.y)/2;
	velField.z = timeElapsed*(accFieldPrevious.z + accField.z)/2;
	velField.rotx = timeElapsed*(accFieldPrevious.rotx + accField.rotx)/2;
	velField.roty = timeElapsed*(accFieldPrevious.roty + accField.roty)/2;
	velField.roty = timeElapsed*(accFieldPrevious.rotz + accField.rotz)/2;	
	velField+=velFieldPrevious; //add vel pre-change to get current velocity.

	//math -- velocity to position: Intermediate values ie change in pos.
	posFieldNew.x = timeElapsed*(velFieldPrevious.x + velField.x)/2;
	posFieldNew.y = timeElapsed*(velFieldPrevious.y + velField.y)/2;
	posFieldNew.z = timeElapsed*(velFieldPrevious.z + velField.z)/2;
	posFieldNew.rotx = timeElapsed*(velFieldPrevious.rotx + velField.rotx)/2;
	posFieldNew.roty = timeElapsed*(velFieldPrevious.roty + velField.roty)/2;
	posFieldNew.rotz = timeElapsed*(velFieldPrevious.rotz + velField.rotz)/2;
	posFieldNew+=posField; //add pre-change position to get final position.

	//set "previous" values to "current" to prepare for next iteration
	accFieldPrevious = accField;
	velFieldPrevious = velField;
	timePhidgetPrevious = timePhidget;

	//return position data.
	return posFieldNew;
}
