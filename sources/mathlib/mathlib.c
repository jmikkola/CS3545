#include <math.h>
#include "mathlib.h"

/*
 * glmatrix_identity
 * Sets input matrix to the identity.
 */
void glmatrix_identity (float *m) {
	int i;
	for(i = 0; i < 16; i++)
		if(i%5 == 0)
			m[i] = 1.0;
		else
			m[i] = 0.0;
}

void math_triangleNormal(vect_t *t, vect3_t normal) {
	vect_t *v1 = t, *v2 = &t[3], *v3 = &t[6];
	vect3_t side1, side2;

	// Create side vectors
	// Should hopefully give a vector pointing out of shapes
	VectorSubtract(v2, v1, side1);
	VectorSubtract(v3, v2, side2);
	// Create triangle normal
	VectorCross(side1, side2, normal);
	VectorNormalize(normal);
}

float math_absF(float val) {
	if (val < 0)
		return -val;
	return val;
}

void multiplyQuaternoins(vect4_t q1, vect4_t q2, vect4_t out) {
	out[0] = q1[0]*q2[0] - q1[1]*q2[1] - q1[2]*q2[2] - q1[3]*q2[3];
	out[1] = q1[0]*q2[1] + q1[1]*q2[0] + q1[2]*q2[3] - q1[3]*q2[2];
	out[2] = q1[0]*q2[2] - q1[1]*q2[3] + q1[2]*q2[0] + q1[3]*q2[1];
	out[3] = q1[0]*q2[3] + q1[1]*q2[2] - q1[2]*q2[1] + q1[3]*q2[0];
}

// Matrices are in column-major order
void multiplyMats16f(float *mat1, float *mat2, float *out) {
	int i, j, k;
	float temp;
	for (i = 0; i < 4; i++) { // column
		for (j = 0; j < 4; j++) { // row
			temp = 0;
			for (k = 0; k < 4; k++) { // index
				temp += mat1[j + 4*k] * mat2[4*i + k];
			}
			out[i*4 + j] = temp;
		}
	}
}


// point is a row matrix
// matrix is 4x4 in column-major order
void rotatePoint(float *point, float *matrix) {
	vect3_t out;
	out[0] = point[0] * matrix[0] +
			 point[1] * matrix[1] +
			 point[2] * matrix[2];
	out[1] = point[0] * matrix[4] +
			 point[1] * matrix[5] +
			 point[2] * matrix[6];
	out[2] = point[0] * matrix[8] +
			 point[1] * matrix[9] +
			 point[2] * matrix[10];
	point[0] = out[0];
	point[1] = out[1];
	point[2] = out[2];
}
