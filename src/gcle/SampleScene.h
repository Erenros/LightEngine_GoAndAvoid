#pragma once
#include "Engine/Scene.h"

class SampleScene : public Scene
{
public:
	void OnInitialize() override;
	void OnUpdate(Timer& time) override;

private:
	Entity* pEntity = nullptr;
};

