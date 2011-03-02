/*
===========================================================================
File:		renderer_materials.h
Author: 	Clinton Freeman
Created on: Feb 19, 2011
===========================================================================
*/

#ifndef RENDERER_MATERIALS_H_
#define RENDERER_MATERIALS_H_

#define MAX_TEXTURES 512

int renderer_img_createMaterial(char *name, vect3_t ambient, vect3_t diffuse, vect3_t specular,
		float shine, float shineStrength, float transparency);

int renderer_img_getMatGLID(int i);
int renderer_img_getMatWidth(int i);
int renderer_img_getMatHeight(int i);
int renderer_img_getMatBpp(int i);

#endif /* RENDERER_MATERIALS_H_ */
