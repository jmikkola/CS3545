/*
 * collisions.h
 *
 *  Created on: Mar 23, 2011
 *      Author: jeremy
 */

#ifndef COLLISIONS_H_
#define COLLISIONS_H_

typedef struct {
	vect3_t v1, v2, v3;
} Triangle;

int doesCollide(float boundingBox[3], Triangle *t);

#endif /* COLLISIONS_H_ */
