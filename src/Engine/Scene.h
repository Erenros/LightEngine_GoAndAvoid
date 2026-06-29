#pragma once
#include <vector>
#include <iostream>
#include "Entity.h"
#include "Core/InputManager.h"



//class Entity;
class Font;
class Text;
class Camera;
class GameManager;
class Window;



class Scene
{
public:

	Text* CreateText(const std::string& text, int x, int y, int w, int h, byte r = 255, byte g = 255, byte b = 255);
	void DestroyText(Text* text);

	template<typename T>
	T* CreateEntity(gcle::Shapes shape);

	Camera* CreateCamera();


	uint32 GetFlag() { return m_flag; }

	void AddDrawnTexture(const std::string&);
	bool isDrawn(const std::string& tag);

	void SetDebug();

protected: 
	Scene() = default;
		
	virtual ~Scene();

	virtual void OnInitialize();
	virtual void OnUpdate(Clock& time);
	virtual void OnExit();

	Camera* GetMainCamera();

private: 
	void Update(Clock& time);
	void DrawDebug(Window* window);
	void Draw(Window* window);
	Text* CreateDebugText(const std::string& text, int x, int y, int w, int h, byte r = 255, byte g = 255, byte b = 255);
	void DestroyDebugText(Text* text);


protected:
	std::string m_tag;

private:

	uint32 m_flag;
	std::vector<Text*> m_texts;
	std::vector<std::string> m_activeTextures;


private:
	// DEBUG
	bool m_debug = false;
	std::vector<Text*> m_debugTexts;
	Entity* m_selectedEntity = nullptr;

	Camera* mp_mainCamera = nullptr;

	Text* mp_Position = nullptr;
	Text* mp_PosX = nullptr;
	Text* mp_PosY = nullptr; 
	Text* mp_Rotation = nullptr;
	Text* mp_RotZ = nullptr; 
	Text* mp_Scale = nullptr;
	Text* mp_ScaleX = nullptr;
	Text* mp_ScaleY = nullptr; 

private: 
	friend class GameManager;
	friend class SceneManager;
};

#include "Scene.inl"