#pragma once
#include "Entity.h"
#include "Render/Text.h"


class WorldText : public Entity{

public:
	void OnInitialize() override;
	void OnUpdate() override;

	Text* GetText() { return m_text; }

	WorldText(): Entity() {};
	
	void SetRenderPosition(Vector2f pos);
private:
	Vector2f m_renderPosition = { 0, 0 };

	Text* m_text = nullptr;

private: 
	friend class Scene;
};

