/*
 * world.c
 *
 *  Created on: Apr 10, 2011
 *      Author: jeremy
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "mathlib/mathlib.h"
#include "camera.h"
#include "collisions.h"
#include "world.h"


static vect_t **triangleList = NULL;
static vect_t **listPtr = NULL;
static int numTriangles = 0;

static void transform(float *point, vect4_t q);
static void buildTransform(vect4_t q);

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
	int i, j, k;
	vect_t **triPtr = triangleList, *tri;
	vect4_t q;
	// Create camera-rotation quaternion
	buildTransform(q);
	for (i = 0; i < numTriangles; i++, triPtr++) {
		// Copy triangle into correct format
		// and translate by camera position
		tri = *triPtr;
		for (j = 0; j < 3; j++)
			for (k = 0; k < 3; k++)
				triangle[j][k] = tri[k + 3*j] - camera.position[k];
		// Apply camera rotation to each vertex
		for (k = 0; k < 3; k++)
			transform(triangle[k], q);
		// Test for collision
		if (doesCollide(boundingBox, triangle))
			return 1;
	}
	return 0;
}

static void transform(float *point, vect4_t q){
	vect4_t qin, qout;
	VectorCopy(point, qin);
	QuaternionMultiply(qin, q, qout);
	VectorCopy(qout, point);
}

static void buildTransform(vect4_t q) {
	vect4_t q1, q2, q3;
	// Apply X rotation
	q1[0] = sin(0.5f * camera.angleRad[_X]);
	q1[1] = 0;
	q1[2] = 0;
	q1[3] = cos(0.5f * camera.angleRad[_X]);
	// Apply Y rotation
	q2[0] = 0;
	q2[1] = sin(0.5f * camera.angleRad[_Y]);
	q2[2] = 0;
	q2[3] = cos(0.5f * camera.angleRad[_Y]);
	QuaternionMultiply(q1, q2, q3);
	// Apply Z rotation
	q2[0] = 0;
	q2[1] = 0;
	q2[2] = sin(0.5f * camera.angleRad[_Z]);
	q2[3] = cos(0.5f * camera.angleRad[_Z]);
	QuaternionMultiply(q3, q2, q);

}
