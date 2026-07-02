#pragma once
#include "Entity.h"
#include "Render/Text.h"


class WorldText : public Entity{

	Text* m_text = nullptr;
	friend class Scene;
public:
	void OnInitialize() override;
	void OnUpdate() override;

	Text* GetText() { return m_text; }

	WorldText(): Entity() {};
	
	void SetRenderPosition(Vector2f pos);
private:
	Vector2f m_renderPosition = { 0, 0 };
};

