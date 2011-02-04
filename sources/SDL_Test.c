#include <SDL/SDL.h>
#include <SDL/SDL_main.h>
#include <SDL/SDL_opengl.h>
#include <stdio.h>

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
static void cube(float size);

static float mouse_x = 0.0f, mouse_y = 0.0f;
static float ratio, dist = 5.0f, size = 2.0f;

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
		// Cap it at 100 fps
		usleep(10000);
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
    mouse_x = ((2.0f* xPos)/WINDOW_WIDTH - 1.0f) * ratio;
    mouse_y = (2.0f* yPos)/WINDOW_HEIGHT - 1.0f;
}

/*
 * Reads keyboard input and updates scene variables.
 * 'w' decreases the distance of the model
 * 's' increases the distance of the model
 * 'a' increases the size of the model
 * 'd' decreases the size of the model
 */
static void input_update() {
    if(keys_down['w']) {
        dist /= 1.01;
        if (dist <= 1.00f)
            dist = 1.00f;
    } else if(keys_down['s']) {
        dist *= 1.01;
        if (dist >= 10.0f)
            dist = 10.00f;
    }
    if (keys_down['a']) {
    	size += 1.0f / (1 << 8);
    	if (size >= 4.0f)
    		size = 4.0f;
    } else if (keys_down['d']) {
    	size -= 1.0f / (1 << 8);
    	if (size <= 1.0f)
    		size = 1.0f;
    }
}

/*
 * Sets up the window
 */
static void r_init () {
	ratio = WINDOW_WIDTH;
	ratio /= 1.0f * WINDOW_HEIGHT;

	glEnable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(90.0, ratio, 0.5, 20.0);
	glTranslatef(0,0,-dist);
	glPushMatrix();
}

/*
 * Draws a frame
 */
static void r_drawFrame () {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	float z = -dist;
	float dx = mouse_x * dist;
	float dy = -mouse_y * dist;

    glPushMatrix();
	glTranslatef(0,0,-dist);
    glRotatef(180*mouse_x,0,1,0);
    glRotatef(180*mouse_y,1,0,0);
	cube (size);
	glPopMatrix();

	SDL_GL_SwapBuffers();
}

static void cube (float size) {
	float side = size * 0.5f;
	// Front face
	glColor3f(1,0,0);
	glBegin(GL_POLYGON);
		glVertex3f(-side, -side, side);
		glVertex3f( side, -side, side);
		glVertex3f( side,  side, side);
		glVertex3f(-side,  side, side);
	glEnd();
	// Back face
	glColor3f(1.0f, 0.25f, 0.25f);
	glBegin(GL_POLYGON);
		glVertex3f (side, -side, -side);
		glVertex3f(-side, -side, -side);
		glVertex3f(-side,  side, -side);
		glVertex3f( side,  side, -side);
	glEnd();
	// Left face
	glColor3f(0,0,1);
	glBegin(GL_POLYGON);
		glVertex3f(-side, -side,  side);
		glVertex3f(-side,  side,  side);
		glVertex3f(-side,  side, -side);
		glVertex3f(-side, -side, -side);
	glEnd();
	// Right face
	glColor3f(0.5,0.5,0.5);
	glBegin(GL_POLYGON);
		glVertex3f(side, -side, side);
		glVertex3f(side,  side, side);
		glVertex3f(side,  side, -side);
		glVertex3f(side, -side, -side);
	glEnd();
	// Bottom face
	glColor3f(0,1,0);
	glBegin(GL_POLYGON);
		glVertex3f(-side, -side,  side);
		glVertex3f( side, -side,  side);
		glVertex3f( side, -side, -side);
		glVertex3f(-side, -side, -side);
	glEnd();
	// Top face
	glColor3f(0.5,1,0.5);
	glBegin(GL_POLYGON);
		glVertex3f(-side, side,  side);
		glVertex3f( side, side,  side);
		glVertex3f( side, side, -side);
		glVertex3f(-side, side, -side);
	glEnd();
}
