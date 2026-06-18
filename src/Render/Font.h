#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include <string>

class Font
{
private:

	TTF_Font* mp_font = nullptr;

public:
	
	bool IsFontInit() { return mp_font == nullptr ? false : true; };

	Font(const std::string& path, int size = 25);
	Font() = default;
	~Font() { TTF_CloseFont(mp_font); };

	void InitFont(const std::string& path, int size = 25);

	void SetFontSize(int size);
};