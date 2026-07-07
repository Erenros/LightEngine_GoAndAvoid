#include "Scene.h"
#include "Render/Text.h"
#include "PhysicsManager.h"

#include <sstream>
#include <iomanip>
#include "WorldText.h"
#include "RessourceManager.h"
#include "InputManager.h"

void Scene::DrawDebug(Window* window)
{
	constexpr Vector2f screenMiddle({ 1920.0f / 2.0f, 1080.0f / 2.0f });
	Vector2f camOffset = mp_activeCamera != nullptr ? (screenMiddle - mp_activeCamera->GetPosition()) : Vector2f{ 0.f, 0.f };

	if (m_isVisualDebugActive)
	{
		for (auto& layer : GameManager::GetInstance().m_entities)
		{
			for (Entity* e : layer)
			{
				if (!e->IsActiveIn(m_tag))
					continue;

				for (auto& col : e->GetColliders())
				{
					if (col->IsActive())
					{
						GameManager::GetInstance().GetWindow()->ClearWindowWithColor(0, 255, 0, 255);
						GameManager::GetInstance().GetWindow()->DrawDebug(col->GetShape(), camOffset);
					}
				}
			}
		}
	}

	if (m_selectedEntity != nullptr && m_debugPerf && m_selectedEntity->IsActiveIn(m_tag))
	{
		GameManager::GetInstance().GetWindow()->ClearWindowWithColor(255, 255, 0, 255);
		GameManager::GetInstance().GetWindow()->DrawDebug(m_selectedEntity->GetRenderShape()); 

		for (auto& col : m_selectedEntity->GetColliders())
		{
			GameManager::GetInstance().GetWindow()->ClearWindowWithColor(0, 255, 0, 255);
			GameManager::GetInstance().GetWindow()->DrawDebug(col->GetShape(), camOffset); 
		}
	}
}


void Scene::Draw(Window* window) 
{
	m_numberOfDraw = 0;
	for (auto& layer : GameManager::GetInstance().m_entities) {
		for(Entity* e : layer){
			if (window->IsInsideWindow(e) && e->GetRenderShape() != nullptr && m_frustrumCulling) 
			{
				if (e->IsActiveIn(m_tag)) 
				{
					GameManager::GetInstance().GetWindow()->Draw(e->GetRenderShape());
					m_numberOfDraw++;
				}
			}
			else if (e->GetRenderShape() != nullptr)
			{
				if (e->IsActiveIn(m_tag))
				{
					GameManager::GetInstance().GetWindow()->Draw(e->GetRenderShape());
					m_numberOfDraw++;
				}
			}
			else if (e->IsWorldText()) 
			{
				if (e->IsActiveIn(m_tag)) 
				{
					WorldText* text = static_cast<WorldText*>(e);
					GameManager::GetInstance().GetWindow()->DrawTextOnRenderer(text->GetText());
					m_numberOfDraw++;
				}
			}
		}
	}

	for (auto& layer : GameManager::GetInstance().m_UIs) {
		for (UI* ui : layer) {
			if (window->IsInsideWindow(ui) && ui->GetRenderShape() != nullptr && m_frustrumCulling)
			{
				if (ui->IsActiveIn(m_tag))
				{
					GameManager::GetInstance().GetWindow()->Draw(ui->GetRenderShape());
					m_numberOfDraw++;
				}
			}
			else if (ui->GetRenderShape() != nullptr)
			{
				if (ui->IsActiveIn(m_tag))
				{
					GameManager::GetInstance().GetWindow()->Draw(ui->GetRenderShape());
					m_numberOfDraw++;
				}
			}
		}
	}

	for (Text* t : m_texts)
	{
		GameManager::GetInstance().GetWindow()->DrawTextOnRenderer(t);
		m_numberOfDraw++;
	}

	if (m_debug && m_selectedEntity != nullptr)
	{
		for (Text* t : m_debugInfoTexts)
			GameManager::GetInstance().GetWindow()->DrawTextOnRenderer(t);
	}

	if (m_debug && m_debugPerf)
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

	for (Text* t : m_debugInfoTexts)
		delete t;
	m_debugInfoTexts.clear();

	mp_mainCamera = nullptr;
	mp_activeCamera = nullptr;
}

