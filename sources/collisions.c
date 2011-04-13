/*
 * collisions.c
 *
 *  Created on: Mar 23, 2011
 *      Author: jeremy
 */

#include <math.h>
#include "mathlib/mathlib.h"
#include "collisions.h"

int testCardinalAxies      (float boundingBox[3], 
                            vect_t triangle[3][3]);

int testTriangleNormal     (float boundingBox[3], 
                            vect_t triangle[3][3]);

int testTriangleEdges      (float boundingBox[3], 
                            vect_t triangle[3][3]);

vect_pair crossWithTriangle(vect3_t normal, 
                            vect_t triangle[3][3]);

vect_pair crossWithBox     (vect3_t normal, 
                            float x, 
                            float y, 
                            float z);

int testWithNormal         (vect3_t normal, 
                            float boundingBox[3], 
                            vect_t triangle[3][3]);

int testBounds             (vect_pair triangle, 
                            vect_pair box);

static int testUsed = 0;
static int useCount[4] = {0,0,0,0};

int getTestUsed() {
    return testUsed;
}

int* getUseCounts() {
	return &useCount[0];
}

void resetUseCount() {
	int i;
	for (i = 0; i < 4; i++)
		useCount[i] = 0;
}

// doesCollide
//
int doesCollide(float boundingBox[3], vect_t triangle[3][3]) {
	int out = 0;
    if (testCardinalAxies(boundingBox, triangle)) {
        testUsed = 1;
    } else if (testTriangleNormal(boundingBox, triangle)) {
        testUsed = 2;
    } else if (testTriangleEdges(boundingBox, triangle)) {
        testUsed = 3;
    } else {
    	testUsed = 4;
    	out = 1;
    }
    useCount[testUsed-1]++;
    return out;
}

// testCardinalAxies
// Tests to see if there is a plane of separation on
// one of the cardinal axies - x, y, or z. 
// Returns 1 if there is, 0 if there is not
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
			return 1;
		// Try the other side of the box
		if (v1[i] < -distance
			&& v2[i] < -distance
			&& v3[i] < -distance)
			return 1;
	}

	// No sides found where the triangle is completely to one side
	return 0;
}

// testTriangleNormal
// Tests the box with the triangle normal to see
// if the box is entirely on one side of the triangle or 
// the other. 
// Returns 1 if they do not collide, 0 if they still may
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

// testWithNormal
// tests to see if there is the plane with
// the specified normal is a plane of separation
// for the box and the triangle.
// Returns 1 if it is, 0 if it is not
int testWithNormal(vect3_t normal, 
                   float boundingBox[3], 
                   vect_t triangle[3][3]) {
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

// testBounds
// tests to see if the intervals are non-overlapping
// Returns 1 if they do not overlap, 0 if they do
int testBounds(vect_pair triangle, vect_pair box) {
	// __.b stores the maximum value,
	// __.a stores the minimum.
	return (triangle.b < box.a) || (triangle.a > box.b);
}

// crossWithBox
// Finds the range of values given by crossing the points of
// the box with the normal.
// Returns a vect_pair where a is the minimum
//         and b is the maximum value.
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

// crossWithTriangle
// Finds the range of values given by crossing the points of 
// the triangle with the given normal vector. 
// Returns a vect_pair where a is the minimum
//         and b is the maximum values. 
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

// testTriangleEdges
// Tests to see if there is a plane of separation
// between the triangle and the box that has a normal
// that is found by crossing a side of the triangle 
// with a cardinal axis (i.e. x, y, or z)
// Returns 1 if such a plane exists, 
//         0 if one does not. 
int testTriangleEdges(float boundingBox[3], vect_t triangle[3][3]) {
	vect3_t normal, triangleNormal, sides[3];
	vect_t *axis, *side;
	vect_t axies[3][3] =
	        {{1,0,0}, {0,1,0}, {0,0,1}};
	int i, j;

	// Create side vectors
	VectorSubtract(triangle[1], triangle[0], sides[0]);
	VectorSubtract(triangle[2], triangle[1], sides[1]);
	VectorSubtract(triangle[0], triangle[2], sides[2]);

    for (i = 0; i < 3; i++) {
        side = sides[i];
        for (j = 0; j < 3; j++) {
            axis = axies[i];
            VectorCross(axis, side, normal);
            VectorNormalize(normal);
            if (testWithNormal(normal, boundingBox, triangle))
                return 1;
        }
    }

	return 0;
}

