/*******************************************************************************
*  Structs containing variables for the odometry code.                         **	Note on abbreviations:													   *
*		tp = type 															   *
*		acc = acceleration													   *
*		pos = position														   *
*																			   *
*******************************************************************************/

#ifndef ODOMVAR_H
#define ODOMVAR_H
#include <cmath>

struct tpunit
{
	double x;
	double y;
	double z;
	double rotx;
	double roty;
	double rotz;
	
    void setData(double newx, double newy, double newz,
					double newrotx, double newroty, double newrotz)
    {
        x = newx;
        y = newy;
        z = newz;
        rotx = newrotx;
        roty = newroty;
        rotz = newrotz;
	}

	tpunit()
	{
		x = y = z = rotz = roty = rotx = 0;
	}

	tpunit(double newx, double newy, double newz,
			double newrotx, double newroty, double newrotz)
	{
		x = newx;
		y = newy;
		z = newz;
		rotx = newrotx;
		roty = newroty;
		rotz = newrotz;
	}
};




struct cartesianCoordData
{
	double x;
	double y;
	double z;
	double rotx;
	double roty;
	double rotz;
	double magnitude;
	double rotmagnitude;
	tpunit unit;

	void calculateMagnitude()
	{
		magnitude = sqrt(x*x+y*y+z*z);
		rotmagnitude = sqrt(rotx*rotx + roty*roty + rotz*rotz);
		return;
	}
	void setUnits()
	{
		unit.setData(x/magnitude,y/magnitude,z/magnitude,
					rotx/rotmagnitude,roty/rotmagnitude,rotz/rotmagnitude);
		return;
	}
	cartesianCoordData addComponents(cartesianCoordData addend)
	{
		cartesianCoordData temp;
		temp.x = x + addend.x;
		temp.y = y + addend.y;
		temp.z = z + addend.z;
		temp.rotx = rotx + addend.rotx;
		temp.roty = roty + addend.roty;
		temp.rotz = rotz + addend.rotz;
		return temp;
	}
	cartesianCoordData()
	{
		x = y = z = rotx = roty = rotz = magnitude = rotmagnitude = 0;
		unit.setData(0,0,0,0,0,0);
	}
    cartesianCoordData(double newx, double newy, double newz,
			            double newrotx, double newroty, double newrotz)
	{
		x = newx;
		y = newy;
		z = newz;
		rotx = newrotx;
		roty = newroty;
		rotz = newrotz;
		calculateMagnitude();
		setUnits();
	}

};

/* --turns out structis in phidget21.h header file
//Used to store the raw data from Phidget. 
struct rawPhidget
{
	double accel[3];
	double angRate[3];
	double mag[3];

	CPhidget_Timestamp time;

};*/
#endif
