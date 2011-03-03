/*
 * render.h
 *
 *  Created on: Mar 3, 2011
 *      Author: jeremy
 */

#ifndef RENDER_H_
#define RENDER_H_

void r_setupProjection();
void r_translateCamera();
void r_rotateCamera();
void r_init ();
void r_drawFrame ();
void load_skybox_textures();
void draw_skybox ();

#endif /* RENDER_H_ */
