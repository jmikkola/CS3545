/*
 * input.c
 *
 *  Created on: Mar 3, 2011
 *      Author: jeremy
 */
#include <SDL/SDL.h>
#include <SDL/SDL_main.h>
#include <SDL/SDL_opengl.h>
#include "mathlib/mathlib.h"
#include "common.h"
#include "camera.h"
#include "input.h"
#include "timestep.h"

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
	float distance = 60.0f * getTimeStep();
	// WASD movement
	if(keys_down[SDLK_w])
		camera_translateForward(-1 * distance);
	if(keys_down[SDLK_s])
		camera_translateForward(distance);
	if(keys_down[SDLK_a])
		camera_translateStrafe(-1 * distance);
	if(keys_down[SDLK_d])
		camera_translateStrafe(distance);

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
