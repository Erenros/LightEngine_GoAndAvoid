#include "Event.h"

#include <SDL.h>

bool Event::WindowEvent()
{
	SDL_Event event;

	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_QUIT)
		{
			return true;
		}
	}
	return false;
}


