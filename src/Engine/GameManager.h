#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>

#include <vector>
#include "Window.h"
#include "include.h"

#include "RessourceManager.h"

class Entity;
class Scene;


class GameManager
{
private:

	GameManager* mp_Instance = nullptr;

	Window* mp_window = nullptr;

	bool isRunning = false;

	int m_WindW, m_WindH;

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

	/*template<typename T>
	T* LaunchScene();*/
};

#include "GameManager.inl"