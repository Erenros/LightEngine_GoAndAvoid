#pragma once
#include <string>
#include "include.h"


struct SDL_Surface;

struct GlyphInfo {
	wchar_t charactere;
	int32 x = 0;
	int32 y = 0;
	int32 width = 0;
	int32 height = 0;
	int32 advanceX = 0;

	void DrawData();
	void DrawCharacter();
};

class Font
{
private:

	SDL_Surface* mp_font = nullptr;

	std::unordered_map<char , GlyphInfo> m_glyphs;

	int32 fontSize = 0;
public:
	
	SDL_Surface* GetFontSurface() { return mp_font; };

	bool IsFontInit() { return mp_font == nullptr ? false : true; };

	Font(const std::string& path);
	Font() = default;
	~Font();

	void InitFont(const std::string& path);

private:
	uint32 ReadBigEndian(const uint8* data);

	bool ReadFromAtlasChunk(const std::string& path);

public:
	GlyphInfo& GetGlypInfo(char& charactere);

	void GetTextSize(const std::string& text, int32& width, int32& height);

	int32 GetFontSize() { return fontSize; }
};
