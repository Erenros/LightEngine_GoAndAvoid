#pragma once

#include <unordered_map>
#include <iostream>
#include <string>
#include "include.h"
#include <vector>

class Scene;
class Window;
class Clock;


class SceneManager {
	std::unordered_map<std::string, Scene*> m_Scenes;
	std::string m_CurrentSceneTag = "";
	std::string m_PreviousSceneTag = "";

	//Update
	void UpdateCurrentScene(Clock& time);

	//Draw
	void DrawCurrentScene(Window* pWindow);
	void DrawCurrentSceneDebug(Window* pWindow);


	friend class GameManager;

	void LoadUnloadActiveTextures(const std::string& newScene);

public:

	//Constructors

	SceneManager();

	//Destructors

	~SceneManager();

	//Getter

	static SceneManager& GetInstance();
	Scene* GetCurrentScene();
	Scene* GetPreviousScene();
	Scene* GetSceneWithTag(const std::string& tag);
	std::string& GetCurrentSceneTag();
	std::string& GetPreviousSceneTag();

	//Setter

	void SetCurrentSceneWithTag(const std::string& tag);
	void SetCurrentSceneToPreviousScene();

	//Scene
	void DeleteAllScenes();

	template <typename S>
	Scene* CreateScene(const std::string& tag);
	void DeleteScene(const std::string& tag);
};

#include "SceneManager.inl"
