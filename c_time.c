#include "c_time.h"

#ifdef _WIN32

	#define WIN32_LEAN_AND_MEAN
	#include <windows.h>
	
	ctime_t SYS_getTicks()
	{
	    return GetTickCount();
	}

#elif __linux__

	#include <time.h>
	#include <inttypes.h>
	#include <math.h>
	
	ctime_t SYS_getTicks()
	{
		//ctime_t         ms; // Milliseconds
		//time_t          s;  // Seconds
		//struct timespec spec = {0};
	
		//clock_gettime(CLOCK_MONOTONIC, &spec);
	
		//s = spec.tv_sec;
		//return round(spec.tv_nsec / 1.0e6); // Convert nanoseconds to milliseconds
		
		struct timespec ts;
	    ctime_t theTick = 0U;
	    clock_gettime( CLOCK_REALTIME, &ts );
	    theTick  = ts.tv_nsec / 1000000;
	    theTick += ts.tv_sec * 1000;
	    return theTick;
		
	}

#endif //_WIN32