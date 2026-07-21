#pragma once
#include "Entity.h"
#include "Core/InputManager.h"


class Player: public Entity
{
public:
	void OnUpdate() override;
	void OnInitialize() override;
};

