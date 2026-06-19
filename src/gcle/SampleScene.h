#pragma once
#include "Engine/Scene.h"

class SampleScene : public Scene
{
	public:
		void OnInitialize() override;
		void OnUpdate() override;

	private:
		Entity* pEntity = nullptr;
		Entity* otherEntity = nullptr;
};

