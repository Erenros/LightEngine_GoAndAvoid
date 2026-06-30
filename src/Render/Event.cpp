#include "Event.h"

#include <SDL.h>
#include "Engine/GameManager.h"

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
		if (event.type == SDL_WINDOWEVENT)
		{
			if (event.window.event == SDL_WINDOWEVENT_RESIZED || event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
			{
				Window* pWindow = GameManager::GetInstance().GetWindow();
				pWindow->m_width = static_cast<uint32>(pWindow->GetWindowSize().x);
				pWindow->m_height = static_cast<uint32>(pWindow->GetWindowSize().y);
			}
		}

		//ImGui_ImplSDL2_ProcessEvent(&event);
	}
	return false;
}


