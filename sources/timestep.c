/*
 * timestep.c
 *
 *  Created on: Mar 16, 2011
 *      Author: jeremy
 */

#include "timestep.h"
#include <stdlib.h>
#include <sys/time.h>

static long long int delta_ms = 0;

static struct timeval currTv, oldTv;

void initTime() {
	gettimeofday(&currTv, NULL);
}

void timeStep() {
	oldTv.tv_sec =  currTv.tv_sec;
	oldTv.tv_usec = currTv.tv_usec;
	gettimeofday(&currTv, NULL);
	delta_ms = (currTv.tv_sec - oldTv.tv_sec)*1000000 + currTv.tv_usec - oldTv.tv_usec;
}

float getTimeStep() {
	return delta_ms / 1.0e6f;
}
