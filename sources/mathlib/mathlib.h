// Header guard
#ifndef MATHLIB_H
#define MATHLIB_H

// Constants
#define M_PI_DIV180 0.01745329251994329576
#define M_PI 3.14159265358979323846
#define M_PI_DIV2 1.57079632679489661923
#define _X 0
#define _Y 1
#define _Z 2

// Vector types
typedef float vect_t;
typedef vect_t vect2_t[2];
typedef vect_t vect3_t[3];
typedef vect_t vect4_t[4];
typedef vect_t vect5_t[5];

// Cross product
#define VectorCross(v1, v2, out) {  \
    out[0]=v1[1]*v2[2]-v1[2]*v2[1]; \
    out[1]=v1[2]*v2[0]-v1[0]*v2[2]; \
    out[2]=v1[0]*v2[1]-v1[1]*v2[0];}
//
// Dot product
#define VectorDot(v1, v2, out) {out=v1[0]*v2[0]+v1[1]*v2[1]+v1[2]*v2[2];}
//
// Vector addition
#define VectorAdd(v1, v2, out) {out[0]=v1[0]+v2[0]; \
    out[1]=v1[1]+v2[1]; \
    out[2]=v1[2]+v2[2];}
//
// Vector subtraction
#define VectorSubtract(v1, v2, out) {out[0]=v1[0]-v2[0]; \
    out[1]=v1[1]-v2[1]; \
    out[2]=v1[2]-v2[2];}
//
// Vector scaling
#define VectorScale(v, c) {v[0]*=c;v[1]*=c;v[2]*=c;}
// 
// Vector copying
#define VectorCopy(v1, v2) {v2[0]=v1[0];v2[1]=v1[1];v2[2]=v1[2];}
// 
// Clear the vector
#define VectorClear(v) {v[0]=0.0f;v[1]=0.0f;v[2]=0.0f;}
// 
// Invert the vector
#define VectorInverse(v) {v[0]=-v[0];v[1]=-v[1];v[2]=-v[2];}
// 
// Find the magnitude of the vector
// Any file that uses this must include math.h
#define VectorMagnitude(v, out) \
    {out=sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);}
//
// Normalize the vector
#define VectorNormalize(v) {float mag;VectorMagnitude(v,mag); \
    v[0]/=mag;v[1]/=mag;v[2]/=mag;}
//
// Print a vector
#define prvect(v) {printf("%.2f %.2f %.3f\n", v[0], v[1], v[2]);}
//
// Set a matrix to the identity
void glmatrix_identity (float *m);
//
// End header guard
#endif
