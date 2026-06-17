#pragma once
#include <vector>
#include <iostream>
#include "Entity.h"
#include "Core/InputManager.h" 

//class Entity;
class GameManager;

class Scene
{
private:

	GameManager* mp_GameManager = nullptr;

	void SetGameManager(GameManager* gameManager) { mp_GameManager = gameManager; };

protected:

	Scene() = default;

	virtual void OnInitialize() {};
	virtual void OnUpdate() {};

public:

	template<typename T>
	T* CreateEntity(Shape& shape);


	friend class GameManager;
};

#include "Scene.inl"