Text* Scene::CreateText(const std::string& text,Vector2f pos, int32 fontSize, byte r, byte g, byte b)
{
	Font* font = RessourceManager::GetInstance().GetFont("Hack-Regular");
	if (font == nullptr)
	{
		GCLE_WARN << "Couldn't find the default font" << ENDL;
		return nullptr;
	}

	Text* new_text = GCLE_NEW Text(font,text,pos, fontSize, r, g, b);
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

	pCamera->SetActive(true);
	mp_activeCamera = pCamera;

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

void Scene::Update(Clock& time) const 
{ 
	for (auto& layer : GameManager::GetInstance().m_entities) {
		for(Entity* e : layer)
		{
			if (e->IsActiveIn(m_tag))
				e->Update(static_cast<float32>(time.GetDeltaTime()));
		}
	} 
	for (auto& layer : GameManager::GetInstance().m_UIs) {
		for(UI* ui : layer)
		{
			if (ui->IsActiveIn(m_tag))
				ui->Update(static_cast<float32>(time.GetDeltaTime()));
		}
	} 

	if (InputManager::GetInstance().IsDown(LeftButton))
	{
		for (auto it : buttons) {
			if (it.IsInside(mp_activeCamera->GetScreenMousePosition()))
				it.OnClick();
		}
	}
}

Text* Scene::CreateDebugText(const std::string& text, Vector2f pos, int32 fontSize, byte r, byte g, byte b)
{
	Font* font = RessourceManager::GetInstance().GetFont("Hack-Regular");
	if (font == nullptr)
	{
		GCLE_WARN << "Couldn't find the default font" << ENDL;
		return nullptr;
	}

	Text* new_text = GCLE_NEW Text(font, text, pos, fontSize, r, g, b);
	m_debugTexts.push_back(new_text);
	return new_text;
}

void Scene::DestroyDebugText(Text* text)
{
	std::erase_if(m_debugTexts, [text](Text* t) { return t == text; });
	delete text;
}

Text* Scene::CreateDebugInfoText(const std::string& text, Vector2f pos, int32 fontSize, byte r, byte g, byte b)
{
	Font* font = RessourceManager::GetInstance().GetFont("Hack-Regular");
	if (font == nullptr)
	{
		GCLE_WARN << "Couldn't find the default font" << ENDL;
		return nullptr;
	}

	Text* new_text = GCLE_NEW Text(font, text, pos, fontSize, r, g, b);
	m_debugInfoTexts.push_back(new_text);
	return new_text;
}

void Scene::DestroyDebugInfoText(Text* text)
{
	std::erase_if(m_debugInfoTexts, [text](Text* t) { return t == text; });
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

	if (m_selectedEntity != nullptr && m_updateDebug >= debugConstant)
	{ 
		Vector2f pos = m_selectedEntity->GetPosition();
		Degrees rot = m_selectedEntity->GetRotation();
		Vector2f scale = m_selectedEntity->GetScale();


		mp_PosX->SetText(std::to_string(pos.x));
		mp_PosY->SetText(std::to_string(pos.y));

		mp_RotZ->SetText(std::to_string(rot));

		mp_ScaleX->SetText(std::to_string(scale.x));
		mp_ScaleY->SetText(std::to_string(scale.y));


		m_updateDebug = 0;
	}
}

void Scene::DebugSetEntityInfo()
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

		if (InputManager::GetInstance().IsHeld(LeftArrow))
		{
			m_selectedEntity->Rotate(-0.1f);
		}

		if (InputManager::GetInstance().IsHeld(RightArrow))
		{
			m_selectedEntity->Rotate(0.1f);
		}

		if (InputManager::GetInstance().IsHeld(UpArrow))
		{
			m_selectedEntity->ScaleBy({ 1.01f, 1.01f });
		}

		if (InputManager::GetInstance().IsHeld(DownArrow))
		{
			m_selectedEntity->ScaleBy({ 0.99f, 0.99f });
		}
	}
}

void Scene::OnInitialize()
{
	SetDebug();
	mp_mainCamera = CreateCamera();
	mp_mainCamera->SetActive(true);
	mp_activeCamera = mp_mainCamera;

	mp_Position				= CreateDebugInfoText("Pos: "		, { 0,		0	},		25,		255, 225, 255);
	mp_PosX					= CreateDebugInfoText(""			, { 100,	0	},		25,		255, 225, 255);
	mp_PosY					= CreateDebugInfoText(""			, { 300,	0	},		25,		255, 225, 255);

	mp_Rotation				= CreateDebugInfoText("Rot: "		, { 0,		75  },		25,		255, 225, 255);
	mp_RotZ					= CreateDebugInfoText(""			, { 100,	75  },		25,		255, 225, 255);

	mp_Scale				= CreateDebugInfoText("Scale: "		, { 0,		150 },		25,		255, 225, 255);
	mp_ScaleX				= CreateDebugInfoText(""			, { 100,	150 },		25,		255, 225, 255);
	mp_ScaleY				= CreateDebugInfoText(""			, { 300,	150 },		25,		255, 225, 255);

	mp_Frame				= CreateText("FPS"					, { 1860,	0	},		25,		255, 225, 255);
	mp_FPS					= CreateText(""						, { 1800,	0	},		25,		255, 225, 255);
																
	mp_Colliders			= CreateDebugText("Colliders: "		, { 0,		1000 },		25,		255, 225, 255);
	mp_CollidersP			= CreateDebugText(""				, { 150,	1000 },		25,		255, 225, 255);
																
	mp_Entity				= CreateDebugText("Entity C/D: "	, { 275,	1000 },		25,		255, 225, 255);
	mp_EntityP				= CreateDebugText(""				, { 450,	1000 },		25,		255, 225, 255);
																
	mp_Input				= CreateDebugText("InputManager: "	, { 560,	1000 },		25,		255, 225, 255);
	mp_InputP				= CreateDebugText(""				, { 760,	1000 },		25,		255, 225, 255);
																
	mp_Update				= CreateDebugText("Update: "		, { 870,	1000 },		25,		255, 225, 255);
	mp_UpdateP				= CreateDebugText(""				, { 975,	1000 },		25,		255, 225, 255);
																
	mp_Draw 				= CreateDebugText("Draw: "			, { 1085,	1000 },		25,		255, 225, 255);
	mp_DrawP				= CreateDebugText(""				, { 1160,	1000 },		25,		255, 225, 255);

	mp_QuadTree				= CreateDebugText("QuadTree"		, { 1280,	1000 },		25,		0  , 225, 0);
	mp_FrustrumCulling      = CreateDebugText("FrustrumCulling"	, { 1420,	1000 },		25,		0  , 225, 0);

	mp_NumberDraw			= CreateDebugText("Number To Draw: ", {1085, 1050},			25,		255, 255, 255);
	mp_NumberDrawP			= CreateDebugText(""				, {1350, 1050},			25,		255, 255, 255);
}

