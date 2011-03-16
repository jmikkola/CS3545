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

#endif /* TIMESTEP_H_ */
