/******************************************************************************
*  More modular odometry script to enable multiple people to work on		  *
*  different parts at the same time, without messing each other up.			  *
*  Functions marked by an empty comment block in the first 4 columns may	  *
*  need to be renamed to match actual functions.	Just saying.			  *
******************************************************************************/

#include <ros/ros>
#include <tf/transform_broadcaster>
#include <nav_msgs/Odometry>

#include "odomvar.h"
#include "velfromacc.h"
#include "acctransform.h"
#include "accget.h"

int main(int argc, char *argv[])
{
	//symbolic constants that may need changing
	const float loopFrequency = 10.0;


	ros::init(argc, argv, "odometry_publisher");
	
	ros::NodeHandle n;
	ros::Publisher odom_pub = n.advertise<nav_msgs::Odometry>("odom", 50);
	tf::TransformBroadcaster odom_broadcaster;
	
	ros::Time current_time = ros::Time::now(), last_time = ros::Time::now();

	ros::Rate r(loopFrequency);

	while(n.ok())
	{
		ros::spinOnce();						//Check for incoming messages
		last_time = current_time;				//set last_time based on last
		current_time = ros::Time::now();		//	iteration

/**/	struct acc3d = accget();
/**/	struct acc2d = acctransform(acc3);
/**/	struct vel2d = velfromacc(acc2d);
		
		r.sleep();
	}
//	return 0;
}

