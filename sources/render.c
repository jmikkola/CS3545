/*
 * render.c
 *
 *  Created on: Mar 3, 2011
 *      Author: jeremy
 */
#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>
#include "common.h"
#include "mathlib/mathlib.h"
#include "models/renderer_models.h"
#include "camera.h"
#include "render.h"
#include "math.h"
#include "timestep.h"

#include <stdio.h>

const GLfloat flipMatrix[16] =
		{1.0, 0.0,  0.0, 0.0,
		 0.0, 0.0, -1.0, 0.0,
		 0.0, 1.0,  0.0, 0.0,
		 0.0, 0.0,  0.0, 1.0};
TextureInfo skybox, weapon;
float ratio;

/*
 * Sets up the window
 */
void r_init () {

	ratio = WINDOW_WIDTH;
	ratio /= 1.0f * WINDOW_HEIGHT;

	glEnable (GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Load model
	renderer_model_loadASE("models/submarine.ASE", efalse);

	// Load textures
	skybox = load_texture("images/skybox003.tga");
	weapon = load_texture("images/weapon001.tga");

	camera_init();
	reset_position();
	r_setupProjection();

	initTime();
}

TextureInfo load_texture (char* filename) {
	TextureInfo texInfo;
	r_image_loadTGA(filename,
			&texInfo.num, &texInfo.width, &texInfo.height, &texInfo.BPP);
	return texInfo;
}

void r_setupProjection() {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(90.0, ratio, 0.5, 1024.0);
}

void r_rotateCamera() {
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

	glMultMatrixf(flipMatrix);
	glMultMatrixf(xRotMatrix);
	glMultMatrixf(zRotMatrix);
}

void r_translateCamera() {
	translateMatrix[12] = -camera.position[_X];
	translateMatrix[13] = -camera.position[_Y];
	translateMatrix[14] = -camera.position[_Z];

	glMultMatrixf(translateMatrix);
}

// ==== Drawing ====

/*
 * Draws a frame
 */
void r_drawFrame () {
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	timeStep();
	if (getAcc() > 1000000) {
		printf("%u FPS\n", getFrameCount());
		subtractAcc(1000000);
	}

	// Draw skybox
	glPushMatrix();
	r_rotateCamera();
	draw_skybox();
	glClear(GL_DEPTH_BUFFER_BIT);

	// Draw scene
	r_translateCamera();
	renderer_model_drawASE(0);
	glPopMatrix();
	glClear(GL_DEPTH_BUFFER_BIT);

	// Draw weapon
	draw_weapon();

	SDL_GL_SwapBuffers ();
}

void draw_skybox() {
	float x1, x2, y1, y2;
	int textureID;
	// Setup
	glPushAttrib(GL_ENABLE_BIT);
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glDisable(GL_BLEND);
    glColor4f(1,1,1,1);

    // Side 0 - bottom
    x1 = 1/4.0f;
    x2 = 2/4.0f;
    y1 = 3/4.0f;
    y2 = 2/4.0f;
    textureID = skybox.num;
    glBindTexture(GL_TEXTURE_2D, textureID);
    glBegin(GL_QUADS);
		glTexCoord2f(x1, y1); glVertex3f(  1.0f, -1.0f, -1.0f );
		glTexCoord2f(x2, y1); glVertex3f(  1.0f,  1.0f, -1.0f );
		glTexCoord2f(x2, y2); glVertex3f( -1.0f,  1.0f, -1.0f );
		glTexCoord2f(x1, y2); glVertex3f( -1.0f, -1.0f, -1.0f );
    glEnd();

    // Side 1 - right
    x1 = 0/4.0f;
    x2 = 1/4.0f;
    y1 = 3/4.0f;
    y2 = 2/4.0f;
    glBindTexture(GL_TEXTURE_2D, textureID);
    glBegin(GL_QUADS);
		glTexCoord2f(x1, y1); glVertex3f(  1.0f, -1.0f,  1.0f );
		glTexCoord2f(x2, y1); glVertex3f(  1.0f,  1.0f,  1.0f );
		glTexCoord2f(x2, y2); glVertex3f(  1.0f,  1.0f, -1.0f );
		glTexCoord2f(x1, y2); glVertex3f(  1.0f, -1.0f, -1.0f );
    glEnd();

    // Side 2 - top
    x1 = 3/4.0f;
    x2 = 4/4.0f;
    y1 = 3/4.0f;
    y2 = 2/4.0f;
    glBindTexture(GL_TEXTURE_2D, textureID);
    glBegin(GL_QUADS);
		glTexCoord2f(x1, y1); glVertex3f( -1.0f, -1.0f,  1.0f );
		glTexCoord2f(x2, y1); glVertex3f( -1.0f,  1.0f,  1.0f );
		glTexCoord2f(x2, y2); glVertex3f(  1.0f,  1.0f,  1.0f );
		glTexCoord2f(x1, y2); glVertex3f(  1.0f, -1.0f,  1.0f );
    glEnd();

    // Side 3 - left
    x1 = 2/4.0f;
    x2 = 3/4.0f;
    y1 = 3/4.0f;
    y2 = 2/4.0f;
    glBindTexture(GL_TEXTURE_2D, textureID);
    glBegin(GL_QUADS);
		glTexCoord2f(x1, y1); glVertex3f( -1.0f, -1.0f, -1.0f );
		glTexCoord2f(x2, y1); glVertex3f( -1.0f,  1.0f, -1.0f );
		glTexCoord2f(x2, y2); glVertex3f( -1.0f,  1.0f,  1.0f );
		glTexCoord2f(x1, y2); glVertex3f( -1.0f, -1.0f,  1.0f );
    glEnd();

    // Side 4 - front
    x1 = 1/4.0f;
    x2 = 2/4.0f;
    y1 = 1/4.0f;
    y2 = 2/4.0f;
    glBindTexture(GL_TEXTURE_2D, textureID);
    glBegin(GL_QUADS);
		glTexCoord2f(x2, y2); glVertex3f( -1.0f,  1.0f, -1.0f );
		glTexCoord2f(x2, y1); glVertex3f(  1.0f,  1.0f, -1.0f );
		glTexCoord2f(x1, y1); glVertex3f(  1.0f,  1.0f,  1.0f );
		glTexCoord2f(x1, y2); glVertex3f( -1.0f,  1.0f,  1.0f );
    glEnd();

    // Side 5 - back
    x1 = 1/4.0f;
    x2 = 2/4.0f;
    y1 = 3/4.0f;
    y2 = 4/4.0f;
    glBindTexture(GL_TEXTURE_2D, textureID);
    glBegin(GL_QUADS);
		glTexCoord2f(x2, y1); glVertex3f( -1.0f, -1.0f, -1.0f );
		glTexCoord2f(x2, y2); glVertex3f( -1.0f, -1.0f,  1.0f );
		glTexCoord2f(x1, y2); glVertex3f(  1.0f, -1.0f,  1.0f );
		glTexCoord2f(x1, y1); glVertex3f(  1.0f, -1.0f, -1.0f );
    glEnd();

    // Clean up
    glPopAttrib();
}

void draw_weapon () {
	int textureID = weapon.num;
	float offset = ratio - 1.0;

	// Draw the shape
	glBindTexture(GL_TEXTURE_2D, textureID);
	glBegin(GL_POLYGON);
		glTexCoord2f(0,1); glVertex3f( offset,  0.0f, -1.0f);
		glTexCoord2f(0,0); glVertex3f( offset, -1.0f, -1.0f);
		glTexCoord2f(1,0); glVertex3f( ratio, -1.0f, -1.0f);
		glTexCoord2f(1,1); glVertex3f( ratio,  0.0f, -1.0f);
	glEnd();

	// Clean up
	glPopAttrib();
}
