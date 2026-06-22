#pragma once

#include <vector>
#include "Render/Window.h"
#include "include.h"

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

	void AddEntity(Entity* entity) { m_entitiesToCreate.push_back(entity); };
	
	Window* GetWindow() { return mp_window; };

private:
	void UpdateEntitySystem();

private:

	GameManager* mp_Instance = nullptr;

	Window* mp_window = nullptr;

	bool isRunning = false;

	int32 m_WindW, m_WindH = 0;

	std::vector <Entity*> m_entities;
	std::vector <Entity*> m_entitiesToDestroy;
	std::vector <Entity*> m_entitiesToCreate;
	
	int m_loopTour = 0; 

private:
	friend class Scene;

};