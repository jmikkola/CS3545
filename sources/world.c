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
#include "common.h"
#include "world.h"


static vect_t **triangleList = NULL;
static int numTriangles = 0;


void printTriangle(float *triangle) {
	int j;
	printf("tri = [");
	for (j = 0; j < 9; j++)
		printf("%.2f ", triangle[j]);
	printf("]\n");
}

void world_allocCollisionTris(int number) {
	triangleList = (vect_t **) realloc(triangleList, (numTriangles+number) * sizeof(vect_t **));
	if (triangleList == NULL) {
		fprintf(stderr, "Fatal error: could not allocate memory "
				"for collision detection triangles in world.c\n");
	}
}

void world_addCollisionTri(vect_t *t) {
	int i;
	vect_t *triangle = malloc(sizeof(vect_t) * 9);
//	vect_t *triangle = NULL;
	if (triangle != NULL) {
		for (i = 0; i < 9; i++)
			triangle[i] = t[i];
		triangleList[numTriangles] = triangle;
		numTriangles++;
	}
}

void world_getPushBack(float boundingBox[3], vect3_t velocity, vect3_t pushback) {
	float triangle[3][3] = {{0,0,0},{0,0,0},{0,0,0}},
		  matrix[16],
		  force;
	int i, j, k;
	vect_t **triPtr = triangleList,
		   *tri;
	vect3_t triangleNormal;
	vect4_t q;

	// Reset the pushback value
	pushback[0] = 0;
	pushback[1] = 0;
	pushback[2] = 0;


	for (i = 0; i < numTriangles; i++, triPtr++) {
		// Copy triangle into correct format
		// and translate by camera position
		tri = *triPtr;
		for (j = 0; j < 3; j++)
			for (k = 0; k < 3; k++)
				triangle[j][k] = tri[k + 3*j] - camera.position[k];

		// Apply camera rotation to each vertex
		for (k = 0; k < 3; k++)
			rotatePoint(triangle[k], matrix);

		// If if collides, get pushback vector
		if (doesCollide(boundingBox, triangle)) {
			math_triangleNormal(&triangle[0][0], triangleNormal);
			VectorDot(triangleNormal, velocity, force);
//			force = velocity * triangleNormal[0];

			// Scale the triangleNormal by the force
			// to get the amount pushed back
			VectorScale(triangleNormal, -force);

			// Set the maximum pushback amount
			for (j = 0; j < 3; j++) {
				if (math_absF(pushback[j]) < math_absF(triangleNormal[j]))
					pushback[j] = triangleNormal[j];
			}
		}
	}
}


