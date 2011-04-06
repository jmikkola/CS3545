/*
 * colisions.c
 *
 *  Created on: Mar 23, 2011
 *      Author: jeremy
 */

#include "mathlib/mathlib.h"
#include "collisions.h"

int testCardinalAxies(float boundingBox[3], Triangle *t);
int testTriangleNormal(float boundingBox[3], Triangle *t);
int testTriangleEdges(float boundingBox[3], Triangle *t);

int doesCollide(AABB* box, Triangle *t) {
	float x = box->x, y = box->y, z = box->z;
	float boundingBox[3] = {x, y, z};

	return testCardinalAxies(boundingBox, t)
			&& testTriangleNormal(boundingBox, t)
			&& testTriangleEdges(boundingBox, t);
}

int testCardinalAxies(float boundingBox[3], Triangle *t) {
	float *v1 = t->v1,
		  *v2 = t->v2,
		  *v3 = t->v3;
	int i;
	float distance;

	// Loop through the directions x,y,z
	for (i = 0; i < 3; i++) {
		distance = boundingBox[i];
		// Test if all vertices of the triangle are on
		// one side of the box
		if (v1[i] > distance
			&& v2[i] > distance
			&& v3[i] > distance)
			return 0;
		// Try the other side of the box
		if (v1[i] < -distance
			&& v2[i] < -distance
			&& v3[i] < -distance)
			return 0;
	}

	// No sides found where the triangle is completely to one side
	return 1;
}

int testTriangleNormal(float boundingBox[3], Triangle *t) {
	float *v1 = t->v1,
		  *v2 = t->v2,
		  *v3 = t->v3;
	vect3_t triangleNormal,
			side1,
			side2,
			boxVector;
	int direction,
		prevDirection,
		i;
	float distance,
		  x = boundingBox[0],
		  y = boundingBox[1],
		  z = boundingBox[2];

	// Create triangle normal
	VectorSubtract(v2, v1, side1);
	VectorSubtract(v3, v1, side2);
	VectorCross(side1, side2, triangleNormal);

	for (i = 0; i < 8; i++) {
		// Generate the vector to each point on the box
		// (1 - 2*(i&1)) is either -1 or 1, depending on a bit in i
		boxVector[0] = x*(1 - 2*(i&1)) // Point on box
					   - v1[0];        // Point on triangle
		boxVector[1] = y*(1 - 2*(i&2))
				       - v1[1];
		boxVector[2] = z*(1 - 2*(i&4))
				       - v1[2];

		// Project along the triangle normal
		VectorDot(triangleNormal, boxVector, distance);

		// figure out which side of the triangle that part of the box is on
		direction = (distance > 0);

		// Test whether that direction is different from the previous directions
		if ((i != 0)  							// Don't test the first instance
			&& (direction != prevDirection)) {  // Ensure that the direction is the same
			// It wasn't, so there might be a collision
			return 1;
		} else {
			// It was on the same side, keep checking
			prevDirection = direction;
		}
	}

	// There is no collision
	return 0;
}

int testTriangleEdges(float boundingBox[3], Triangle *t) {
	int edgeNum, axisNum;
	vect3_t edgeNormal,
			side1,
			side2;

	for (edgeNum = 0; edgeNum < 3; edgeNum++) {
		for (axisNum = 0; axisNum < 3; axisNum++) {

		}
	}

	return 0;
}

