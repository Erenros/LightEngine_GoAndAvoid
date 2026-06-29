#include <iostream>
#include "Scene.h" 
#include "Shape.h"
#include "Event.h"
#include "Entity.h"
#include "GameManager.h"
#include "SceneManager.h"
#include "PhysicsManager.h"
#include "Core/InputManager.h" 

void GameManager::Loop()
{
#ifndef NDEBUG
	GCLE::DebuggerDesc desc{};
	desc.output = GCLE::DebuggerOutput::CONSOLE | GCLE::DebuggerOutput::LOGS;
	GCLE::Debugger::Init(&desc); 
#endif

	isRunning = true;
	 
	m_Cam.Init(mp_window);

	while (isRunning == true)
	{
		//PROFILER_START("Update", "Update");

		//PROFILER_START("Colliders", "Colliders Update");
		int32 exec = 0;
		accDt += m_Time.GetDeltaTime();
		while (accDt >= fixedUpdateDT) {
			accDt -= fixedUpdateDT;
			if (m_loopTour < 1)
				m_loopTour++;
			else
				PhysicsManager::GetInstance().Update(m_Time.GetDeltaTime());
			exec += 1;
		}
		//PROFILER_END("Colliders");
		//GCLE_INFO << "Fixed update execution : " << exec << ENDL;


		//PROFILER_START("time", "Timer Update");
		m_Time.Update();
		//PROFILER_END("time");
	
		//PROFILER_START("Entity", "Entity Creation / Deletion");
		UpdateEntitySystem();
		//PROFILER_END("Entity");

		if (InputManager::GetInstance().IsDown(F1))
		{
			m_isVisualDebugActive = !m_isVisualDebugActive;
		}

		//PROFILER_START("Input", "Input Update");
		InputManager::GetInstance().Update();
		//PROFILER_END("Input");

		//PROFILER_START("SceneU", "Scene Update");
		SceneManager::GetInstance().UpdateCurrentScene(m_Time);
		//PROFILER_END("SceneU");
		
		//PROFILER_START("Camera", "Camera Update");
		m_Cam.Update(m_Time, m_entities);
		//PROFILER_END("Camera");
	
		mp_window->ClearWindowWithColor(m_ClearColor.r, m_ClearColor.g, m_ClearColor.b, m_ClearColor.a);
		mp_window->Clear();

		//PROFILER_START("SceneD", "Scene Draw");
		SceneManager::GetInstance().DrawCurrentScene(mp_window);
		 
		if (m_isVisualDebugActive)
		{
			SceneManager::GetInstance().DrawCurrentSceneDebug(mp_window);
		}
		//PROFILER_END("SceneD");
	
		mp_window->Present();

		if (Event::WindowEvent())
		{
			isRunning = false;
		}

		//PROFILER_START("FPSCount", "FPSCount");

		fpsTimer += m_Time.GetDeltaTime();
		if (fpsTimer >= 1.f) {
			fpsTimer -= 1.f;
			fpsCount = static_cast<int16>(1.f / m_Time.GetDeltaTime());
		}
		GCLE_INFO << "FPS : " << fpsCount << ENDL;

		//PROFILER_END("FPSCount");


		system("CLS");
		//PROFILER_END("Update");

	}

	isRunning = false;
}

GameManager::~GameManager()
{
	UpdateEntitySystem();

	for (auto& layer : m_entities)
	{
		if (layer.empty())
			continue;

		for (auto& entity : layer)
		{
			delete entity;
		}

		layer.clear();
	}

	m_entities.clear();
}

bool GameManager::Init(int32 windowWidth, int32 windowHeight)
{
	srand(static_cast<int32>(m_Time.GetTime()));

	m_WindW = windowWidth;
	m_WindH = windowHeight;

	uint32 windowFlags = SDL_WINDOW_FLAGS::WINDOW_RESIZABLE | SDL_WINDOW_FLAGS::WINDOW_SHOWN;
	uint32 renderFlags = SDL_RENDERER_FLAGS::RENDERER_ACCELERATED | SDL_RENDERER_FLAGS::RENDERER_PRESENTVSYNC;


	mp_window = GCLE_NEW Window("gcle", m_WindW, m_WindH, windowFlags, renderFlags, SDL_WINDOW_POSITION::WINDOWPOS_UNDEFINED, SDL_WINDOW_POSITION::WINDOWPOS_UNDEFINED);

	if (!mp_window)
	{
		std::cout << "Window Initialization";
		return false;
	}

	RessourceManager::GetInstance().Init(mp_window);

	for (int i = 0; i < 32; i++)
		m_entities.push_back({});

	return true;
}

void GameManager::Close()
{
	RessourceManager::GetInstance().DeleteAll();
	mp_window->End();

	delete mp_window;

}

std::vector<Entity*> GameManager::GetActiveEntities(const std::string& scene)
{
	std::vector<Entity*> results;
	for (auto entities : m_entities) {
		for (auto e : entities) {
			if (e->IsActiveIn(scene))
				results.push_back(e);
		}
	}
	return results;
}

void GameManager::UpdateEntitySystem()
{
	for (int i = 0; i < m_entities.size(); i++)
	{
		for (auto it = m_entities[i].begin(); it != m_entities[i].end(); )
		{
			Entity* entity = *it;

			if (entity->ToDestroy())
			{
				m_entitiesToDestroy.push_back(entity);
				it = m_entities[i].erase(it);
			}

			++it;
		}
	}

	for (auto it = m_entitiesToDestroy.begin(); it != m_entitiesToDestroy.end(); ++it)
	{
		delete* it;
	}

	m_entitiesToDestroy.clear();

	for (auto it = m_entitiesToCreate.begin(); it != m_entitiesToCreate.end(); ++it)
	{
		Entity* e= *it;
		m_entities[e->GetLayer()].push_back(*it);
	}

	m_entitiesToCreate.clear();
}
