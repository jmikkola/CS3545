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
#include "camera.h"
#include "render.h"

const GLfloat flipMatrix[16] =
		{1.0, 0.0,  0.0, 0.0,
		 0.0, 0.0, -1.0, 0.0,
		 0.0, 1.0,  0.0, 0.0,
		 0.0, 0.0,  0.0, 1.0};
int *skyboxTexture, *skyboxTexWidth, *skyboxTexHeight, *skyboxTextBPP;
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

	// Load model
	char name[] = "models/submarine.ASE";
	renderer_model_loadASE(name, efalse);

	camera_init();
	reset_position();
	r_setupProjection();
}

void load_skybox_textures() {
	skyboxTexture = calloc(sizeof(int), 6);
	skyboxTexWidth = calloc(sizeof(int), 6);
	skyboxTexHeight = calloc(sizeof(int), 6);
	skyboxTextBPP = calloc(sizeof(int), 6);
	r_image_loadTGA("images/skybox003.tga",
				&skyboxTexture[0], &skyboxTexWidth[0], &skyboxTexHeight[0], &skyboxTextBPP[0]);
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

	r_rotateCamera();
	draw_skybox();

	r_translateCamera();
	renderer_model_drawASE(0);

	SDL_GL_SwapBuffers ();
}

void draw_skybox() {
	float x1, x2, y1, y2;
	// Setup
	glPushMatrix();
	glPushAttrib(GL_ENABLE_BIT);

	glEnable(GL_TEXTURE_2D);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glDisable(GL_BLEND);
	glDisable(GL_CULL_FACE); // For debugging only
    glColor4f(1,1,1,1);

    // Side 0 - bottom
    x1 = 1/4.0f;
    x2 = 2/4.0f;
    y1 = 3/4.0f;
    y2 = 2/4.0f;
    glBindTexture(GL_TEXTURE_2D, skyboxTexture[0]);
    glBegin(GL_QUADS);
		glTexCoord2f(x1, y1); glVertex3f(  1.0f, -1.0f, -1.0f );
		glTexCoord2f(x2, y1); glVertex3f( -1.0f, -1.0f, -1.0f );
		glTexCoord2f(x2, y2); glVertex3f( -1.0f,  1.0f, -1.0f );
		glTexCoord2f(x1, y2); glVertex3f(  1.0f,  1.0f, -1.0f );
    glEnd();

    // Side 1 - right
    x1 = 0/4.0f;
    x2 = 1/4.0f;
    y1 = 3/4.0f;
    y2 = 2/4.0f;
    glBindTexture(GL_TEXTURE_2D, skyboxTexture[0]);
    glBegin(GL_QUADS);
		glTexCoord2f(x1, y1); glVertex3f(  1.0f, -1.0f,  1.0f );
		glTexCoord2f(x2, y1); glVertex3f(  1.0f, -1.0f, -1.0f );
		glTexCoord2f(x2, y2); glVertex3f(  1.0f,  1.0f, -1.0f );
		glTexCoord2f(x1, y2); glVertex3f(  1.0f,  1.0f,  1.0f );
    glEnd();

    // Side 2 - top
    x1 = 3/4.0f;
    x2 = 4/4.0f;
    y1 = 3/4.0f;
    y2 = 2/4.0f;
    glBindTexture(GL_TEXTURE_2D, skyboxTexture[0]);
    glBegin(GL_QUADS);
		glTexCoord2f(x1, y1); glVertex3f( -1.0f, -1.0f,  1.0f );
		glTexCoord2f(x2, y1); glVertex3f(  1.0f, -1.0f,  1.0f );
		glTexCoord2f(x2, y2); glVertex3f(  1.0f,  1.0f,  1.0f );
		glTexCoord2f(x1, y2); glVertex3f( -1.0f,  1.0f,  1.0f );
    glEnd();

    // Side 3 - left
    x1 = 2/4.0f;
    x2 = 3/4.0f;
    y1 = 3/4.0f;
    y2 = 2/4.0f;
    glBindTexture(GL_TEXTURE_2D, skyboxTexture[0]);
    glBegin(GL_QUADS);
		glTexCoord2f(x1, y1); glVertex3f( -1.0f, -1.0f, -1.0f );
		glTexCoord2f(x2, y1); glVertex3f( -1.0f, -1.0f,  1.0f );
		glTexCoord2f(x2, y2); glVertex3f( -1.0f,  1.0f,  1.0f );
		glTexCoord2f(x1, y2); glVertex3f( -1.0f,  1.0f, -1.0f );
    glEnd();

    // Side 4 - front
    x1 = 1/4.0f;
    x2 = 2/4.0f;
    y1 = 1/4.0f;
    y2 = 2/4.0f;
    glBindTexture(GL_TEXTURE_2D, skyboxTexture[0]);
    glBegin(GL_QUADS);
		glTexCoord2f(x2, y2); glVertex3f( -1.0f,  1.0f, -1.0f );
		glTexCoord2f(x2, y1); glVertex3f( -1.0f,  1.0f,  1.0f );
		glTexCoord2f(x1, y1); glVertex3f(  1.0f,  1.0f,  1.0f );
		glTexCoord2f(x1, y2); glVertex3f(  1.0f,  1.0f, -1.0f );
    glEnd();

    // Side 5
    x1 = 1/4.0f;
    x2 = 2/4.0f;
    y1 = 3/4.0f;
    y2 = 4/4.0f;
    glBindTexture(GL_TEXTURE_2D, skyboxTexture[0]);
    glBegin(GL_QUADS);
		glTexCoord2f(x2, y1); glVertex3f( -1.0f, -1.0f, -1.0f );
		glTexCoord2f(x2, y2); glVertex3f( -1.0f, -1.0f,  1.0f );
		glTexCoord2f(x1, y2); glVertex3f(  1.0f, -1.0f,  1.0f );
		glTexCoord2f(x1, y1); glVertex3f(  1.0f, -1.0f, -1.0f );
    glEnd();

    // Clean up
	glClear(GL_DEPTH_BUFFER_BIT);
    glPopAttrib();
    glPopMatrix();
}
