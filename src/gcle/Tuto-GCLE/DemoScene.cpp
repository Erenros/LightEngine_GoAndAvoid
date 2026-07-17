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
	SetSelectedEntity(mp_Player);

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
			mp_Player->SetCurrentLife(std::floor(value));
		}
	);
	mp_HealthBar->SetInteractable(true);
	mp_HealthBar->SetLayer(11);
	 
	mp_PlayerPortrait = CreateImage(gcle::Shapes::Rectangle, "player_portrait");
	mp_PlayerPortrait->SetRenderSize(0, { 64.0f, 64.0f });
	mp_PlayerPortrait->SetPosition(40.0f, 40.0f);
	mp_PlayerPortrait->SetLayer(10);
	 
	mp_PauseButton = CreateButton(gcle::Shapes::Rectangle, "Pause");
	mp_PauseButton->SetPosition(500.0f, 200.0f);
	mp_PauseButton->AddFunction([]()
		{
			Clock* pClock = GameManager::GetInstance().GetTime();
			pClock->SetTimeScale(pClock->GetTimeScale() > 0.0 ? 0.0 : 1.0);
		});
	 
	mp_MuteToggle = CreateToggle(gcle::Shapes::Rectangle, "icon_mute", "icon_sound");
	mp_MuteToggle->SetRenderSize(0, { 48.0f, 48.0f });
	mp_MuteToggle->SetPosition(1600.0f, 240.0f); 
	mp_MuteToggle->SetOnValueChanged([](bool isMuted)
		{
			RessourceManager::GetInstance().SetMusicVolume(isMuted ? 0 : 128);
		});
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
}