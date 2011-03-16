/*
 * timestep.h
 *
 *  Created on: Mar 16, 2011
 *      Author: jeremy
 */

#ifndef TIMESTEP_H_
#define TIMESTEP_H_

// Update to the next frame
void timeStep(void);

// Sets the initial time to work from
void initTime(void);

// Returns the time step, in seconds
float getTimeStep(void);

// Returns the current accumulator value
long long int getAcc(void);

// Returns the number of frames since last subtractAcc() call
long int getFrameCount();

// Subtracts a value from the accumulator,
// if the value is not larger than the current value of the accumulator.
void subtractAcc(long long int value);

#endif /* TIMESTEP_H_ */