void Scene::OnUpdate(Clock& time)
{ 
	constexpr int32 DEBUG_UPDATE = 5; 
	
	m_updateDebug++;

	//GCLE_INFO << GameManager::GetInstance().GetWindow()->GetMousePositionOnRenderTarget().x << " " << GameManager::GetInstance().GetWindow()->GetMousePositionOnRenderTarget().y << ENDL;
	//GCLE_INFO << GameManager::GetInstance().GetWindow()->GetMousePosition().x << " " << GameManager::GetInstance().GetWindow()->GetMousePosition().y << ENDL;
	//GCLE_INFO << mp_activeCamera->GetMouseScreenToWorldPosition().x << " " << mp_activeCamera->GetMouseScreenToWorldPosition().y << ENDL;

	if (InputManager::GetInstance().IsDown(F1))
		m_debugPerf = !m_debugPerf;

	// QuadTree debug
	if (InputManager::GetInstance().IsDown(F2) && m_debugPerf)
	{
		bool isActive = PhysicsManager::GetInstance().IsQuadTreeActive();

		if (isActive)
		{
			mp_QuadTree->SetColor(255, 0, 0, 0);
			PhysicsManager::GetInstance().SetActivateQuadTree(!isActive);
		}
		else
		{
			mp_QuadTree->SetColor(0, 255, 0, 0);
			PhysicsManager::GetInstance().SetActivateQuadTree(!isActive);
		}

		
	}
	
	// Frustrum Culling
	if (InputManager::GetInstance().IsDown(F3) && m_debugPerf)
	{ 
		if (m_frustrumCulling)
		{
			m_frustrumCulling = false;
			mp_FrustrumCulling->SetColor(255, 0, 0, 0); 
		}
		else
		{
			m_frustrumCulling = true;
			mp_FrustrumCulling->SetColor(0, 255, 0, 0);
		} 
	}

	// Gizmo Colliders
	if (InputManager::GetInstance().IsDown(F4) && m_debugPerf)
	{
		SetGizmoVisibility();
	}

	if (m_updateDebug >= 1)
	{
		mp_FPS->SetText(std::to_string(time.GetFramePerSecond()));
	}

	if (m_debugPerf)
		SetDebugInfo();
	
	EntityInfoVisibility(DEBUG_UPDATE);
	DebugSetEntityInfo(); 
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

void Scene::SetDebugInfo() const
{
	if (m_debugPerf && m_updateDebug >= 4)
	{
		auto FormatMs = +[](float value)
		{
			std::ostringstream oss;
			oss << std::fixed << std::setprecision(2) << value;
			return oss.str();
		};

		mp_CollidersP->SetText(	FormatMs(PROFILER_GET("Colliders"))	+ "ms");
		mp_EntityP->SetText(	FormatMs(PROFILER_GET("Entity"))	+ "ms");
		mp_InputP->SetText(		FormatMs(PROFILER_GET("Input"))		+ "ms");
		mp_UpdateP->SetText(	FormatMs(PROFILER_GET("SceneU"))	+ "ms");
		mp_DrawP->SetText(		FormatMs(PROFILER_GET("SceneD"))	+ "ms");

		mp_NumberDrawP->SetText(std::to_string(m_numberOfDraw));
	}
}

Button* Scene::CreateButton(gcle::Shapes shape, std::vector<std::function<void* ()>> functions)
{
	Button* button = CreateUI<Button>(shape);
	buttons.push_back(*button);
	button->SetFunctions(functions);
	return button;
}

Entity* Scene::CreateWorldText( const std::string& text, int32 fontSize, const std::string& fontId, byte r, byte g, byte b, byte a)
{
	WorldText* worldText = CreateEntity<WorldText>();
	Font* font = RessourceManager::GetInstance().GetFont(fontId);
	if (font == nullptr)
	{
		GCLE_WARN << "Couldn't find the font " << fontId <<  ENDL;
		return nullptr;
	}

	Text* textObject = GCLE_NEW Text(font, text, worldText->GetPosition(), fontSize, r, g, b, a);
	worldText->m_text = textObject;
	Entity* entity = worldText;

	return entity;
}