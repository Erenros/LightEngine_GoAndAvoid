#include "GameManager.h"
#include <iostream>
#include "RessourceManager.h"
#include "InputManager.h"
#include "Entity.h"
#include "Scene.h"

#include "Shape.h"

void GameManager::Loop()
{
	isRunning = true;	

	LaunchScene<Scene>();

	SDL_Texture* tex = RessourceManager::GetInstance().GetTexture("images");

	Rectangle* rectangle = new Rectangle(0, 0, 300, 300, { 250,250, 250, 250 });


	rectangle->SetTexture(tex);


	Triangle* triangle = new Triangle(0.f, 0.f, 500.f, 300.f, 500.f, 500.f, { 255, 255, 255, 255 });
	triangle->SetTexture(tex);


	
	Circle* circle = new Circle(0, 0, 100, 50, { 0, 0, 230, 255 });

	
	 
	Timer time;

	//m_entities.push_back(new Entity());
	Transform2D transform;
	transform.Initialize({ 0.f, 0.f }, 0.f);
	//m_entities[0]->Initialize(*circle, transform);

	mp_currentScene->CreateEntity<Entity>(*circle);

	while (isRunning == true)
	{ 
		time.ResetChrono();

		SDL_RenderClear(mp_window->GetRenderer());

		for (Entity* entity : m_entities) {
			entity->Update(time);
			mp_window->Draw(entity->GetShape());
		} 

		SDL_RenderPresent(mp_window->GetRenderer());
	}

	isRunning = false;
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

void GameManager::LaunchGame()
{
	if (!Init()) return;
	
	Loop();
	Close();
}