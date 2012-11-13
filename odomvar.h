#ifndef ODOMVAR_H
#define ODOMVAR_H

struct acc3d
{
	double x;
	double y;
	double z;
	double t; //theta
	double p; //phi
	double g; //gamma
};

//Used to store the raw data from Phidget. 
struct rawPhidget
{
	double accel[3];
	double angRate[3];
	double mag[3];

	CPhidget_Timestamp time;

};

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