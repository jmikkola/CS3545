/*
 * world.c
 *
 *  Created on: Apr 10, 2011
 *      Author: jeremy
 */

#include <stdio.h>
#include <stdlib.h>
#include "mathlib/mathlib.h"
#include "camera.h"
#include "collisions.h"
#include "world.h"


static vect_t **triangleList = NULL;
static vect_t **listPtr = NULL;
static int numTriangles = 0;

void world_allocCollisionTris(int number) {
	numTriangles += number;
	triangleList = (vect_t **) realloc(triangleList, numTriangles * sizeof(vect_t **));
	if (triangleList == NULL) {
		fprintf(stderr, "Fatal error: could not allocate memory "
				"for collision detection triangles in world.c\n");
	}
}

void world_addCollisionTri(Triangle t) {
	if (listPtr == NULL)
		listPtr = triangleList;
	else
		listPtr++;
	*listPtr = t;
}

int world_testCollision(float boundingBox[3]) {
	float triangle[3][3] = {{0,0,0},{0,0,0},{0,0,0}};
	int i, k;
	vect_t **triPtr = triangleList, *tri;
	for (i = 0; i < numTriangles; i++, triPtr++) {
		// Copy triangle into correct format
		tri = *triPtr;
		for (k = 0; k < 3; k++)
			triangle[0][k] = tri[k];
		for (k = 0; k < 3; k++)
			triangle[1][k] = tri[k+3];
		for (k = 0; k < 3; k++)
			triangle[2][k] = tri[k+6];
		// Test for collision
		if (doesCollide(boundingBox, triangle))
			return 1;
	}
	return 0;
}
