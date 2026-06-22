#pragma once
#include <vector>
#include <iostream>
#include "Entity.h"
#include "Core/InputManager.h" 

//class Entity;
class GameManager;
class Window;


class Scene
{
private:


	void Update(Timer& time);
	void Draw(Window* window);

protected:
	std::string m_tag;

	Scene() = default;
		
	~Scene() = default;

	virtual void OnInitialize() {};
	virtual void OnUpdate(Timer& time) {};
	virtual void OnExit() {};


public:

	template<typename T>
	T* CreateEntity(gcle::Shapes shape);


	friend class GameManager;
	friend class SceneManager;
};

#include "Scene.inl"