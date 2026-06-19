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
private: 

	GameManager* mp_Instance = nullptr;

	Window* mp_window = nullptr;

	bool isRunning = false;

	int32 m_WindW, m_WindH = 0;

	std::vector <Entity*> m_entities;

	friend class Scene;

public:

	static GameManager& GetInstance() {
		static GameManager instance;
		return instance;
	}


	GameManager() = default;


	bool Init(int windowWidth, int windowHeight);
	void Loop();
	void Close();

	void AddEntity(Entity* entity) { m_entities.push_back(entity); };

	Window* GetWindow() { return mp_window; };

private:
	int m_loopTour = 0;

};