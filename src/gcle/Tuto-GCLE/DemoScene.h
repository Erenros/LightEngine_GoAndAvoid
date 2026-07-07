#pragma once

#include "Engine/Scene.h"

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
	Demo::GCEnemy*  mp_Enemy  = nullptr;

};

