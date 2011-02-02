#include <SDL/SDL.h>
#include <SDL/SDL_main.h>
#include <SDL/SDL_opengl.h>
#include <stdio.h>
#include <sys/time.h>

// Packages needed:
// libsdl-ttf2.0-dev libboost-dev build-essential libsdl1.2-dev libsdl-image1.2-dev libsdl-mixer1.2-dev

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

static int user_exit = 0;

static void r_init();
static void r_drawFrame();

static int keys_down[256];
static void input_update();
static void input_keyDown(SDLKey k);
static void input_keyUp(SDLKey k);
static void input_mouseMove(int xPos, int yPos);

static float mouse_x = 0.0f, mouse_y = 0.0f;
static float ratio, dist = 5.0f;

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
				input_keyDown(event.key.keysym.sym);
				break;
			case SDL_KEYUP:
				input_keyUp(event.key.keysym.sym);
				break;
			case SDL_MOUSEMOTION:
				input_mouseMove(event.motion.x, event.motion.y);
				break;
			case SDL_QUIT:
				exit(0);
			}
		}
		input_update();
		r_drawFrame();
	}

	SDL_Quit();

	return 0;
}

static void input_keyDown (SDLKey k) {
	keys_down[k] = 1;
	if (k == SDLK_ESCAPE || k == SDLK_q)
		user_exit = 1;
}

static void input_keyUp (SDLKey k) {
	keys_down[k] = 0;
}

static void input_mouseMove(int xPos, int yPos) {
	float halfWinWidth, halfWinHeight,
		  dx, dy;

	halfWinWidth  = (float)WINDOW_WIDTH  / 2.0;
	halfWinHeight = (float)WINDOW_HEIGHT / 2.0;

	dx = xPos - halfWinWidth;
    dy = yPos - halfWinHeight;

	//Do something with deltas.
    //mouse_x = dx / WINDOW_WIDTH;
    //mouse_y = dy / WINDOW_HEIGHT;
    mouse_x = ((2.0f* xPos)/WINDOW_WIDTH - 1.0f) * ratio;
    mouse_y = (2.0f* yPos)/WINDOW_HEIGHT - 1.0f;

	//Reset cursor to center
	//SDL_WarpMouse(halfWinWidth, halfWinHeight);
}

static void input_update() {
    if(keys_down['w']) {
        dist /= 1.02;
        if (dist <= 1.00f)
            dist = 0.999f;
    }
    if(keys_down['a']) {
        dist *= 1.02;
        if (dist >= 10.0f)
            dist = 9.999f;
    }
}

static void r_init () {
	ratio = WINDOW_WIDTH;
	ratio /= 1.0f * WINDOW_HEIGHT;
	glEnable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(90.0, ratio, 0.5, 11.0);
}

static void r_drawFrame () {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	float z = -dist;
	float dx = mouse_x * dist;
	float dy = -mouse_y * dist;

	glBegin(GL_POLYGON);
		glColor3f(1,1,0);
		glVertex3f(-0.5 + dx, -0.5 + dy, z);
		glColor3f(1,0,1);
		glVertex3f(0.5 + dx, -0.5 + dy, z);
		glColor3f(0,1,1);
		glVertex3f(0.0 + dx, 0.5 + dy, z);
	glEnd();

	SDL_GL_SwapBuffers();
}
