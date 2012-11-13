/*******************************************************************************
*  Structs containing variables for the odometry code.                         **	Note on abbreviations:
*		tp = type 
*		acc = acceleration
*		pos = position
*		
*/

#ifndef ODOMVAR_H
#define ODOMVAR_H

struct tpunit
{
	double x;
	double y;
	double z;
	double rotx;
	double roty;
	double rotz;
};

struct accphidget
{
	double x;
	double y;
	double z;
	double rotx;
	double roty;
	double rotz;
	double mag;
	tpunit unit;
};

struct accrob
{
    double x;
    double y;
    double z;
    double rotx;
	double roty;
	double rotz;
	tpunit unit;
};

struct accfield
{
    double x;
    double y;
    double z;
    double rotx;
    double roty;
    double rotz;
	tpunit unit;
};

struct pos
{
	double x;
	double y;
	double z;
	double rotx;
	double roty;
	double rotz;
}

/* --turns out struct is in phidget21.h header file
//Used to store the raw data from Phidget. 
struct rawPhidget
{
	double accel[3];
	double angRate[3];
	double mag[3];

	CPhidget_Timestamp time;

};*/









struct acc2d
{
	double x;
	double y;
	double t;
};

struct vel2d
{
	double x;
	double y;
	double t;
};

#endif
