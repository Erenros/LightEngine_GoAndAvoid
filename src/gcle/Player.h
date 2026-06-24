#pragma once
#include "Entity.h"
#include "Core/InputManager.h"


class Player: public Entity{
	void OnUpdate() override;
	void OnInitialize() override;
};

