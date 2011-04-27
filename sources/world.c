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
static void setupRotationMatrix (float *matrix);
static void setFlipMatrix(float *matrix);


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
	int i, j, k, collided = 0, collisions = 0;
	vect_t **triPtr = triangleList,
		   *tri;
	vect3_t triangleNormal;

	// Reset the pushback value
	pushback[0] = 0;
	pushback[1] = 0;
	pushback[2] = 0;

	// Setup the rotation matrix
	setupRotationMatrix(matrix);
//	setFlipMatrix(matrix);

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
			collided = 1;
			if (math_absF(pushback[1]) < math_absF(triangleNormal[1]))
				 pushback[1] = triangleNormal[1];
			pushback[0] += triangleNormal[0];
			pushback[2] += triangleNormal[2];
			math_triangleNormal(&triangle[0][0], triangleNormal);
			VectorDot(triangleNormal, velocity, force);
			printf("Collided with normal %6.2f %6.2f %6.2f\n",
					triangleNormal[0], triangleNormal[1], triangleNormal[2]);

//			printf("Force = %6.2f\n", force);
			collisions++;
			VectorScale(triangleNormal, -1);
			// Scale the triangleNormal by the force
			// to get the amount pushed back

			// Set the maximum pushback amount
			for (j = 0; j < 3; j++) {
				if (math_absF(pushback[j]) < math_absF(triangleNormal[j]))
					pushback[j] = triangleNormal[j];
			}
		}
	}
	if (collided) {
		printf("Pushback = (%6.2f, %6.2f, %6.2f)\n",
				pushback[0], pushback[1], pushback[2]);
	}
}

static void setupRotationMatrix (float *matrix) {
	// This borrows heavily from the code in render.c
	float xRotMat[16], zRotMat[16];

	// Storage for precalculated trig function results
	float sinX, cosX, sinY, cosY, sinZ, cosZ;

	glmatrix_identity(xRotMat);
	glmatrix_identity(zRotMat);

	// Calculate the trig functions
	cosX = cos(camera.angleRad[_X]);
	sinX = sin(camera.angleRad[_X]);
	/*
	sinY = sin(camera.angleRad[_Y]);
	cosY = cos(camera.angleRad[_Y]);
	*/
	cosZ = cos(camera.angleRad[_Z]);
	sinZ = sin(camera.angleRad[_Z]);

	// Build the x rotation matrix
	xRotMat[5] = cosX;
	xRotMat[6] = sinX;
	xRotMat[9] = -sinX;
	xRotMat[10] = cosX;

	// Build the Z rotation matrix
	zRotMat[0] = cosZ;
	zRotMat[1] = -sinZ;
	zRotMat[4] = sinZ;
	zRotMat[5] = -cosZ;

	multiplyMats16f(zRotMat, xRotMat, matrix);
}
