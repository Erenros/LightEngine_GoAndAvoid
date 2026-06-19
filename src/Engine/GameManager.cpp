#include "GameManager.h"
#include <iostream>
#include "Core/InputManager.h" 
#include "Camera.h"
#include "Entity.h"
#include "Scene.h" 
#include "SceneManager.h"
#include "PhysicsManager.h"
#include "Shape.h"
#include "SDLEvent.h"




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
		time.ResetChrono();
		
		InputManager::GetInstance().Update();

		SceneManager::GetInstance().UpdateCurrentScene(time);

		if (m_loopTour < 3)
			m_loopTour++;
		else
			PhysicsManager::GetInstance().Update(0.016f);
		
	
		cam.Update(time, m_entities);

		mp_window->Clear();

		SceneManager::GetInstance().DrawCurrentScene(mp_window);
    
		mp_window->Present();

		if (Event::WindowEvent())
		{
			isRunning = false;
		}

	}

	isRunning = false;
}

bool GameManager::Init(int windowWidth, int windowHeight)
{
	srand(time(NULL));

	m_WindW = windowWidth;
	m_WindH = windowHeight;

	uint32 windowFlags = SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN;
	uint32 renderFlags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;


	mp_window = new Window("gcle", m_WindW, m_WindH, windowFlags, renderFlags, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED);

	if (!mp_window)
	{
		std::cout << "Window Initialization";
		return false;
	}

	RessourceManager::GetInstance().Init(mp_window);

	return true;
}

void GameManager::Close()
{
	RessourceManager::GetInstance().DeleteAll();
	mp_window->End();

	delete mp_window;

}
