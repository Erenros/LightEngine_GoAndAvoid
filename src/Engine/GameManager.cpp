#include "GameManager.h"
#include <iostream>
#include "Core/InputManager.h" 
#include "Camera.h"
#include "Entity.h"
#include "Scene.h" 

#include "Shape.h"




GameManager::GameManager(int32 _width, int32 _height) : m_WindW(_width), m_WindH(_height)
{
	mp_Instance = this;
}

void GameManager::Loop()
{
	isRunning = true;
	 
	Timer time;

	Camera cam;
	cam.Init(mp_window->GetRenderer());
	cam.SetFollowing(m_entities[3]);

	SDL_Renderer* renderer = mp_window->GetRenderer(); 

	while (isRunning == true)
	{ 
		SDL_Delay(10);

		InputManager::GetInstance().Update();

		mp_currentScene->OnUpdate();

		time.ResetChrono();

		SDL_RenderClear(renderer);
		
		cam.Update(time, m_entities);

		for (int32 i = 0; i < m_entities.size(); i++)
		{
			mp_window->Draw(m_entities[i]->GetRenderShape());
		}

		SDL_RenderPresent(renderer);
	}

	isRunning = false;
}

bool GameManager::Init()
{
	int a;

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
