#ifndef _TIMER_H
#define _TIMER_H

#include <unistd.h>
#include <sys/time.h>

class Timer{
public:
	struct timeval t_start,t_end;
	double timeCost;
	void start(){
		gettimeofday(&t_start, NULL); 
	}
	void end(){
		gettimeofday(&t_end, NULL); 
		timeCost = t_end.tv_sec - t_start.tv_sec + (t_end.tv_usec - t_start.tv_usec)/1000000.0;
	}
};

#endif