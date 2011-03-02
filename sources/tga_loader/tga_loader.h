#ifndef TGA_LOADER_H
#define TGA_LOADER_H

#define HEADER_SIZE 18

//typedef unsigned char byte;

typedef struct _IO_FILE FILE;

typedef struct {
	unsigned char 	idLength, colormapType, imageType;
	unsigned char	colormapSize;
	unsigned short	colormapIndex, colormapLength;
	unsigned short	xOrigin, yOrigin, width, height;
	unsigned char	pixelSize, attributes;
} tgaHeader_t;

void r_image_loadTGA(char *name, int *glTexID, int *width, int *height, int *bpp);

#endif
