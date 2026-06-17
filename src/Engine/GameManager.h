#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>

#include <vector>
#include "Window.h"
#include "include.h"


class Entity;


class GameManager
{
private:

	Window* mp_window = nullptr; 

	bool isRunning = false;

	int m_WindW, m_WindH;

	std::vector <Entity*> m_entities;

	bool Init();
	void Loop();
	void Close();

public:

	GameManager(int windowWidth,int windowHeight) :
		m_WindW(windowWidth),
		m_WindH(windowHeight) { }

	void LaunchGame();

	Window* GetWindow() { return mp_window; };
};