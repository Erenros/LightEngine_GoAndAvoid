#include "GameManager.h"
#include <iostream>
#include "Core/InputManager.h" 
#include "Entity.h"
#include "Scene.h" 
#include "SceneManager.h"
#include "PhysicsManager.h"
#include "Shape.h"
#include "SDLEvent.h"

void GameManager::Loop()
{
	isRunning = true;
	 
	Timer time;
	while (isRunning == true)
	{
		time.ResetChrono();
		
		InputManager::GetInstance().Update();

		SceneManager::GetInstance().UpdateCurrentScene(time);

		if (m_loopTour < 3)
			m_loopTour++;
		else
			PhysicsManager::GetInstance().Update(0.016f);


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