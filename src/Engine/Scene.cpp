#include "Scene.h"
#include "Render/Text.h"

void Scene::DrawDebug(Window* window)
{
	for (auto layer : GameManager::GetInstance().m_entities) {
		for(Entity* e : layer)
		{
			if (e->IsActiveIn(m_tag) && e->m_isHighlighted) {
				if (e->CollidingEntity.empty())
				{
					GameManager::GetInstance().GetWindow()->ClearWindowWithColor(0, 255, 0, 255);
				}
				else
				{
					GameManager::GetInstance().GetWindow()->ClearWindowWithColor(255, 0, 0, 255);
				}

				if (m_isVisualDebugActive)
				{
					GameManager::GetInstance().GetWindow()->DrawDebug(e->GetRenderShape());
				}
				else if (m_selectedEntity != nullptr)
				{
					if (e->GetId() == m_selectedEntity->GetId())
					{
						GameManager::GetInstance().GetWindow()->ClearWindowWithColor(255, 255, 0, 255);
						GameManager::GetInstance().GetWindow()->DrawDebug(e->GetRenderShape());
					}
				}
			}
		}
	}
}


void Scene::Draw(Window* window) {
	for (auto layer : GameManager::GetInstance().m_entities) {
		for(Entity* e : layer)
		{
			if(e->IsActiveIn(m_tag))
				GameManager::GetInstance().GetWindow()->Draw(e->GetRenderShape());
			
		}
	}

	for (Text* t : m_texts)
		GameManager::GetInstance().GetWindow()->DrawTextOnRenderer(t);

	if (m_debug && m_selectedEntity != nullptr)
	{
		for (Text* t : m_debugTexts)
			GameManager::GetInstance().GetWindow()->DrawTextOnRenderer(t);
	}
}

Scene::~Scene()
{
	for (Text* t : m_texts)
		delete t;
	m_texts.clear();

	for (Text* t : m_debugTexts)
		delete t;
	m_debugTexts.clear();
}

Text* Scene::CreateText(const std::string& text, int x, int y, int w, int h, byte r, byte g, byte b)
{
	Font* font = RessourceManager::GetInstance().GetFont("Hack-Regular");
	if (font == nullptr)
	{
		GCLE_WARN << "Couldn't find the default font" << ENDL;
		return nullptr;
	}

	Text* new_text = GCLE_NEW Text(font,text, x, y, w, h, r, g, b);
	m_texts.push_back(new_text);
	return new_text;
}

void Scene::DestroyText(Text* text)
{
	std::erase_if(m_texts, [text](Text* t) { return t == text; });
	delete text;
}

Camera* Scene::CreateCamera()
{
	Camera* pCamera = GCLE_NEW Camera();
	pCamera->Init(GameManager::GetInstance().GetWindow());

	GameManager::GetInstance().m_camera.push_back(pCamera);
	return pCamera;
}

void Scene::AddDrawnTexture(const std::string& textureName){
	if (std::find(m_activeTextures.begin(), m_activeTextures.end(), textureName) != m_activeTextures.end()) {
		return;
	}
	m_activeTextures.push_back(textureName);
}

bool Scene::isDrawn(const std::string& tag){
	return (std::find(m_activeTextures.begin(), m_activeTextures.end(), tag) != m_activeTextures.end());
}

void Scene::SetDebug()
{
	m_debug = true;
}

void Scene::Update(Clock& time){
	for (auto layer : GameManager::GetInstance().m_entities) 
	{
		for(Entity* e : layer)
		{
			if (e->IsActiveIn(m_tag))
				e->Update(time);
		}
	}
}

Text* Scene::CreateDebugText(const std::string& text, int x, int y, int w, int h, byte r, byte g, byte b)
{
	Font* font = RessourceManager::GetInstance().GetFont("Hack-Regular");
	if (font == nullptr)
	{
		GCLE_WARN << "Couldn't find the default font" << ENDL;
		return nullptr;
	}

	Text* new_text = GCLE_NEW Text(font, text, x, y, w, h, r, g, b);
	m_debugTexts.push_back(new_text);
	return new_text;
}

void Scene::DestroyDebugText(Text* text)
{
	std::erase_if(m_debugTexts, [text](Text* t) { return t == text; });
	delete text;
}

void Scene::SetGizmoVisibility()
{
	m_isVisualDebugActive = !m_isVisualDebugActive;
}

