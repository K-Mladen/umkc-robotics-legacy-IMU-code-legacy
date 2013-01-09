#ifndef CONFIG_H
#define CONFIG_H

#ifndef DEBUG
#define DEBUG  //remove this line when done debugging
#endif


#ifdef DEBUG

#ifndef DEBUG_ROTATION
//#define DEBUG_ROTATION 
#endif

#ifndef DEBUG_FILTERING
#define DEBUG_FILTERING
#endif

#ifndef DEBUG_INTEGRATE
#define DEBUG_INTEGRATE
#endif

//DEBUG
#endif	

const double US_PER_SECOND = 1000000;


#endif
