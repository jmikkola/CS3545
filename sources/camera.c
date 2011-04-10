/*
 * camera.c
 *
 *  Created on: Mar 2, 2011
 *      Author: jeremy
 */
#include <SDL/SDL.h>
#include <math.h>
#include "common.h"
#include "mathlib/mathlib.h"
#include "models/renderer_models.h"
#include "camera.h"

float xRotMatrix[16],
			 yRotMatrix[16],
			 zRotMatrix[16],
			 translateMatrix[16];
float walkHeight = 0;
camera_t camera;

void camera_init() {
	glmatrix_identity(xRotMatrix);
	glmatrix_identity(yRotMatrix);
	glmatrix_identity(zRotMatrix);
	glmatrix_identity(translateMatrix);
}


void reset_position() {
	VectorClear(camera.angleRad);
	VectorClear(camera.position);
	camera.position[_Y] = -200;
	camera.position[_Z] = 5;
	walkHeight = 0;
}

void camera_rotateX (float degree) {
	camera.angleRad[_X] += degree * M_PI_DIV180;
	if (degree > 0 && camera.angleRad[_X] > M_PI_DIV2)
		camera.angleRad[_X] = M_PI_DIV2;
	else if (degree < 0 && camera.angleRad[_X] < -M_PI_DIV2)
		camera.angleRad[_X] = -M_PI_DIV2;
}

void camera_rotateY (float degree) {
	camera.angleRad[_Y] += degree * M_PI_DIV180;
}

void camera_rotateZ (float degree) {
	camera.angleRad[_Z] += degree * M_PI_DIV180;
}

void camera_translateForward(float dist) {
	float theta = camera.angleRad[_X], phi = camera.angleRad[_Z];
	float cosTheta = cos(theta), sinTheta = sin(theta),
		  cosPhi = cos(phi), sinPhi = sin(phi);
	if (free_move) {
		// Free movement
		camera.position[_X] += dist * cosTheta * sinPhi;
		camera.position[_Y] -= dist * cosTheta * cosPhi;
		camera.position[_Z] += dist * sinTheta;
	} else {
		// Person movement
		camera.position[_X] +=  dist * sinPhi;
		camera.position[_Y] -=  dist * cosPhi;
		camera.position[_Z] +=  0;
	}
}

void camera_translateStrafe(float dist) {
	float phi = camera.angleRad[_Z] + 3.14159/2.0;
	float cosPhi = cos(-phi), sinPhi = sin(-phi);
	// Person or Free
	camera.position[_X] -= dist * sinPhi;
	camera.position[_Y] -= dist * cosPhi;
}