void Scene::EntityInfoVisibility(const int32 debugConstant)
{
	if (InputManager::GetInstance().IsDown(LeftButton))
	{
		bool selected = false;
		for (auto& layer : GameManager::GetInstance().m_entities)
		{
			for (Entity* e : layer)
			{
				if (e->IsInside(mp_activeCamera->GetScreenMousePosition()))
				{
					m_selectedEntity = e;
					m_selectedEntity->m_isHighlighted = true;
					selected = true;
					break;
				}
			}
		}

		if (selected == false)
		{
			if (m_selectedEntity != nullptr)
			{
				m_selectedEntity->m_isHighlighted = false;
				m_selectedEntity = nullptr;
			}
		}
	}

	if (m_selectedEntity != nullptr && m_updateDebug > debugConstant)
	{


		Vector2f pos = m_selectedEntity->GetPosition();
		Degrees rot = m_selectedEntity->GetRotation();
		Vector2f scale = m_selectedEntity->GetScale();


		mp_PosX->SetText(std::to_string(pos.x));
		mp_PosY->SetText(std::to_string(pos.y));

		mp_RotZ->SetText(std::to_string(rot));

		mp_ScaleX->SetText(std::to_string(scale.x));
		mp_ScaleY->SetText(std::to_string(scale.y));
	}
}

void Scene::DebugSetEntityPosition()
{
	if (m_selectedEntity != nullptr)
	{
		if (InputManager::GetInstance().IsDown(RightButton))
		{
			m_selectedEntity->SetPosition(
				mp_activeCamera->GetMouseScreenToWorldPosition().x,
				mp_activeCamera->GetMouseScreenToWorldPosition().y
			);
		}
	}
}

void Scene::OnInitialize()
{
	SetDebug();
	mp_mainCamera	= CreateCamera();
	mp_mainCamera->SetActive(true);
	mp_activeCamera = mp_mainCamera;

	mp_Position		= CreateDebugText("Pos: ",		0,		0,		100,	50,	255, 225, 255);
	mp_PosX			= CreateDebugText("",			125,	0,		100,	50,	255, 225, 255);
	mp_PosY			= CreateDebugText("",			250,	0,		100,	50,	255, 225, 255); 

	mp_Rotation		= CreateDebugText("Rot: ",		0,		75,		100,	50,	255, 225, 255);
	mp_RotZ			= CreateDebugText("",			125,	75,		100,	50,	255, 225, 255); 

	mp_Scale		= CreateDebugText("Scale: ",	0,		150,	100,	50,	255, 225, 255);
	mp_ScaleX		= CreateDebugText("",			125,	150,	100,	50,	255, 225, 255);
	mp_ScaleY		= CreateDebugText("",			250,	150,	100,	50,	255, 225, 255); 

	mp_Frame		= CreateText("FPS: ",	1600, 0, 100, 50, 255, 225, 255);
	mp_FPS			= CreateText("",		1750, 0, 50, 50, 255, 225, 255);
}

void Scene::OnUpdate(Clock& time)
{ 
	constexpr int32 DEBUG_UPDATE = 5;

	m_updateDebug++;
	
	EntityInfoVisibility(DEBUG_UPDATE);
	DebugSetEntityPosition();

	GCLE_INFO << GameManager::GetInstance().GetWindow()->GetMousePositionOnRenderTarget().x << " " << GameManager::GetInstance().GetWindow()->GetMousePositionOnRenderTarget().y << ENDL;
	GCLE_INFO << GameManager::GetInstance().GetWindow()->GetMousePosition().x << " " << GameManager::GetInstance().GetWindow()->GetMousePosition().y << ENDL;
	GCLE_INFO << mp_activeCamera->GetMouseScreenToWorldPosition().x << " " << mp_activeCamera->GetMouseScreenToWorldPosition().y << ENDL;

	if (m_updateDebug > DEBUG_UPDATE)
	{
		m_updateDebug = 0;
		mp_FPS->SetText(std::to_string(time.GetFramePerSecond()));
	}

}

void Scene::OnExit()
{

}

Camera* Scene::GetMainCamera()
{
	return mp_mainCamera;
}

Camera* Scene::GetCurrentCamera()
{
	return mp_activeCamera;
}

void Scene::SwitchCamera(Camera* pCamera)
{
	for (auto& cam : GameManager::GetInstance().m_camera)
	{
		cam->SetActive(false);

		if (cam->GetId() == pCamera->GetId())
		{
			cam->SetActive(true);
			mp_activeCamera = cam;
		}
	}
}
