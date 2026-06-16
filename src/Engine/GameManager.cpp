#include "GameManager.h"
#include <iostream>
#include "RessourceManager.h"
#include "SceneManager.h"
#include "InputManager.h"

#include "Entity.h"

void GameManager::Loop()
{
	isRunning = true;

	SDL_Rect rect(50, 50, 50, 50);

	InputManager& IM = InputManager::GetInstance();

	RessourceManager& RM = RessourceManager::GetInstance();
	RM.Init(mp_window->GetRenderer());

	SDL_SetRenderDrawColor(mp_window->GetRenderer(), 255, 255, 255, 255);
	SDL_RenderClear(mp_window->GetRenderer());

	SDL_SetRenderDrawColor(mp_window->GetRenderer(), 0, 0, 0, 255);

	SDL_RenderDrawRect(mp_window->GetRenderer(), &rect);


	SDL_Rect srect = SDL_Rect(0, 0, 32, 32);
	SDL_Rect rect2 = SDL_Rect(0, 0, 256, 256);

	//SDL_RenderCopy(mp_window->GetRenderer(), RM.GetTexture("test"), &srect, &rect2);

	Sprite* spritesheet = new Sprite("test", srect,true, 1, 4);
	spritesheet->PlayAnimation(0);


	//SDL_Delay(5000);

	while (IM.HandleInput() && isRunning == true)
	{
		//TODO Loop
		spritesheet->UpdateAnimation();
		SDL_Delay(spritesheet->Duration * 500);
		SDL_RenderCopy(mp_window->GetRenderer(), spritesheet->pTexture, &spritesheet->SourceRect, &rect2);
		SDL_RenderPresent(mp_window->GetRenderer());

		for (Entity* entity : m_entities) {
			//TODO faire update et draw pour les entiti�s
			/*entity.Update();
			entity.Draw();*/
		}
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