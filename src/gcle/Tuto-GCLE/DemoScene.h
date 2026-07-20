#pragma once

#include "Engine/Scene.h"
#include "Tuto-GCLE/SmoothCamera.h"

class Panel;
class Slider;
class Button;
class Toggle;
class Image;

namespace Demo
{
	class GCPlayer;
	class GCEnemy;
}

class DemoScene : public Scene
{
	void OnInitialize() override;
	void OnUpdate(Clock& time) override;

private:

	Camera* pSceneCamera = nullptr;

	Demo::GCPlayer* mp_Player = nullptr;
	Demo::GCEnemy* mp_Enemy = nullptr;

	Demo::SmoothCamera m_Smooth;

	// --- HUD ---
	Panel* mp_HealthBarBackground = nullptr;
	Slider* mp_HealthBar = nullptr;
	Button* mp_PauseButton = nullptr;
	Toggle* mp_MuteToggle = nullptr;
	Image* mp_PlayerPortrait = nullptr;

	// --- Pause ---
	Panel* mp_PauseMenu = nullptr;

	void InitializeHUD();
	void UpdateHUD();
};