#pragma once
#include <string>

struct TTF_Font;

class Font
{
private:

	TTF_Font* mp_font = nullptr;

public:
	
	TTF_Font* GetSDLFont() { return mp_font; };

	bool IsFontInit() { return mp_font == nullptr ? false : true; };

	Font(const std::string& path, int size = 25);
	Font() = default;
	~Font();

	void InitFont(const std::string& path, int size = 25);

	void SetFontSize(int size);
};