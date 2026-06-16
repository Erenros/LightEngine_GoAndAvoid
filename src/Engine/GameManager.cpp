#include "GameManager.h"
#include <iostream>

void GameManager::Loop()
{
	isRunning = true;

	while (isRunning == true)
	{
		//TODO Loop

		SDL_Delay(3000);
		isRunning = false;
	}
}

bool GameManager::Init()
{
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) != 0)
	{
		std::cout << "SDL_Init_Error :  " << SDL_GetError() << std::endl;
		return false;
	}

	if (IMG_Init(IMG_INIT_PNG) == 0) {
		std::cout << "Error SDL2_image Initialization";
		return false;
	}


	// Temporaire En attente de la class Window
	SDL_CreateWindowAndRenderer(m_WindW, m_WindH, SDL_WINDOW_RESIZABLE, &mp_Window, &mp_Renderer);

	if (!mp_Window)
	{
		std::cout << "[Initialisation] : SDL_CreateWindow Error : " << SDL_GetError() << std::endl;
		Close();
		return false;
	}

	if (!mp_Renderer)
	{
		std::cout << "[Initialisation] : Renderer Error : " << SDL_GetError() << std::endl;
		Close();
		return false;
	}
	///////////////////////////////////////////


	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	{
		std::cout << "[Initialisation] : Audio Error : " << SDL_GetError() << std::endl;
		Close();
		return false;
	}

	if (TTF_Init() != 0)
	{
		std::cout << "[Initialisation] : Font Error" << std::endl;
		Close();
		return false;
	}

	return true;
}

void GameManager::Close()
{
	// Temporaire En attente de la class Window
	SDL_DestroyRenderer(mp_Renderer);
	SDL_DestroyWindow(mp_Window);
	///////////////////////////////////////////

	Mix_CloseAudio();
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
	SDL_Quit();
}

void GameManager::LaunchGame()
{
	if (!Init()) return;
	
	Loop();
	Close();
}