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
#include "camera.h"
#include "render.h"
#include "input.h"

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

	load_skybox_textures();
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

