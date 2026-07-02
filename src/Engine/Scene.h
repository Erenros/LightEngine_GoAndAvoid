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
class WorldText;



class Scene
{
private :
	struct DebugInformation
	{
		float32 Colliders	= 0;
		float32 Entity		= 0;
		float32 Input		= 0;
		float32 SceneUpdate	= 0;
		float32 Draw		= 0;
	};

public:

	Text* CreateText(const std::string& text, Vector2f pos, int32 fontSize, byte r = 255, byte g = 255, byte b = 255);
	void DestroyText(Text* text);

	template<typename T>
	T* CreateEntity(gcle::Shapes shape);

	template<typename T>
	T* CreateEntity();

	Entity* CreateWorldText(const std::string& text, int32 fontSize, const std::string& fontId = "Hack-Regular", byte r = 255, byte g = 255, byte b = 255, byte a = 255);


	Camera* CreateCamera();


	uint32 GetFlag() const { return m_flag; }

	void AddDrawnTexture(const std::string&);
	bool isDrawn(const std::string& tag);

	void SetDebug();

	Camera* GetCurrentCamera();
protected: 
	Scene() = default;
		
	virtual ~Scene();

	virtual void OnInitialize();
	virtual void OnUpdate(Clock& time);
	virtual void OnExit();

	Camera* GetMainCamera();
	void SwitchCamera(Camera* pCamera);

private: 
	void Update(Clock& time);
	void Draw(Window* window);


private:
	Text*	CreateDebugText(const std::string& text, Vector2f pos, int32 fontSize, byte r = 255, byte g = 255, byte b = 255);
	void	DestroyDebugText(Text* text);
	Text*	CreateDebugInfoText(const std::string& text, Vector2f pos, int32 fontSize, byte r = 255, byte g = 255, byte b = 255);
	void	DestroyDebugInfoText(Text* text);
	void	DrawDebug(Window* window);
	void	SetGizmoVisibility();

	void	EntityInfoVisibility(const int32 debugConstant);
	void	DebugSetEntityInfo();

	void	SetDebugInfo(DebugInformation& info) const;


protected:
	std::string m_tag;
	Camera* mp_mainCamera   = nullptr;
	Camera* mp_activeCamera = nullptr;

private:

	uint32 m_flag = 0;
	std::vector<Text*> m_texts;
	std::vector<std::string> m_activeTextures;


private:
	// DEBUG
	bool m_isVisualDebugActive = false;
	bool m_debug = false;
	bool m_debugPerf = false;
	bool m_frustrumCulling = true;
	int32 m_updateDebug = 0;

	std::vector<Text*> m_debugInfoTexts;
	std::vector<Text*> m_debugTexts;
	Entity* m_selectedEntity = nullptr;

	Text* mp_Position			= nullptr;
	Text* mp_PosX				= nullptr;
	Text* mp_PosY				= nullptr; 

	Text* mp_Rotation			= nullptr;
	Text* mp_RotZ				= nullptr; 

	Text* mp_Scale				= nullptr;
	Text* mp_ScaleX				= nullptr;
	Text* mp_ScaleY				= nullptr; 

	Text* mp_Frame				= nullptr;
	Text* mp_FPS				= nullptr;

	Text*  mp_Colliders			= nullptr;
	Text*  mp_CollidersP		= nullptr;

	Text*  mp_Entity			= nullptr;
	Text*  mp_EntityP			= nullptr;

	Text*  mp_Input				= nullptr; 
	Text*  mp_InputP			= nullptr;

	Text*  mp_Update			= nullptr;
	Text*  mp_UpdateP			= nullptr;

	Text*  mp_Draw 				= nullptr;
	Text*  mp_DrawP				= nullptr;

	Text*  mp_QuadTree			= nullptr;
	Text* mp_FrustrumCulling	= nullptr;

private: 
	friend class GameManager;
	friend class SceneManager;
};

#include "Scene.inl"