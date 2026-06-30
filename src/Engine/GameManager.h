#pragma once

#include <vector>
#include "Render/Window.h"
#include "include.h"
#include "Camera.h"

#include "RessourceManager.h"
#include "SceneManager.h"

#define FixedUpdateExecution 50

class Entity;
class Scene;

class GameManager
{
public:
	int16 m_fps = 120;

	static GameManager& GetInstance() {
		static GameManager instance;
		return instance;
	}

	float64 fixedUpdateDT = 1.f / FixedUpdateExecution; 
	float32 m_accDt = 0.f;
	float64 m_fpsDT = 1.f / m_fps; 

	GameManager() = default;
	~GameManager();


	bool Init(int32 windowWidth, int32 windowHeight, int16 FPS = 60);
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
	Window* mp_window = nullptr;

	Color m_ClearColor = { 0, 0, 0, 255 };

	Clock m_Time;

	bool isRunning = false;

	int32 m_WindW = 0, m_WindH = 0;

	std::vector <std::vector<Entity*>> m_entities;
	std::vector <Entity*> m_entitiesToDestroy;
	std::vector <Entity*> m_entitiesToCreate;

	std::vector <Camera*> m_camera; 
	
	int m_loopTour = 0; 


private:
	friend class Scene;
	friend class Window;

};