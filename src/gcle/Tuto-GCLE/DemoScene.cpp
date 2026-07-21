#include "DemoScene.h"

#include "Engine/PhysicsManager.h"
#include "Engine/RessourceManager.h"
#include "Engine/GameManager.h"
#include "Engine/Panel.h"
#include "Engine/Slider.h"
#include "Engine/Button.h"
#include "Engine/Toggle.h"
#include "Engine/Image.h"
#include "Render/Text.h"

#include "Tuto-GCLE/DemoPlayer.h"
#include "Tuto-GCLE/GCEnemy.h"
#include "Tuto-GCLE/Table.h"
#include "Tuto-GCLE/Wall.h"
#include "Tuto-GCLE/Tag.h"

void DemoScene::OnInitialize()
{
	Scene::OnInitialize();

	pSceneCamera = CreateCamera();
	SwitchCamera(pSceneCamera);

	mp_Player = CreateEntity<Demo::GCPlayer>(gcle::Shapes::Rectangle);
	//SetSelectedEntity(mp_Player);

	m_Smooth.Initialize(pSceneCamera, mp_Player);

	mp_Enemy = CreateEntity<Demo::GCEnemy>(gcle::Shapes::Rectangle);
	mp_Enemy->SetTarget(mp_Player);
	mp_Enemy->SetDetectionRange(600.0f);
	mp_Enemy->SetAttackRange(400.0f);
	mp_Enemy->SetPosition(100.0f, 100.0f);

	Demo::Wall* pWall = CreateEntity<Demo::Wall>(gcle::Shapes::Rectangle);
	pWall->SetPosition(0, -400);
	pWall->SetScale({ 8.0f, 1.0f });

	Demo::Wall* pWall2 = CreateEntity<Demo::Wall>(gcle::Shapes::Rectangle);
	pWall2->SetPosition(0, 400);
	pWall2->SetScale({ 8.0f, 1.0f });

	Demo::Wall* pWall3 = CreateEntity<Demo::Wall>(gcle::Shapes::Rectangle);
	pWall3->SetPosition(-400, 0);
	pWall3->SetScale({ 1.0f, 8.0f });

	Demo::Wall* pWall4 = CreateEntity<Demo::Wall>(gcle::Shapes::Rectangle);
	pWall4->SetPosition(400, 0);
	pWall4->SetScale({ 1.0f, 8.0f });

	PhysicsManager::GetInstance().SetFrameBetweenQuadTreeRegenerations(1);
	PhysicsManager::GetInstance().SetActivateQuadTree(true);
	PhysicsManager::GetInstance().SetQuadTreePos1({ -5000, -5000 });
	PhysicsManager::GetInstance().SetQuadTreePos2({ 5000, 5000 });

	InitializeHUD();
}

void DemoScene::InitializeHUD()
{
	constexpr float32 HEALTHBAR_WIDTH = 256.0f;
	constexpr float32 HEALTHBAR_HEIGHT = 32.0f;
	 
	mp_HealthBarBackground = CreatePanel(gcle::Shapes::Rectangle, Color{ 30, 30, 30, 200 });
	mp_HealthBarBackground->SetRenderSize(0, { HEALTHBAR_WIDTH, HEALTHBAR_HEIGHT }); 
	mp_HealthBarBackground->SetPosition(148.0f, 40.0f);         
	mp_HealthBarBackground->SetLayer(10);

	mp_HealthBar = CreateSlider(gcle::Shapes::Rectangle);
	//mp_HealthBar->SetVisualMode(SliderVisualMode::Handle);
	//mp_HealthBar->SetHandleTexture("knob");
	//mp_HealthBar->SetHandleSizeRatio(0.10f);
	mp_HealthBar->SetVisualMode(SliderVisualMode::Fill);
	mp_HealthBar->SetFillAnchor(SliderFillAnchor::Start); 
	mp_HealthBar->SetRenderSize(0, { HEALTHBAR_WIDTH, HEALTHBAR_HEIGHT });
	mp_HealthBar->SetPosition(148.0f, 40.0f);
	mp_HealthBar->SetColor(Color::Red);
	mp_HealthBar->SetRange(0.0f, static_cast<float32>(mp_Player->GetMaxLife()));
	mp_HealthBar->SetValue(static_cast<float32>(mp_Player->GetCurrentLife()), false);
	mp_HealthBar->SetOnValueChanged(
		[this](float32 value)
		{
			mp_Player->SetCurrentLife(static_cast<int>(std::floor(value)));
		}
	);
	mp_HealthBar->SetInteractable(true);
	mp_HealthBar->SetLayer(11);
	 
	mp_PlayerPortrait = CreateImage(gcle::Shapes::Rectangle, "player_portrait");
	mp_PlayerPortrait->SetRenderSize(0, { 64.0f, 64.0f });
	mp_PlayerPortrait->SetPosition(40.0f, 40.0f);
	mp_PlayerPortrait->SetLayer(10);
	 
	mp_MuteToggle = CreateToggle(gcle::Shapes::Rectangle, "icon_mute", "icon_sound");
	mp_MuteToggle->SetRenderSize(0, { 48.0f, 48.0f });
	mp_MuteToggle->SetPosition(1600.0f, 240.0f); 
	mp_MuteToggle->SetOnValueChanged([](bool isMuted)
		{
			RessourceManager::GetInstance().SetMusicVolume(isMuted ? 0 : 128);
		});

	m_UIMainHUD = { mp_HealthBarBackground, mp_HealthBar, mp_PlayerPortrait, mp_MuteToggle};

	InitializePauseHUD();
}

