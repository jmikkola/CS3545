/*
 * File: main.c
 * Author: Jeremy Mikkola
 * Last modified: 12/02/2011
 *
 * Packages needed to compile:
 * libsdl-ttf2.0-dev libboost-dev build-essential libsdl1.2-dev libsdl-image1.2-dev libsdl-mixer1.2-dev
 */

#include <SDL/SDL.h>
#include <SDL/SDL_main.h>
#include <SDL/SDL_opengl.h>
#include <stdio.h>
#include "common.h"
#include "mathlib/mathlib.h"
#include "models/renderer_models.h"
#include "materials/renderer_materials.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define MAX_NUM_TEXTURES 16


typedef struct {
	vect3_t position;
	vect3_t angleRad;
} camera_t;

static void r_init ();
static void r_drawFrame ();
static void input_update ();
static void input_keyDown (SDLKey k);
static void input_keyUp (SDLKey k);
static void input_mouseMove (int xPos, int yPos);
static void cube (float size, float offsetX, float offsetY, float offsetZ);
static void r_setupProjection();
static void r_setupModelview();
static void camera_init ();
static void camera_rotateX (float degree);
static void camera_rotateY (float degree);
static void camera_rotateZ (float degree);
static void camera_translateForward (float dist);
static void camera_translateStrafe (float dist);

static camera_t camera;
static int user_exit = 0, free_move = 0;
static int keys_down[330];
static float ratio, size = 2.0f;
//Maintain a matrix for each rotation and one for translation
static float xRotMatrix[16],
			 yRotMatrix[16],
			 zRotMatrix[16],
			 translateMatrix[16];
static const GLfloat flipMatrix[16] =
		{1.0, 0.0,  0.0, 0.0,
		 0.0, 0.0, -1.0, 0.0,
		 0.0, 1.0,  0.0, 0.0,
		 0.0, 0.0,  0.0, 1.0};


int main (int argc, char* argv[]) {
	SDL_Event		event;
	SDL_Surface		*screen;

	if (SDL_Init (SDL_INIT_VIDEO|SDL_INIT_TIMER) != 0) {
		printf ("Unable to initialize SDL: %s\n", SDL_GetError ());
		return 1;
	}

	SDL_WM_SetCaption ("Cube of cubes", "Cube of cubes");
	SDL_ShowCursor(SDL_DISABLE);
	SDL_GL_SetAttribute (SDL_GL_DOUBLEBUFFER, 1);

	//Initialize window
	screen = SDL_SetVideoMode (WINDOW_WIDTH, WINDOW_HEIGHT, 32, SDL_OPENGL);
	if (!screen) {
		printf ("Unable to set video mode: %s\n", SDL_GetError ());
		return 1;
	}

	r_init ();

	//Main loop
	while (!user_exit) {
		//Handle input
		while (SDL_PollEvent (&event)) {
			switch (event.type) {
			case SDL_KEYDOWN:
				input_keyDown (event.key.keysym.sym);
				break;
			case SDL_KEYUP:
				input_keyUp (event.key.keysym.sym);
				break;
			case SDL_MOUSEMOTION:
				input_mouseMove (event.motion.x, event.motion.y);
				break;
			case SDL_QUIT:
				exit (0);
			}
		}
		input_update ();
		r_drawFrame ();
		// Cap it at 100 fps
		usleep (10000);
	}

	SDL_Quit ();

	return 0;
}

// ==== Input ====

static void input_keyDown (SDLKey k) {
	keys_down[k] = 1;
	if (k == SDLK_ESCAPE || k == SDLK_q)
		user_exit = 1;
}

static void input_keyUp (SDLKey k) {
	keys_down[k] = 0;
}

static void input_mouseMove (int xPos, int yPos) {
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
static void input_update () {
	// WASD movement
	if(keys_down[SDLK_w])
		camera_translateForward(-1);
	if(keys_down[SDLK_s])
		camera_translateForward(1);
	if(keys_down[SDLK_a])
		camera_translateStrafe(-1);
	if(keys_down[SDLK_d])
		camera_translateStrafe(1);

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
		VectorClear(camera.angleRad);
		VectorClear(camera.position);
	}

	// Set movement to free or person
	if (keys_down[SDLK_f])
		free_move = 1;
	else if (keys_down[SDLK_p])
		free_move = 0;
}

// ==== openGL setup ====

/*
 * Sets up the window
 */
static void r_init () {

	ratio = WINDOW_WIDTH;
	ratio /= 1.0f * WINDOW_HEIGHT;

	glEnable (GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_TEXTURE_2D);

	// Load model
	char name[] = "models/submarine.ASE";
	renderer_model_loadASE(name, efalse);

	camera_init();
	r_setupProjection();
}

static void r_setupProjection() {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(90.0, ratio, 0.5, 1024.0);
}

static void r_setupModelview() {
	float sinX, cosX, sinY, cosY, sinZ, cosZ;
	sinX = sin(camera.angleRad[_X]);
	sinY = sin(camera.angleRad[_Y]);
	sinZ = sin(camera.angleRad[_Z]);
	cosX = cos(camera.angleRad[_X]);
	cosY = cos(camera.angleRad[_Y]);
	cosZ = cos(camera.angleRad[_Z]);

	xRotMatrix[5] = cosX;
	xRotMatrix[6] = sinX;
	xRotMatrix[9] = -sinX;
	xRotMatrix[10] = cosX;

	yRotMatrix[0] = cosY;
	yRotMatrix[2] = sinY;
	yRotMatrix[8] = -sinY;
	yRotMatrix[10] = cosY;

	zRotMatrix[0] = cosZ;
	zRotMatrix[1] = -sinZ;
	zRotMatrix[4] = sinZ;
	zRotMatrix[5] = cosZ;

	translateMatrix[12] = -camera.position[_X];
	translateMatrix[13] = -camera.position[_Y];
	translateMatrix[14] = -camera.position[_Z];

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glMultMatrixf(flipMatrix);
	glMultMatrixf(xRotMatrix);
	glMultMatrixf(zRotMatrix);
	glMultMatrixf(translateMatrix);
}

// ==== Drawing ====

/*
 * Draws a frame
 */
static void r_drawFrame () {
	int i, j, k;
    float offsets[3] = {2*size,0,-2*size};

	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	r_setupModelview();

	renderer_model_drawASE(0);

	SDL_GL_SwapBuffers ();
}

// ==== Camera ====

// Reset the camera matrices
static void camera_init() {
	glmatrix_identity(xRotMatrix);
	glmatrix_identity(yRotMatrix);
	glmatrix_identity(zRotMatrix);
	glmatrix_identity(translateMatrix);
}

static void camera_rotateX (float degree) {
	camera.angleRad[_X] += degree * M_PI_DIV180;
	if (degree > 0 && camera.angleRad[_X] > M_PI_DIV2)
		camera.angleRad[_X] = M_PI_DIV2;
	else if (degree < 0 && camera.angleRad[_X] < -M_PI_DIV2)
		camera.angleRad[_X] = -M_PI_DIV2;
}

static void camera_rotateY (float degree) {
	camera.angleRad[_Y] += degree * M_PI_DIV180;
}

static void camera_rotateZ (float degree) {
	camera.angleRad[_Z] += degree * M_PI_DIV180;
}

static void camera_translateForward(float dist) {
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

static void camera_translateStrafe(float dist) {
	float phi = camera.angleRad[_Z] + 3.14159/2.0;
	float cosPhi = cos(-phi), sinPhi = sin(-phi);
	// Person or Free
	camera.position[_X] -= dist * sinPhi;
	camera.position[_Y] -= dist * cosPhi;
}

