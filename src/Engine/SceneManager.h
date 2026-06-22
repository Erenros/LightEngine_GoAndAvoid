#pragma once

#include <unordered_map>
#include <iostream>
#include <string>
#include "include.h"


class Scene;
class Window;
class Clock;


class SceneManager {
	std::unordered_map<std::string, Scene*> m_Scenes;
	std::string m_CurrentSceneTag = "";
	std::string m_PreviousSceneTag = "";

	int8 m_sceneCount = 0;

	//Update
	void UpdateCurrentScene(Clock& time);

	//Draw
	void DrawCurrentScene(Window* window);


	friend class GameManager;
public:

	//Constructors

	SceneManager() = default;

	//Destructors

	~SceneManager() = default;

	//Getter

	static SceneManager& GetInstance() {
		static SceneManager instance;
		return instance;
	}

	Scene* GetCurrentScene();
	Scene* GetPreviousScene();
	Scene* GetSceneWithTag(const std::string& tag);
	std::string& GetCurrentSceneTag();
	std::string& GetPreviousSceneTag();

	//Setter

	void SetCurrentSceneWithTag(const std::string& tag);
	void SetCurrentSceneToPreviousScene();


	//Scene

	template <typename S>
	Scene* CreateScene(const std::string& tag);
	void DeleteScene(const std::string& tag);
};

#include "SceneManager.inl"