void DemoScene::InitializePauseHUD()
{
	constexpr float WIDTH_MULTIPLYER = 2.0f;
	constexpr float CENTER_X = 960.0f;
	constexpr float CENTER_Y = 540.0f;

	mp_PauseMenu = CreatePanel(gcle::Shapes::Rectangle, Color{ 30, 30, 30, 200 });
	mp_PauseMenu->SetRenderSize(0, { (WIDTH_MULTIPLYER + 0.5f) * 100, 864.0f });
	mp_PauseMenu->SetPosition(CENTER_X, CENTER_Y);
	mp_PauseMenu->SetLayer(14);

	mp_ResumeButton = CreateButton(gcle::Shapes::Rectangle, "Resume");
	mp_ResumeButton->SetPosition(CENTER_X, 288.0f);
	mp_ResumeButton->ScaleBy({ WIDTH_MULTIPLYER , 1.0f});
	mp_ResumeButton->SetLayer(15);
	mp_ResumeButton->AddFunction([this]()
		{
			HandleEscape();
		});

	mp_OptionButton = CreateButton(gcle::Shapes::Rectangle, "Options");
	mp_OptionButton->SetPosition(CENTER_X, 576.0f);
	mp_OptionButton->ScaleBy({ WIDTH_MULTIPLYER , 1.0f});
	mp_OptionButton->SetLayer(15);
	mp_OptionButton->AddFunction([this]()
		{
			OpenMenu(m_UIOptionMenu);
		});

	mp_QuitButton = CreateButton(gcle::Shapes::Rectangle, "Quit");
	mp_QuitButton->SetPosition(CENTER_X, 864.0f);
	mp_QuitButton->ScaleBy({ WIDTH_MULTIPLYER , 1.0f });
	mp_QuitButton->SetLayer(15);
	mp_QuitButton->AddFunction([]()
		{
			GameManager::GetInstance().Quit();
		});


	m_UIPauseMenu = { mp_PauseMenu, mp_ResumeButton, mp_OptionButton, mp_QuitButton };

	SetMenuActive(m_UIPauseMenu, false);
}

void DemoScene::SetMenuActive(std::vector<UI*>& ui, bool active)
{
	for (auto& element : ui)
	{
		if (element != nullptr)
			element->SetActive(active);
	}
}

void DemoScene::OpenMenu(std::vector<UI*>& menu)
{
	if (!m_MenuStack.empty())
		SetMenuActive(*m_MenuStack.back(), false);

	SetMenuActive(menu, true);
	m_MenuStack.push_back(&menu);
}

void DemoScene::CloseCurrentMenu()
{
	if (m_MenuStack.empty())
		return;

	SetMenuActive(*m_MenuStack.back(), false);
	m_MenuStack.pop_back();

	if (!m_MenuStack.empty())
		SetMenuActive(*m_MenuStack.back(), true);
}

void DemoScene::HandleEscape()
{
	Clock* pClock = GameManager::GetInstance().GetTime();

	if (m_MenuStack.empty())
	{ 
		OpenMenu(m_UIPauseMenu);
		pClock->SetTimeScale(0.0);
	}
	else if (m_MenuStack.back() == &m_UIPauseMenu)
	{ 
		CloseCurrentMenu();
		pClock->SetTimeScale(1.0);
	}
	else
	{ 
		CloseCurrentMenu();
	}
}

void DemoScene::UpdateHUD()
{
	if (mp_Player == nullptr || mp_HealthBar == nullptr)
		return;

	mp_HealthBar->SetValue(static_cast<float32>(mp_Player->GetCurrentLife()), true);
}

void DemoScene::OnUpdate(Clock& time)
{
	Scene::OnUpdate(time);

	m_Smooth.Update(static_cast<float>(time.GetDeltaTime()));

	UpdateHUD();

	if (InputManager::GetInstance().IsDown('O'))
		SceneManager::GetInstance().SetCurrentSceneWithTag("Sample", true); 
	
	if (InputManager::GetInstance().IsDown(Escape))
	{
		HandleEscape();
	}
}