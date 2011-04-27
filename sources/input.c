/*
 * input.c
 *
 *  Created on: Mar 3, 2011
 *      Author: jeremy
 */
#include <SDL/SDL.h>
#include <SDL/SDL_main.h>
#include <SDL/SDL_opengl.h>
#include <math.h>

#include "mathlib/mathlib.h"
#include "common.h"
#include "camera.h"
#include "input.h"
#include "timestep.h"
#include "world.h"

#define WALK_TIME_SCALE M_PI*2
#define WALK_POSITION_SCALE 0.5

// ==== Input ====

int keys_down[330];

void input_keyDown (SDLKey k) {
	keys_down[k] = 1;
	if (k == SDLK_ESCAPE || k == SDLK_q)
		user_exit = 1;
}

void input_keyUp (SDLKey k) {
	keys_down[k] = 0;
}

void input_mouseMove (int xPos, int yPos) {
	float halfWinWidth, halfWinHeight, dx, dy;

	halfWinWidth  = (float)WINDOW_WIDTH  / 2.0;
	halfWinHeight = (float)WINDOW_HEIGHT / 2.0;
	dx = ((float)xPos) - halfWinWidth;
    dy = yPos - halfWinHeight;

	// Update direction.
	camera_rotateX( dy * 0.5f);
	camera_rotateZ(-dx * 0.5f);
	// Reset cursor to center
	SDL_WarpMouse(halfWinWidth, halfWinHeight);
}

/*
 * Reads keyboard input and updates scene variables.
 * 'w' moves forward, 's' moves back
 * 'a' strafes left, 'd' strafes right
 * 'r' resets the view
 * 'f' Sets movement to free, 'p' sets movement to person
 */
void input_update () {
	float distance = 1.0f;
//	float distance = 60.0f * getTimeStep();
	static float walkTheta = 0;
	vect3_t old_position, boundingBox = {10, 10, 10}, pushback, velocity;
	VectorCopy(camera.position, old_position);

	// Reset velocity
	velocity[0] = 0; // x
	velocity[1] = 0; // y
	velocity[2] = 0; // z

	// walk motion
//	if (keys_down[SDLK_w] || keys_down[SDLK_s]) {
//		walkTheta -= getTimeStep();
//		walkHeight = WALK_POSITION_SCALE*sin(walkTheta * WALK_TIME_SCALE);
//	}

	// WASD movement
	if(keys_down[SDLK_w]) {
		velocity[1] -= distance;
	}
	if(keys_down[SDLK_s]) {
		velocity[1] += distance;
	}
	if(keys_down[SDLK_a]) {
		velocity[0] -= distance;
	}
	if(keys_down[SDLK_d]) {
		velocity[0] += distance;
	}

	world_getPushBack(boundingBox, velocity, pushback);
	velocity[0] += pushback[0];
	velocity[1] += pushback[1];
	velocity[2] += pushback[2];

	camera_translateForward(velocity[1]);
	camera_translateStrafe(velocity[0]);

	if(keys_down[SDLK_z])
		camera.position[_X] += 1;
	if(keys_down[SDLK_x])
		camera.position[_X] -= 1;
	if(keys_down[SDLK_c])
		camera.position[_Y] += 1;
	if(keys_down[SDLK_v])
		camera.position[_Y] -= 1;
	if(keys_down[SDLK_b])
		camera.position[_Z] += 1;
	if(keys_down[SDLK_n])
		camera.position[_Z] -= 1;


	// r for reset
	if(keys_down[SDLK_r]){
		reset_position();
	}

	// Set movement to free or person
	if (keys_down[SDLK_f])
		free_move = 1;
	else if (keys_down[SDLK_p])
		free_move = 0;
}
