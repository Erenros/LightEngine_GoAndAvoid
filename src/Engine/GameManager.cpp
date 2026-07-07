#include <iostream>
#include "Scene.h" 
#include "Shape.h"
#include "Event.h"
#include "Entity.h"
#include "GameManager.h"
#include "SceneManager.h"
#include "PhysicsManager.h"
#include "Core/InputManager.h" 

#include <timeapi.h>
#pragma comment(lib, "winmm.lib")
 
GameManager& GameManager::GetInstance() {
	static GameManager instance;
	return instance;
}


void GameManager::Loop()
{
	m_IsRunning = true;   
	 
	timeBeginPeriod(1);
	while (m_IsRunning == true)
	{
 

		PROFILER_START("Colliders", "Colliders Update");
		int32 exec = 0;
		accDt += static_cast<float32>(m_Time.GetDeltaTime());
		
		SceneManager::GetInstance().UpdateCurrentScene(m_Time);
		InputManager::GetInstance().Update();
		 
		while (accDt >= fixedUpdateDT) 
		{
			accDt -= static_cast<float32>(fixedUpdateDT);
			if (m_LoopTour < 1)
				m_LoopTour++;
			else
			{
				UpdateRigidBodies(static_cast<float32>(fixedUpdateDT));
				PhysicsManager::GetInstance().Update(static_cast<float32>(fixedUpdateDT));
			}
			exec += 1;
		}
		PROFILER_END("Colliders");
	
		PROFILER_START("Entity", "Entity Creation / Deletion");
		UpdateEntitySystem();
		PROFILER_END("Entity");

		PROFILER_START("Input", "Input Update");
		InputManager::GetInstance().Update();
		PROFILER_END("Input");

		PROFILER_START("SceneU", "Scene Update");
		SceneManager::GetInstance().UpdateCurrentScene(m_Time);
		PROFILER_END("SceneU");
		 
		PROFILER_START("SceneD", "Scene Draw");
		for (auto& cam : m_Camera)
		{
			if (cam->IsActive())
			cam->Update(m_Time, m_Entities);
		} 
	
		mp_Window->ClearWindowWithColor(m_ClearColor.r, m_ClearColor.g, m_ClearColor.b, m_ClearColor.a);
		mp_Window->Clear();
		 
		SceneManager::GetInstance().DrawCurrentScene(mp_Window);
		 
		SceneManager::GetInstance().DrawCurrentSceneDebug(mp_Window);  
	
		mp_Window->Present();
		PROFILER_END("SceneD");

		if (Event::WindowEvent())
		{
			m_IsRunning = false;
		}
		 
		
		float64 rawDT = m_Time.GetRawDT();
		if (rawDT < fpsDT) {
			float64 timeToSleep = fpsDT - rawDT;
			m_Time.SmartSleep(timeToSleep);
		}


		//PROFILER_START("time", "Timer Update");
		m_Time.Update();
		//PROFILER_END("time"); 
	
		// GCLE_INFO << "FPS : " << m_Time.GetFramePerSecond() << ENDL;
		// PROFILER_END("Update");
		// system("CLS");
	}
	timeEndPeriod(1);
	m_IsRunning = false;
}

GameManager::~GameManager()
{
	UpdateEntitySystem();
	DestroyAllEntitiesAndCameras();
}

bool GameManager::Init(int32 windowWidth, int32 windowHeight, int16 FPS)
{
#ifndef NDEBUG
	GCLE::DebuggerDesc desc{};
	desc.output = GCLE::DebuggerOutput::CONSOLE | GCLE::DebuggerOutput::LOGS;
	GCLE::Debugger::Init(&desc);
#endif

	srand(static_cast<int32>(m_Time.GetTime()));
	
	m_fps = FPS;
	fpsDT = 1.f / m_fps;

	m_WindW = windowWidth;
	m_WindH = windowHeight;

	uint32 windowFlags = SDL_WINDOW_FLAGS::WINDOW_RESIZABLE | SDL_WINDOW_FLAGS::WINDOW_SHOWN;
	uint32 renderFlags = SDL_RENDERER_FLAGS::RENDERER_ACCELERATED;


	mp_Window = GCLE_NEW Window("gcle", m_WindW, m_WindH, windowFlags, renderFlags, SDL_WINDOW_POSITION::WINDOWPOS_UNDEFINED, SDL_WINDOW_POSITION::WINDOWPOS_UNDEFINED);

	if (!mp_Window)
	{
		std::cout << "Window Initialization";
		return false;
	}
	RessourceManager& rm = RessourceManager::GetInstance();
	RessourceManager::GetInstance().Init(mp_Window);

	for (int i = 0; i < 32; i++)
		m_Entities.push_back({}); 

	return true;
}

void GameManager::Close()
{
	RessourceManager::GetInstance().DeleteAll(); 
	DestroyAllEntitiesAndCameras();

	PROFILER_CLEAR();

	delete mp_Window;

}

Clock* GameManager::GetTime()
{
	return &m_Time;
}

void GameManager::SetWindowClearColor(Color color)
{
	m_ClearColor = color;
}


void GameManager::UpdateRigidBodies(float32 dt)
{
	for (auto& entity : GetActiveEntities(SceneManager::GetInstance().m_CurrentSceneTag))
	{
		if (entity->IsRigidBody())
			entity->GetRigidBody().Update(dt);
	}
}

std::vector<Entity*> GameManager::GetActiveEntities(const std::string& scene)
{
	std::vector<Entity*> results;
	for (auto& layer : m_Entities) {
		for (auto e : layer) {
			if (e->IsActiveIn(scene))
				results.push_back(e);
		}
	}
	return results;
}

void GameManager::UpdateEntitySystem()
{
	for (int i = 0; i < m_Entities.size(); i++)
	{
		for (auto it = m_Entities[i].begin(); it != m_Entities[i].end(); )
		{
			Entity* entity = *it;

			if (entity->ToDestroy())
			{
				m_EntitiesToDestroy.push_back(entity);
				it = m_Entities[i].erase(it);
			}
			else
			{
				++it;
			}

		}
	}

	for (auto it = m_EntitiesToDestroy.begin(); it != m_EntitiesToDestroy.end(); ++it)
	{
		delete* it;
	}

	m_EntitiesToDestroy.clear();

	for (auto it = m_EntitiesToCreate.begin(); it != m_EntitiesToCreate.end(); ++it)
	{
		Entity* e= *it;
		m_Entities[e->GetLayer()].push_back(*it);
	}

	m_EntitiesToCreate.clear();
}
void GameManager::DestroyAllEntitiesAndCameras()
{
	for (auto& layer : m_Entities)
	{
		for (auto& entity : layer)
			delete entity;
		layer.clear();
	}
	m_Entities.clear();

	for (auto& cam : m_Camera)
		delete cam;
	m_Camera.clear();
}

void GameManager::AddEntity(Entity* pEntity) { 
	m_EntitiesToCreate.push_back(pEntity); 
}

Window* GameManager::GetWindow() { 
	return mp_Window; 
}

