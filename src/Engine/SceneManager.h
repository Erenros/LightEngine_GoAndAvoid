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

	std::vector<int8> m_validFlag;

	//Update
	void UpdateCurrentScene(Clock& time);

	//Draw
	void DrawCurrentScene(Window* pWindow);
	void DrawCurrentSceneDebug(Window* pWindow);


	friend class GameManager;

	void LoadUnloadActiveTextures(const std::string& newScene);

public:

	//Constructors

	SceneManager() {
		for(int8 i = 0; i < 32; i++)
			m_validFlag.push_back(i);
	}

	//Destructors

	~SceneManager();

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
	uint32 GetCurrentSceneFlag();

	//Setter

	void SetCurrentSceneWithTag(const std::string& tag);
	void SetCurrentSceneToPreviousScene();

	//Scene
	void DeleteAllScenes();

	template <typename S>
	Scene* CreateScene(const std::string& tag);
	void DeleteScene(const std::string& tag);

	bool isLoaded(uint32 flag);
};

#include "SceneManager.inl"
