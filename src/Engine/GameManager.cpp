#include <iostream>
#include "Scene.h" 
#include "Shape.h"
#include "Event.h"
#include "Entity.h"
#include "Camera.h"
#include "GameManager.h"
#include "SceneManager.h"
#include "PhysicsManager.h"
#include "Core/InputManager.h" 

void GameManager::Loop()
{
	isRunning = true;
	 
	Clock time;

	m_Cam.Init(mp_window);

	while (isRunning == true)
	{
		PROFILER_START("Colliders", "Colliders Update");
		if (m_loopTour < 1)
			m_loopTour++;
		else
			PhysicsManager::GetInstance().Update(0.016f);
		PROFILER_END("Colliders");

		PROFILER_START("time", "Timer Update");
		time.Update();
		PROFILER_END("time");
		
		PROFILER_START("Input", "Input Update");
		InputManager::GetInstance().Update();
		PROFILER_END("Input");

		PROFILER_START("SceneU", "Scene Update");
		SceneManager::GetInstance().UpdateCurrentScene(time);
		PROFILER_END("SceneU");
		
		PROFILER_START("Camera", "Camera Update");
		m_Cam.Update(time, m_entities);
		PROFILER_END("Camera");
	
		PROFILER_START("Entity", "Entity Creation / Deletion");
		UpdateEntitySystem();
		PROFILER_END("Entity");

		mp_window->Clear();

		PROFILER_START("SceneD", "Scene Draw");
		SceneManager::GetInstance().DrawCurrentScene(mp_window);
		PROFILER_END("SceneD");
    
		mp_window->Present();

		if (Event::WindowEvent())
		{
			isRunning = false;
		}

		system("CLS");

	}

	isRunning = false;
}

bool GameManager::Init(int32 windowWidth, int32 windowHeight)
{
	srand(static_cast<int32>(time(NULL)));

	m_WindW = windowWidth;
	m_WindH = windowHeight;

	uint32 windowFlags = SDL_WINDOW_FLAGS::WINDOW_RESIZABLE | SDL_WINDOW_FLAGS::WINDOW_SHOWN;
	uint32 renderFlags = SDL_RENDERER_FLAGS::RENDERER_ACCELERATED | SDL_RENDERER_FLAGS::RENDERER_PRESENTVSYNC;


	mp_window = new Window("gcle", m_WindW, m_WindH, windowFlags, renderFlags, SDL_WINDOW_POSITION::WINDOWPOS_UNDEFINED, SDL_WINDOW_POSITION::WINDOWPOS_UNDEFINED);

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

void GameManager::UpdateEntitySystem()
{
	for (auto it = m_entities.begin(); it != m_entities.end(); )
	{
		Entity* entity = *it;

		if (entity->ToDestroy())
		{
			m_entitiesToDestroy.push_back(entity);
			it = m_entities.erase(it);
		}

		++it;		
	}

	for (auto it = m_entitiesToDestroy.begin(); it != m_entitiesToDestroy.end(); ++it)
	{
		delete* it;
	}

	m_entitiesToDestroy.clear();

	for (auto it = m_entitiesToCreate.begin(); it != m_entitiesToCreate.end(); ++it)
	{
		m_entities.push_back(*it);
	}

	m_entitiesToCreate.clear();
}
