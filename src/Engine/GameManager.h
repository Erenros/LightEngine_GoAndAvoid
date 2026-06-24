#pragma once

#include <vector>
#include "Render/Window.h"
#include "include.h"
#include "Camera.h"

#include "RessourceManager.h"
#include "SceneManager.h"

class Entity;
class Scene;


class GameManager
{
public:

	static GameManager& GetInstance() {
		static GameManager instance;
		return instance;
	}


	GameManager() = default;


	bool Init(int32 windowWidth, int32 windowHeight);
	void Loop();
	void Close();
	Clock* GetTime() { return &m_Time; }

	void AddEntity(Entity* entity) { m_entitiesToCreate.push_back(entity); };
	
	Window* GetWindow() { return mp_window; };
	Camera* GetCamera() { return &m_Cam; }

private:
	void UpdateEntitySystem();

private:

	GameManager* mp_Instance = nullptr;

	Window* mp_window = nullptr;

	Camera m_Cam;

	Clock m_Time;

	bool isRunning = false;

	int32 m_WindW, m_WindH = 0;

	std::vector <Entity*> m_entities;
	std::vector <Entity*> m_entitiesToDestroy;
	std::vector <Entity*> m_entitiesToCreate;
	
	int m_loopTour = 0; 

private:
	friend class Scene;

};