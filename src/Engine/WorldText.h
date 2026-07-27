#pragma once
#include "Entity.h"
#include "Render/Text.h"

class WorldText : public Entity 
{
public:
	WorldText() : Entity() {};
	~WorldText() override;

	void OnInitialize() override;
	void OnUpdate() override;

	Text* GetText();
	void SetTextObject(Text* pText);

	void SetRenderPosition(Vector2f pos);
	 
	void SetText(const std::string& text);
	void SetFont(const std::string& id);
	void SetTextColor(byte r, byte g, byte b, byte a = 255);
	void SetFontSize(int32 size);
	int32 GetFontSize() const;
	Vector2f GetTextSizes() const;

	void SetActive(bool active) override;

private:
	Text* m_Text = nullptr;
	Vector2f m_RenderPosition = { 0, 0 };
};