#include "GameManager.h"
#include <iostream>
#include "Core/InputManager.h" 
#include "Entity.h"
#include "Scene.h" 
#include "SceneManager.h"
#include "Shape.h"

void GameManager::Loop()
{
	isRunning = true;
	 
	Timer time;
	while (isRunning == true)
	{ 
		PROFILER_START("main loop", "main loop");
		if (InputManager::GetInstance().IsDown(Space)) {
			SceneManager::GetInstance().SetCurrentSceneToPreviousScene();
		}

		time.ResetChrono();

		InputManager::GetInstance().Update();

		SceneManager::GetInstance().UpdateCurrentScene(time);


		SDL_RenderClear(mp_window->GetRenderer());

		SceneManager::GetInstance().DrawCurrentScene(mp_window);
    
		SDL_RenderPresent(mp_window->GetRenderer());

		PROFILER_END("main loop");

	}

	isRunning = false;
}

bool GameManager::Init(int windowWidth, int windowHeight)
{

	m_WindW = windowWidth;
	m_WindH = windowHeight;

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) != 0)
	{
		std::cout << "SDL_Init_Error :  " << SDL_GetError() << std::endl;
		return false;
	}

	if (IMG_Init(IMG_INIT_PNG) == 0) {
		std::cout << "Error SDL2_image Initialization";
		return false;
	}

	mp_window = new Window("gcle", m_WindW, m_WindH, SDL_WINDOW_SHOWN, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED);

	if (!mp_window)
	{
		std::cout << "Window Initialization";
		return false;
	}

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

	RessourceManager::GetInstance().Init(mp_window->GetRenderer());

	return true;
}

void GameManager::Close()
{
	mp_window->End();

	delete mp_window;

	RessourceManager::GetInstance().DeleteAll();

	Mix_CloseAudio();
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
	SDL_Quit();
}