#pragma once

#include <vector>
#include "Render/Window.h"
#include "include.h"
#include "Camera.h"

#include "RessourceManager.h"
#include "SceneManager.h"

class Entity;
class Scene;

#define FixedUpdateExecution 60;
#define FPS 60;

class GameManager
{
public:

	static GameManager& GetInstance() {
		static GameManager instance;
		return instance;
	}

	float64 fixedUpdateDT = 1.f / FixedUpdateExecution;
	float64 accDt = 0.f;

	int16 fpsCount = 0;
	float64 fpsTimer = 0.f;

	GameManager() = default;
	~GameManager();


	bool Init(int32 windowWidth, int32 windowHeight);
	void Loop();
	void Close();

	Clock* GetTime() { return &m_Time; }

	void SetWindowClearColor(Color color) { m_ClearColor = color; }

	void AddEntity(Entity* entity) { m_entitiesToCreate.push_back(entity); };
	
	Window* GetWindow() { return mp_window; };

	std::vector<Entity*> GetActiveEntities(const std::string& scene);
private:
	void UpdateEntitySystem();

private:

	GameManager* mp_Instance = nullptr;
	Camera m_Cam;
	Window* mp_window = nullptr;

	Color m_ClearColor = { 0, 0, 0, 255 };

	Clock m_Time;

	bool isRunning = false;

	bool m_isVisualDebugActive = false;

	int32 m_WindW = 0, m_WindH = 0;

	std::vector <std::vector<Entity*>> m_entities;
	std::vector <Entity*> m_entitiesToDestroy;
	std::vector <Entity*> m_entitiesToCreate;
	
	int m_loopTour = 0; 


private:
	friend class Scene;
	friend class Window;

};