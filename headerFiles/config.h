#ifndef CONFIG_H
#define CONFIG_H

#ifndef DEBUG
#define DEBUG  //remove this line when done debugging
#endif


#ifdef DEBUG

	#ifndef DEBUG_ZERO_GYRO
	//#define DEBUG_ZERO_GYRO
	#endif

	#ifndef DEBUG_ROTATION
	//#define DEBUG_ROTATION 
	#endif

	#ifndef DEBUG_INTEGRATE
	#define DEBUG_INTEGRATE
	#endif

	#ifndef DEBUG_FILTERING
	#define DEBUG_FILTERING
	#endif

	#ifndef DEBUG_LIVE_GRAPH
	#define DEBUG_LIVE_GRAPH
	#endif

//DEBUG
#endif	

const double US_PER_SECOND = 1000000;
const int dataRate = 16;
const double TAU = 20000;

const double GYRO_OFFSET[3] = {-0.283139, -0.0655911, -0.340913};


#endif
