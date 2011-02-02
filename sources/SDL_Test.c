#include <SDL/SDL.h>
#include <SDL/SDL_main.h>
#include <SDL/SDL_opengl.h>
#include <stdio.h>
#include <sys/time.h>

// Packages needed:
// libsdl-ttf2.0-dev libboost-dev build-essential libsdl1.2-dev libsdl-image1.2-dev libsdl-mixer1.2-dev

static int user_exit = 0;

static void r_init();
static void r_drawFrame();

int main(int argc, char* argv[]) {
	SDL_Event		event;
	SDL_Surface		*screen;

	if(SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER) != 0) {
		printf("Unable to initialize SDL: %s\n", SDL_GetError());
		return 1;
	}

	SDL_WM_SetCaption("SDL Window", "SDL Window");
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	//Initialize window
	screen = SDL_SetVideoMode(800, 600, 32, SDL_OPENGL);
	if(!screen) {
		printf("Unable to set video mode: %s\n", SDL_GetError());
		return 1;
	}

	r_init();

	//Main loop
	while(!user_exit) {
		//Handle input
		while(SDL_PollEvent(&event)) {
			switch(event.type) {
			case SDL_KEYDOWN:
			case SDL_KEYUP:
			case SDL_MOUSEMOTION:
				break;
			case SDL_QUIT:
				exit(0);
			}
		}
		r_drawFrame();
	}

	SDL_Quit();

	return 0;
}

static void r_init () {
}

static void r_drawFrame () {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColor3f(1.0, 1.0, 1.0);

	glBegin(GL_POLYGON);
		glColor3f(1,1,0);
		glVertex3f(-0.5, -0.5, 0.0);
		glColor3f(1,0,1);
		glVertex3f(0.5, -0.5, 0.0);
		glColor3f(0,1,1);
		glVertex3f(0.0, 0.5, 0.0);
	glEnd();

	SDL_GL_SwapBuffers();
}
