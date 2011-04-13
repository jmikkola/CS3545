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
