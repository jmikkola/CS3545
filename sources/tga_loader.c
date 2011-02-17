/*
 * tga_loader.c
 *
 *  Created on: Feb 16, 2011
 *      Author: jeremy
 */
#include "tga_loader.h"
#include <SDL/SDL_opengl.h>
#include <sys/stat.h>


/*
 * Function: renderer_img_loadTGA
 * Description: Loads a TARGA image file, uploads to GL, and returns the
 * texture ID. Only supports 24/32 bit.
 */
void r_image_loadTGA(char *name, int *glTexID, int *width, int *height, int *bpp){
	int				dataSize, rows, cols, i, j;
	GLuint			type;
	byte			*buf, *imageData, *pixelBuf, red, green, blue, alpha;

	FILE 			*file;
	tgaHeader_t		header;
	struct stat 	st;

	file = fopen(name, "rb");

	if(file == NULL) {
		printf("Loading TGA: %s, failed. Null file pointer.\n", name);
		return;
	}

	if(stat(name, &st)) {
		printf("Loading TGA: %s, failed. Could not determine file size.\n", name);
		return;
	}

	if(st.st_size < HEADER_SIZE) {
		printf("Loading TGA: %s, failed. Header too short.\n", name);
		return;
	}

	buf = (byte *)malloc(st.st_size);
	fread(buf, sizeof(byte), st.st_size, file);

	fclose(file);

	memcpy(&header.idLength, 	 	&buf[0],  1);
	memcpy(&header.colormapType, 	&buf[1],  1);
	memcpy(&header.imageType, 		&buf[2],  1);
	memcpy(&header.colormapIndex, 	&buf[3],  2);
	memcpy(&header.colormapLength,  &buf[5],  2);
	memcpy(&header.colormapSize, 	&buf[7],  1);
	memcpy(&header.xOrigin,			&buf[8],  2);
	memcpy(&header.yOrigin,			&buf[10], 2);
	memcpy(&header.width,			&buf[12], 2);
	memcpy(&header.height,			&buf[14], 2);
	memcpy(&header.pixelSize,		&buf[16], 1);
	memcpy(&header.attributes,		&buf[17], 1);

	//Advance past the header
	buf += HEADER_SIZE;

	if(header.pixelSize != 24 && header.pixelSize != 32) {
		printf("Loading TGA: %s, failed. Only support 24/32 bit images.\n", name);
		return;
	}
	else if(header.pixelSize == 24)
		type = GL_RGB;
	else
		type = GL_RGBA;

	//Determine size of image data chunk in bytes
	dataSize = header.width * header.height * (header.pixelSize / 8);

	//Set up our texture
	*bpp 	 	= header.pixelSize;
	*width  	= header.width;
	*height 	= header.height;

	imageData = (byte *)malloc(dataSize);
	rows	  = *height;
	cols	  = *width;

	if(type == GL_RGB) {
		for(i = 0; i < rows; i++) {
			pixelBuf = imageData + (i * cols * 3);
			for(j = 0; j < cols; j++) {
				blue 	= *buf++;
				green 	= *buf++;
				red		= *buf++;

				*pixelBuf++ = red;
				*pixelBuf++ = green;
				*pixelBuf++ = blue;
			}
		}
	} else {
		for(i = 0; i < rows; i++) {
			pixelBuf = imageData + (i * cols * 4);
			for(j = 0; j < cols; j++) {
				blue 	= *buf++;
				green 	= *buf++;
				red		= *buf++;
				alpha	= *buf++;

				*pixelBuf++ = red;
				*pixelBuf++ = green;
				*pixelBuf++ = blue;
				*pixelBuf++ = alpha;
			}
		}
	}

	//Upload the texture to OpenGL
	glGenTextures(1, glTexID);
	glBindTexture(GL_TEXTURE_2D, *glTexID);

	//Default OpenGL settings have GL_TEXTURE_MAG/MIN_FILTER set to use
	//mipmaps... without these calls texturing will not work properly.
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	//Upload image data to OpenGL
	glTexImage2D(GL_TEXTURE_2D, 0, type, *width, *height,
			0, type, GL_UNSIGNED_BYTE, imageData);

	//Header debugging
	/*
	printf("Attributes: %d\n", 				header.attributes);
	printf("Colormap Index: %d\n", 			header.colormapIndex);
	printf("Colormap Length: %d\n", 		header.colormapLength);
	printf("Colormap Size: %d\n", 			header.colormapSize);
	printf("Colormap Type: %d\n", 			header.colormapType);
	printf("Height: %d\n", 					header.height);
	printf("Identification Length: %d\n",	header.idLength);
	printf("Image Type: %d\n", 				header.imageType);
	printf("Pixel Size: %d\n", 				header.pixelSize);
	printf("Width: %d\n", 					header.width);
	printf("X Origin: %d\n", 				header.xOrigin);
	printf("Y Origin: %d\n", 				header.yOrigin);
	*/
}
