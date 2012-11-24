/*******************************************************************************
*  This code will find the position based on acceleration data using the 	   *
*	trapezoidal approximation of an integral.								   *
*******************************************************************************/

#ifndef POSFROMACC_H
#define POSFROMACC_H

#include "odomvar.h"

cartesianCoordData posFromAcc(cartesianCoordData accField, 
							cartesianCoordData posField, int timePhidgetRaw)
{
	//declare const
	const int timePhidgetScale = 1000000;
	//declare objects
	cartesianCoordData posFieldNew(0,0,0,0,0,0), velField(0,0,0,0,0,0),
						dvelField(0,0,0,0,0,0), dposFieldNew (0,0,0,0,0,0);
	static cartesianCoordData accFieldPrevious(0,0,0,0,0,0),
								velFieldPrevious(0,0,0,0,0,0);
	static double timePhidgetPrevious = 0;
	double timePhidget = timePhidgetRaw/timePhidgetScale;
	double timeElapsed = timePhidget - timePhidgetPrevious;


	//math -- acceleration to velocity: Intermediate values ie change in vel.
	dvelField.x = timeElapsed*(accFieldPrevious.x + accField.x)/2;
	dvelField.y = timeElapsed*(accFieldPrevious.y + accField.y)/2;
	dvelField.z = timeElapsed*(accFieldPrevious.z + accField.z)/2;
	dvelField.rotx = timeElapsed*(accFieldPrevious.rotx + accField.rotx)/2;
	dvelField.roty = timeElapsed*(accFieldPrevious.roty + accField.roty)/2;
	dvelField.roty = timeElapsed*(accFieldPrevious.rotz + accField.rotz)/2;	
	//add dvelField to velField
	velField = dvelField.addComponents(velFieldPrevious);

	//math -- velocity to position: Intermediate values ie change in pos.
	dposFieldNew.x = timeElapsed*(velFieldPrevious.x + velField.x)/2;
	dposFieldNew.y = timeElapsed*(velFieldPrevious.y + velField.y)/2;
	dposFieldNew.z = timeElapsed*(velFieldPrevious.z + velField.z)/2;
	dposFieldNew.rotx = timeElapsed*(velFieldPrevious.rotx + velField.rotx)/2;
	dposFieldNew.roty = timeElapsed*(velFieldPrevious.roty + velField.roty)/2;
	dposFieldNew.rotz = timeElapsed*(velFieldPrevious.rotz + velField.rotz)/2;
	posFieldNew = dposFieldNew.addComponents(posField);

	//set "previous" values to "current" to prepare for next iteration
	accFieldPrevious = accField;
	velFieldPrevious = velField;
	timePhidgetPrevious = timePhidget;

	//return position data.
	return posFieldNew;
}

#endif
