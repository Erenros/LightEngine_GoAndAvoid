#pragma once
#include "Entity.h"
#include "Core/InputManager.h"


class DebugPlayer : public Entity {
	void OnUpdate() override;
	void OnInitialize() override;
};

