#include "GameManager.h"
#include <iostream>
#include "RessourceManager.h"
#include "SceneManager.h"
#include "InputManager.h"
#include "Camera.h"
#include "Entity.h"

void GameManager::Loop()
{
	isRunning = true;

	InputManager& IM = InputManager::GetInstance();

	RessourceManager& RM = RessourceManager::GetInstance();
	RM.Init(mp_window->GetRenderer());

	SDL_Renderer* p_renderer = mp_window->GetRenderer();

	SDL_SetRenderDrawColor(p_renderer, 255, 255, 255, 255);
	SDL_RenderClear(p_renderer);


	SDL_Rect rect(50, 50, 50, 50);

	SDL_SetRenderDrawColor(p_renderer, 0, 0, 0, 255);
	SDL_RenderDrawRect(p_renderer, &rect);
	SDL_RenderPresent(p_renderer);

	Camera cam;
	cam.InitRenderer(p_renderer);

	while (true)
	{
		SDL_Delay(100);
		cam.Update();
		SDL_RenderPresent(p_renderer);
	}


	while (IM.HandleInput() && isRunning == true)
	{
		timer->ResetChrono();

		//TODO Loop

		for (Entity* entity : m_entities) {
			//TODO faire update et draw pour les entiti�s
			/*entity.Update();
			entity.Draw();*/
		}
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

	timer = new Timer();

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

void GameManager::LaunchGame(int windowWidth, int windowHeight)
{
	m_WindW = windowWidth;
	m_WindH = windowHeight;

	if (!Init()) return;
	
	Loop();
	Close();
}

GameManager* GameManager::Get()
{
	static GameManager* mp_instance = new GameManager; 
	return  mp_instance;
}

