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
	uint32 m_flag;

	std::vector<Text*> m_texts;

	void Update(Clock& time);
	void Draw(Window* window);

	std::vector<std::string> m_activeTextures;

protected:
	std::string m_tag;

	Scene() = default;
		
	virtual ~Scene();

	virtual void OnInitialize() {};
	virtual void OnUpdate(Clock& time) {};
	virtual void OnExit() {};

public:

	Text* CreateText(const std::string& text, int x, int y, int w, int h, byte r = 255, byte g = 255, byte b = 255);
	void DestroyText(Text* text);


	template<typename T>
	T* CreateEntity(gcle::Shapes shape);

	uint32 GetFlag() { return m_flag; }

	void AddDrawnTexture(const std::string&);
	bool isDrawn(const std::string&);


	friend class GameManager;
	friend class SceneManager;
};

#include "Scene.inl"