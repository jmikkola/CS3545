/*
===========================================================================
File:		renderer_materials.c
Author: 	Clinton Freeman
Created on: Oct 8, 2010
Notes:		Please remember that this is, for the moment, basically
			just ripped directly out of my project, and so you will
			need to do some re-working to fit it into your project.
===========================================================================
*/

#include <SDL/SDL_opengl.h>

#include "../common.h"
#include "../mathlib/mathlib.h"
#include "../materials/renderer_materials.h"
#include "../tga_loader/tga_loader.h"

typedef struct
{
	int		glTexID;
	char	name[128];
	vect3_t	ambient, diffuse, specular;
	float	shine, shineStrength, transparency;
	int		width, height, bpp;
}
material_t;

static material_t materialList[MAX_TEXTURES];
static int stackPtr = 0;

/*
 * renderer_img_createMaterial
 */
int renderer_img_createMaterial(char *name, vect3_t ambient, vect3_t diffuse, vect3_t specular,
		float shine, float shineStrength, float transparency)
{
	material_t *currentMat = &materialList[stackPtr];

	currentMat->shine 			= shine;
	currentMat->shineStrength 	= shineStrength;
	currentMat->transparency 	= transparency;

	strcpy(currentMat->name, name);

	VectorCopy(ambient,  currentMat->ambient);
	VectorCopy(diffuse,  currentMat->diffuse);
	VectorCopy(specular, currentMat->specular);

	r_image_loadTGA(name, &(currentMat->glTexID),
			&(currentMat->width), &(currentMat->height), &(currentMat->bpp));

	return stackPtr++;
}

int renderer_img_getMatGLID  (int i) { return materialList[i].glTexID; }
int renderer_img_getMatWidth (int i) { return materialList[i].width;   }
int renderer_img_getMatHeight(int i) { return materialList[i].height;  }
int renderer_img_getMatBpp   (int i) { return materialList[i].bpp;     }
