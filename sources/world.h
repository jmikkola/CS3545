/*
 * world.h
 *
 *  Created on: Apr 10, 2011
 *      Author: jeremy
 */

#ifndef WORLD_H_
#define WORLD_H_

void world_allocCollisionTris(int number);
void world_addCollisionTri(Triangle t);
int world_testCollision(float boundingBox[3]);
void world_getPushBack(float boundingBox[3], vect3_t velocity, vect3_t pushback);

#endif /* WORLD_H_ */
