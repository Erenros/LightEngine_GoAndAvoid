#pragma once
#include "Font.h"
#include <string>
#include "PrimitiveTypes.h"
#include "include.h"

class Window;
struct SDL_Texture;
struct SDL_Color;
struct SDL_FRect; 


class Text
{
private:

	Font* mp_font = nullptr;
	SDL_Texture* mp_texture = nullptr;

	std::string m_text;

	SDL_Color* mp_color = nullptr;
	SDL_FRect* mp_rect = nullptr;

	bool m_needToChange = false;
	int32 m_fontSize = 24;
public:

	SDL_Texture* CreateTexture(Window* window);
	SDL_FRect* GetSDLRect() { return mp_rect; };

	Text(Font* font, const std::string& text, Vector2f pos, int32 fontSize, byte r = 255, byte g = 255, byte b = 255, byte a = 255);

	~Text();

	void SetColor(byte r, byte g, byte b, byte a = 255);
	void SetFont(const std::string& id);
	void SetText(const std::string& text);

	void SetPosition(int x, int y);


	Font* GetFont();
	void SetFontSize(int32 size);
	int32 GetFontSize();

	Vector2f GetSizes();
};