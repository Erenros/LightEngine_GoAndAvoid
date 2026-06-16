//#pragma once
//
//#include <unordered_map>
//#include <iostream>
//#include <string>
//
//
//class Scene;
//class Window;
//
//class SceneManager{
//
//	SceneManager* mp_Instance = nullptr;
//
//	std::unordered_map<const std::string&, Scene*> m_Scenes;
//	std::string m_CurrentSceneTag = "";
//	std::string m_PreviousSceneTag = "";
//
//
//
//	//Update
//	void UpdateCurrentScene();
//
//	//Draw
//	void DrawCurrentScene(Window* window);
//
//
//	friend class GameManager;
//public:
//
//	//Constructors
//
//	SceneManager() = default;
//
//	//Destructors
//
//	~SceneManager() = default;
//
//	//Getter
//
//	SceneManager* Get();
//
//	Scene* GetCurrentScene();
//	Scene* GetPreviousScene();
//	Scene* GetSceneWithTag(const std::string& tag);
//	std::string& GetCurrentSceneTag();
//	std::string& GetPreviousSceneTag();
//
//	//Setter
//
//	void SetCurrentSceneWithTag(const std::string& tag);
//	void SetCurrentSceneToPreviousScene();
//
//
//	//Scene
//
//	Scene* CreateScene(const std::string& tag);
//	void DeleteScene(const std::string& tag);
//};
//
