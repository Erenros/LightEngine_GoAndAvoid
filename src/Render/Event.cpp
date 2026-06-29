#include "Event.h"

#include <SDL.h>

#include <imgui.h>
#include <backends/imgui_impl_sdl2.h> 

bool Event::WindowEvent()
{
	SDL_Event event;

	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_QUIT)
		{
			return true;
		}

		//ImGui_ImplSDL2_ProcessEvent(&event);
	}
	return false;
}


