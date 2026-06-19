#pragma once
#include <SDL.h>

class Event
{
public:

	static bool WindowEvent()
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
};