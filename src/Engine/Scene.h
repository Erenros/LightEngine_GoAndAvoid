#pragma once
#include <vector>
#include <iostream>
#include "Entity.h"
#include "Core/InputManager.h"

//class Entity;
class Font;
class Text;
class GameManager;
class Window;

class Scene
{
private:

	std::vector<Text*> m_texts;

	void Update(Timer& time);
	void Draw(Window* window);

protected:
	std::string m_tag;

	Scene() = default;
		
	~Scene();

	virtual void OnInitialize() {};
	virtual void OnUpdate(Timer& time) {};
	virtual void OnExit() {};


public:

	Text* CreateText(Font* font, std::string& text, int x, int y, int w, int h, byte r = 255, byte g = 255, byte b = 255);
	void DestroyText(Text* text);


	template<typename T>
	T* CreateEntity(Shape& shape);


	friend class GameManager;
	friend class SceneManager;
};

#include "Scene.inl"