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

	Window* mp_window = nullptr; 

	bool isRunning = false;

	int m_WindW, m_WindH;

	std::vector <Entity*> m_entities;

	Scene* mp_currentScene = nullptr;

	

public:

	GameManager(int windowWidth,int windowHeight) :
		m_WindW(windowWidth),
		m_WindH(windowHeight) { }

	bool Init();
	void Loop();
	void Close();

	void AddEntity(Entity* entity) { m_entities.push_back(entity); };
	
	Window* GetWindow() { return mp_window; }; 

	template<typename T>
	T* LaunchScene();
};

#include "GameManager.inl"