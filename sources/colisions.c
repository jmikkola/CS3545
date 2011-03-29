/*
 * colisions.c
 *
 *  Created on: Mar 23, 2011
 *      Author: jeremy
 */

#include "mathlib/mathlib.h"
#include "collisions.h"

int doesCollide(AABB* box, Triangle *t) {
	float x = box->x, y = box->y, z = box->z;
	float boxCoords[6] = {x, y, z, -x, -y, -z};
	vect3_t vA, vB, tnorm, vBox = {0,0,0};
	float *v1 = t->v1, *v2 = t->v2, *v3 = t->v3;
	int i, direction = 1, currentDirection = 1;

	// Test 3 cardinal axies
	for (i = 0; i < 3; i++) {
		if (v1[i] > boxCoords[i] && v2[i] > boxCoords[i] && v3[i] > boxCoords[i])
			return 0;
		if (v1[i] < -boxCoords[i] && v2[i] < -boxCoords[i] && v3[i] < -boxCoords[i])
			return 0;
	}

	// Test triangle normal
	VectorSubtract(v2, v1, vA);
	VectorSubtract(v3, v1, vB);
	VectorCross(vA, vB, tnorm);
	for (i = 0; i < 8; i++) {
		// Make pointer from a triangle vertex to each box vertex
		// (1 - 2*(i&1)) is either -1 or 1, depending on a bit in i
		vBox[0] = x*(1 - 2*(i&1)) - v1[0];
		vBox[1] = y*(1 - 2*(i&2)) - v1[1];
		vBox[2] = z*(1 - 2*(i&4)) - v1[2];
		// Project along the triangle normal
		VectorDot(tnorm, vBox, currentDirection);
		currentDirection = (currentDirection > 0);
		if (currentDirection != direction && i) {
			break;
		} else {
			direction = currentDirection;
		}

	}

	// Cross products of triangle edges with cardinal axies
	return 1;
}
