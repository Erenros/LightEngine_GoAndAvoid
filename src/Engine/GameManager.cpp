#include "GameManager.h"
#include <iostream>
#include "RessourceManager.h"
#include "SceneManager.h"
#include "InputManager.h"

#include "Entity.h"

#include "Shape.h"

void GameManager::Loop()
{
	isRunning = true;

	SDL_RenderClear(mp_window->GetRenderer());

	Rectangle* rectangle = new Rectangle(100, 100, 20, 500, { 0, 230, 0, 250 });
	rectangle->Draw(mp_window);

	Triangle* triangle = new Triangle(10, 10, 100, 10, 100, 400, { 230, 0, 0, 250 });
	triangle->Draw(mp_window);

	Circle* circle = new Circle(350, 350, 100, 50, { 0, 0, 230, 255 });
	circle->Draw(mp_window);

	SDL_RenderPresent(mp_window->GetRenderer());


	while (isRunning == true)
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

void GameManager::LaunchGame()
{
	if (!Init()) return;
	
	Loop();
	Close();
}