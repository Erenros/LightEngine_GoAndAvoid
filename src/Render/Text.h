#pragma once
#include "Font.h"
#include <string>
#include "PrimitiveTypes.h"

class Window;

class Text
{
private:

	Font* mp_font = nullptr;
	SDL_Texture* mp_texture = nullptr;

	std::string m_text;

	SDL_Color* m_color = nullptr;
	SDL_Rect* m_rect = nullptr;

public:

	SDL_Texture* CreateTexture(Window* window);
	SDL_Rect* GetSDLRect() { return m_rect; };

	Text(Font* font, std::string& text, int x, int y, int w, int h, byte r = 255, byte g = 255, byte b = 255);
	
	~Text() { if(mp_texture != nullptr) SDL_DestroyTexture(mp_texture); };

	void SetPosition(int x, int y) { m_rect->x = x, m_rect->y; };
	void SetWidth(int w) { m_rect->w = w; };
	void SetHeight(int h) { m_rect->h = h; };
};