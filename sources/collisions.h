/*
 * collisions.h
 *
 *  Created on: Mar 23, 2011
 *      Author: jeremy
 */

#ifndef COLLISIONS_H_
#define COLLISIONS_H_

typedef struct {
	float x, y, z;
} AABB;

typedef struct {
	vect3_t v1, v2, v3;
} Triangle;


#endif /* COLLISIONS_H_ */
