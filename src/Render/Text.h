#pragma once
#include "Font.h"
#include <string>
#include "PrimitiveTypes.h"

class Window;
struct SDL_Texture;
struct SDL_Color;
struct SDL_Rect;


class Text
{
private:

	Font* mp_font = nullptr;
	SDL_Texture* mp_texture = nullptr;

	std::string m_text;

	SDL_Color* mp_color = nullptr;
	SDL_Rect* mp_rect = nullptr;
	
	bool m_needToChange = false;

public:

	SDL_Texture* CreateTexture(Window* window);
	SDL_Rect* GetSDLRect() { return mp_rect; };

	Text(Font* font,const std::string& text, int x, int y, int w, int h, byte r = 255, byte g = 255, byte b = 255, byte a = 255);
	
	~Text();

	void SetColor(byte r = 255,byte g = 255, byte b = 255, byte a = 255);
	void SetFont(const std::string& id);
	void SetText(const std::string& text);

	void SetPosition(int x, int y);
	void SetWidth(int w)		  ;
	void SetHeight(int h);
};