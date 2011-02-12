/*
 * File: SDL_Test.c
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
#include "mathlib/mathlib.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600


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
static int keys_down[256];
static float ratio, size = 2.0f;
//Maintain a matrix for each rotation and one for translation
static float xRotMatrix[16],
			 yRotMatrix[16],
			 zRotMatrix[16],
			 translateMatrix[16];


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
	camera_rotateY(-dx * 0.5f);
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
		camera_translateForward(-0.05);
	if(keys_down[SDLK_s])
		camera_translateForward(0.05);
	if(keys_down[SDLK_a])
		camera_translateStrafe(-0.05);
	if(keys_down[SDLK_d])
		camera_translateStrafe(0.05);

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
	glEnable (GL_TEXTURE_2D);

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
	glMultMatrixf(zRotMatrix);
	glMultMatrixf(xRotMatrix);
	glMultMatrixf(yRotMatrix);
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

	// Set up base matrix

	// Cube 1
    for (i = 0; i < 3; i++) {
    	for (j = 0; j < 3; j++) {
    		if (j == 1 && i == 1) continue;
    		for (k = 0; k < 3; k++) {
    			if (k == 1 && (i == 1 || j == 1)) continue;
    			cube (size, offsets[i], offsets[j], offsets[k]);
    		}
    	}
    }

	SDL_GL_SwapBuffers ();
}

static void cube (float size, float offsetX, float offsetY, float offsetZ) {
	glPushMatrix ();
	glTranslatef (offsetX, offsetY, offsetZ);
	float side = size * 0.5f;
	// Front face
	glBegin (GL_POLYGON);
		glColor3f (1,0,0);
		glVertex3f (-side, -side, side);
		glColor3f (1,1,0);
		glVertex3f ( side, -side, side);
		glColor3f (0,1,0);
		glVertex3f ( side,  side, side);
		glColor3f (0,0,0);
		glVertex3f (-side,  side, side);
	glEnd ();
	// Back face
	glColor3f (0,1,1);
	glBegin (GL_POLYGON);
		glVertex3f (side, -side, -side);
		glVertex3f (-side, -side, -side);
		glVertex3f (-side,  side, -side);
		glVertex3f ( side,  side, -side);
	glEnd ();
	// Left face
	glColor3f (0,1,0);
	glBegin (GL_POLYGON);
		glVertex3f (-side, -side,  side);
		glVertex3f (-side,  side,  side);
		glVertex3f (-side,  side, -side);
		glVertex3f (-side, -side, -side);
	glEnd ();
	// Right face
	glColor3f (1,0,1);
	glBegin (GL_POLYGON);
		glVertex3f (side, -side, side);
		glVertex3f (side,  side, side);
		glVertex3f (side,  side, -side);
		glVertex3f (side, -side, -side);
	glEnd ();
	// Bottom face
	glColor3f (0,0,1);
	glBegin (GL_POLYGON);
		glVertex3f (-side, -side,  side);
		glVertex3f ( side, -side,  side);
		glVertex3f ( side, -side, -side);
		glVertex3f (-side, -side, -side);
	glEnd ();
	// Top face
	glColor3f (1,1,0);
	glBegin (GL_POLYGON);
		glVertex3f (-side, side,  side);
		glVertex3f ( side, side,  side);
		glVertex3f ( side, side, -side);
		glVertex3f (-side, side, -side);
	glEnd ();

	glPopMatrix ();
}

// ==== Camera ====

// Reset the camera matrices
static void camera_init() {
	glmatrix_identity(xRotMatrix);
	glmatrix_identity(yRotMatrix);
	glmatrix_identity(zRotMatrix);
	glmatrix_identity(translateMatrix);
}

//Rotations just increase/decrease the angle and compute a new radian value.
static void camera_rotateX (float degree) {
	camera.angleRad[_X] += degree * M_PI_DIV180;
}

static void camera_rotateY (float degree) {
	camera.angleRad[_Y] += degree * M_PI_DIV180;
}

static void camera_rotateZ (float degree) {
	camera.angleRad[_Z] += degree * M_PI_DIV180;
}

static void camera_translateForward(float dist) {
	float theta = camera.angleRad[_X], phi = camera.angleRad[_Y];
	if (free_move) {
		// Free movement
		camera.position[_X] += dist * cos(theta) * sin(phi);
		camera.position[_Y] += dist * sin(theta);
		camera.position[_Z] += dist * cos(theta) * cos(phi);
	} else {
		// Person movement
		camera.position[_X] +=  dist * sin(phi);
		camera.position[_Y] +=  0;
		camera.position[_Z] +=  dist * cos(phi);
	}
}

static void camera_translateStrafe(float dist) {
	float phi = camera.angleRad[_Y] + 3.14159/2.0;
	// Person or Free
	camera.position[_X] += dist * sin(phi);
	camera.position[_Z] += dist * cos(phi);
}

