#pragma once

#include <unordered_map>
#include <functional>
#include <iostream>
#include <string>
#include "include.h"
#include <vector>

class Scene;
class Window;
class Clock;


class SceneManager {
	std::unordered_map<std::string, Scene*> m_Scenes;
	std::unordered_map<std::string, std::function<Scene* ()>> m_SceneFactories; 
	std::string m_CurrentSceneTag = "";
	std::string m_PreviousSceneTag = "";

	void UpdateCurrentScene(Clock& time);
	void DrawCurrentScene(Window* pWindow);
	void DrawCurrentSceneDebug(Window* pWindow);

	friend class GameManager;

	void LoadUnloadActiveTextures(const std::string& newScene);

public:

	SceneManager();
	~SceneManager();

	static SceneManager& GetInstance();
	Scene* GetCurrentScene();
	Scene* GetPreviousScene();
	Scene* GetSceneWithTag(const std::string& tag);
	std::string& GetCurrentSceneTag();
	std::string& GetPreviousSceneTag();

	void SetCurrentSceneWithTag(const std::string& tag, bool pause = false); 
	void SetCurrentSceneToPreviousScene(bool pause = false);                 

	void DeleteAllScenes();

	template <typename S>
	Scene* CreateScene(const std::string& tag);
	void DeleteScene(const std::string& tag);

private:
	void SetCurrentCamera();
	 
	Scene* InstantiateSceneFromFactory(const std::string& tag);
	void DestroyScene(const std::string& tag);
	void DestroySceneEntities(const std::string& tag);
	void DestroySceneCameras(const std::string& tag);
};

#include "SceneManager.inl"