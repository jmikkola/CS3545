/*
 * render.h
 *
 *  Created on: Mar 3, 2011
 *      Author: jeremy
 */

#ifndef RENDER_H_
#define RENDER_H_

typedef struct {
	int *num, *width, *height, *BPP;
} TextureInfo;

void r_setupProjection ();
void r_translateCamera ();
void r_rotateCamera ();
void r_init ();
void r_drawFrame ();
TextureInfo load_texture (char* filename);
void draw_skybox ();
void draw_weapon ();


#endif /* RENDER_H_ */
