/*
 * camera.h
 *
 *  Created on: Mar 2, 2011
 *      Author: jeremy
 */

#ifndef CAMERA_H_
#define CAMERA_H_

// Functions
void camera_init ();
void reset_position();
void camera_rotateX (float degree);
void camera_rotateY (float degree);
void camera_rotateZ (float degree);
void camera_translateForward (float dist);
void camera_translateStrafe (float dist);

// Types
typedef struct {
	vect3_t position;
	vect3_t angleRad;
} camera_t;


// Variables
//Maintain a matrix for each rotation and one for translation
extern float xRotMatrix[16],
			 yRotMatrix[16],
			 zRotMatrix[16],
			 translateMatrix[16];
extern camera_t camera;
extern float walkHeight;

#endif /* CAMERA_H_ */
