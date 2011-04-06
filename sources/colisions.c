/*
 * colisions.c
 *
 *  Created on: Mar 23, 2011
 *      Author: jeremy
 */

#include <math.h>
#include "mathlib/mathlib.h"
#include "collisions.h"

int testCardinalAxies(float boundingBox[3], vect_t triangle[3][3]);
int testTriangleNormal(float boundingBox[3], vect_t triangle[3][3]);
int testTriangleEdges(float boundingBox[3], vect_t triangle[3][3]);

vect_pair crossWithTriangle(vect3_t normal, vect_t triangle[3][3]);
vect_pair crossWithBox(vect3_t normal, float x, float y, float z);
int testWithNormal(vect3_t normal, float boundingBox[3], vect_t triangle[3][3]);
int testBounds(vect_pair triangle, vect_pair box);

int doesCollide(float boundingBox[3], vect_t triangle[3][3]) {
	return testCardinalAxies(boundingBox, triangle)
			&& testTriangleNormal(boundingBox, triangle)
			&& testTriangleEdges(boundingBox, triangle);
}

int testCardinalAxies(float boundingBox[3], vect_t triangle[3][3]) {
	float *v1 = triangle[0],
		  *v2 = triangle[1],
		  *v3 = triangle[2];
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

int testTriangleNormal(float boundingBox[3], vect_t triangle[3][3]) {
	float *v1 = triangle[0],
		  *v2 = triangle[1],
		  *v3 = triangle[2];
	vect3_t triangleNormal, side1, side2;

	// Create triangle normal
	VectorSubtract(v2, v1, side1);
	VectorSubtract(v3, v1, side2);
	VectorCross(side1, side2, triangleNormal);
	VectorNormalize(triangleNormal);

	return testWithNormal(triangleNormal, boundingBox, triangle);
}

int testWithNormal(vect3_t normal, float boundingBox[3], vect_t triangle[3][3]) {
	vect_pair triangleBounds, boxBounds;

	// Check the points on the triangle
	triangleBounds = crossWithTriangle(normal, triangle);
	// Check the points on the box
	boxBounds = crossWithBox(normal,
							 boundingBox[0],
							 boundingBox[1],
							 boundingBox[2]);
	// Check to see if the intervals are non-overlapping
	// (testBounds will return 1 if they overlap)
	return testBounds(triangleBounds, boxBounds);
}

int testBounds(vect_pair triangle, vect_pair box) {
	// __.b stores the maximum value,
	// __.a stores the minimum.
	return (triangle.b < box.a) || (triangle.a > box.b);
}

vect_pair crossWithBox(vect3_t normal, float x, float y, float z) {
	float boxMin = 0, boxMax = 0, distance;
	vect3_t boxPoint;
	vect_pair out;
	int i;

	// Loop through points on the box
	for (i = 0; i < 8; i++) {
		// Get a point on the box
		// [Explanation: (1 - 2*(bit)) will be -1 or 1, depending on the bit]
		boxPoint[0] = x*(1 - 2*(i&1));
		boxPoint[1] = y*(1 - 2*(i&2 >> 1));
		boxPoint[2] = z*(1 - 2*(i&4 >> 2));

		// Project along the normal
		VectorDot(boxPoint, normal, distance);

		// Compare with current min / max distance
		// and update if needed
		if (i == 0) {
			boxMin = distance;
			boxMax = distance;
		} else {
			if (distance < boxMin)
				boxMin = distance;
			if (distance > boxMax)
				boxMax = distance;
		}
	}

	out.a = boxMin;
	out.b = boxMax;

	return out;
}

vect_pair crossWithTriangle(vect3_t normal, vect_t triangle[3][3]) {
	float triMin = 0, triMax = 0, distance;
	vect_pair out;
	int i;
	for (i = 0; i < 3; i++) {
		vect_t *point = triangle[i];
		VectorDot(point, normal, distance);

		if (i == 0) {
			triMin = distance;
			triMax = distance;
		} else {
			if (distance < triMin)
				triMin = distance;
			if (distance > triMax)
				triMax = distance;
		}
	}
	out.a = triMin;
	out.b = triMax;
	return out;
}

int testTriangleEdges(float boundingBox[3], vect_t triangle[3][3]) {
	vect3_t normal;
	vect_t *axis, *side;
	vect_t axies[3][3] =
	        {{1,0,0},
			 {0,1,0},
			 {0,0,1}};
	int i, j;

	for (i = 0; i < 3; i++) {
		axis = axies[i];
		for (j = 0; j < 3; j++) {
			side = triangle[i];
			VectorCross(axis, side, normal);
			if (testWithNormal(normal, boundingBox, triangle))
				return 1;
		}
	}

	return 0;
}

