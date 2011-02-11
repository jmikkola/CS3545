#include "math.h"

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
