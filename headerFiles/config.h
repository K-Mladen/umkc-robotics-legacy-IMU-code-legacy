
#ifndef CONFIG_H
#define CONFIG_H

//========================================
//DEBUGS
//========================================

#ifndef DEBUG
#define DEBUG  //remove this line when done debugging
#endif

#ifdef DEBUG

	#ifndef DEBUG_RAW_GYRO
	//#define DEBUG_RAW_GYRO
	#endif

	#ifndef DEBUG_ZERO_GYRO
	#define DEBUG_ZERO_GYRO
	#endif

	#ifndef DEBUG_INTEGRATE
	//#define DEBUG_INTEGRATE
	#endif

	#ifndef DEBUG_FILTERING
	//#define DEBUG_FILTERING
	#endif

	//"@ Raw Phidget data, non zeroed, avg constant, then zeroed." 
	#ifndef DEBUG_LIVE_GRAPH_PHIDGET_RAW	
	#define DEBUG_LIVE_GRAPH_PHIDGET_RAW
	#endif

	//"@ Rotated gyro data, after zeroing
	#ifndef DEBUG_LIVE_GRAPH_ROTATION
	#define DEBUG_LIVE_GRAPH_ROTATION 
	#endif

	//
	#ifndef DEBUG_LIVE_GRAPH_DELTA
	#define DEBUG_LIVE_GRAPH_DELTA
	#endif

	//"@ Current Orientation, rotated, then non filtered and filtered."
	#ifndef DEBUG_LIVE_GRAPH_CURRENT_ORIENTATION
	#define DEBUG_LIVE_GRAPH_CURRENT_ORIENTATION
	#endif

//DEBUG
#endif	


//========================================
//CONSTANTS
//========================================

const double US_PER_SECOND = 1000000;
const int dataRate = 16;
const double TAU = 20000;

const double GYRO_OFFSET[3] = {-0.260092, 0.0562493, -0.262105};
const double ACC_OFFSET[3] = {0.00943341, 0.0474827, 0.999374};
//CONFIG_H
#endif
