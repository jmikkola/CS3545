#include <SDL/SDL.h>
#include <SDL/SDL_main.h>
#include <SDL/SDL_opengl.h>
#include <stdio.h>

// Packages needed:
// libsdl-ttf2.0-dev libboost-dev build-essential libsdl1.2-dev libsdl-image1.2-dev libsdl-mixer1.2-dev

static int user_exit = 0;

int main(int argc, char* argv[])
{
	SDL_Event		event;
	SDL_Surface		*screen;

	if(SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER) != 0)
	{
		printf("Unable to initialize SDL: %s\n", SDL_GetError());
		return 1;
	}

	SDL_WM_SetCaption("SDL Window", "SDL Window");
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	//Initialize window
	screen = SDL_SetVideoMode(1024, 768, 32, SDL_OPENGL);
	if(!screen)
	{
		printf("Unable to set video mode: %s\n", SDL_GetError());
		return 1;
	}

	//Main loop
	while(!user_exit)
	{
		//Handle input
		while(SDL_PollEvent(&event))
		{
			switch(event.type)
			{
			case SDL_KEYDOWN:
			case SDL_KEYUP:
			case SDL_MOUSEMOTION:
				break;
			case SDL_QUIT:
				exit(0);
			}
		}
	}

	SDL_Quit();

	return 0;
}
