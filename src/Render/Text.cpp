#include "Text.h"
#include "Utils.h"
#include "Window.h"
#include "Engine/RessourceManager.h"

#include <SDL3_image/SDL_Image.h>
#include <SDL3/SDL.h>

SDL_Texture* Text::GetTexture(Window* window)
{
	if (mp_texture != nullptr && !m_needToChange)
		return mp_texture;

	m_needToChange = false;
	SDL_DestroyTexture(mp_texture);

	if (mp_font == nullptr || !mp_font->IsFontInit())
	{
		GCLE_WARN << "Font is nullptr can't create texture for text" << ENDL;
		return nullptr;
	}


	//SDL_Surface* surface = TTF_RenderText_Blended(mp_font->GetSDLFont(), m_text.c_str(), *mp_color);
	int32 width = 0;
	int32 height = 0;

	mp_font->GetTextSize(m_text, width, height);

	float32 factor = static_cast<float32>(m_fontSize) / static_cast<float32>(mp_font->GetFontSize());

	mp_rect->w = width * factor;
	mp_rect->h = height * factor;

	SDL_Surface* textSurface = SDL_CreateSurface(width * factor, height * factor, SDL_PIXELFORMAT_RGBA32);
	SDL_SetSurfaceBlendMode(textSurface, SDL_BLENDMODE_ADD);
	
	int x = 0;
	for (auto& charactere : m_text) {
		GlyphInfo& info = mp_font->GetGlypInfo(charactere);
		SDL_Rect SrcRect = { info.x, info.y, info.advanceX, info.height };
		SDL_Rect DistRect = {x * factor, 0, info.advanceX * factor, info.height * factor};

		SDL_SetSurfaceColorMod(mp_font->GetFontSurface(), mp_color->r, mp_color->g, mp_color->a);
		SDL_SetSurfaceAlphaMod(mp_font->GetFontSurface(), mp_color->a);

		SDL_BlitSurfaceScaled(mp_font->GetFontSurface(), &SrcRect, textSurface, &DistRect, SDL_SCALEMODE_LINEAR);
		x += info.advanceX;
	}
	SDL_SetSurfaceBlendMode(textSurface, SDL_BLENDMODE_BLEND);

	/*if (mp_color->r != 255 || mp_color->g != 255 || mp_color->b != 255 || mp_color->a != 255) {
		SDL_Surface* colorSurf = SDL_CreateSurface(static_cast<int32>(width * factor), static_cast<int32>(height * factor), SDL_PIXELFORMAT_RGBA32);
		const SDL_PixelFormatDetails* details = SDL_GetPixelFormatDetails(colorSurf->format);
		uint32 color = SDL_MapRGBA(details,NULL, mp_color->r, mp_color->g, mp_color->b, mp_color->a);
		SDL_FillSurfaceRect(colorSurf, NULL, color);

		SDL_BlitSurface(colorSurf, NULL, textSurface, NULL);

		SDL_DestroySurface(colorSurf);
	}*/
	mp_texture = SDL_CreateTextureFromSurface(window->GetRenderer(), textSurface);

	SDL_DestroySurface(textSurface);

	return mp_texture;
}

Text::Text(Font* font, const std::string& text, Vector2f pos, int32 fontSize, byte r, byte g, byte b, byte a):
	mp_font(font),
	m_text(text),
	m_fontSize(fontSize)
{
	mp_color = GCLE_NEW SDL_Color(r, g, b, a);

	int32 width = 0;
	int32 height = 0;

	mp_font->GetTextSize(m_text, width, height);

	float32 factor = static_cast<float32>(m_fontSize) / static_cast<float32>(mp_font->GetFontSize());
	mp_rect = GCLE_NEW SDL_FRect(pos.x, pos.y,width * factor, height * factor);
}

Text::~Text()
{
	if (mp_texture != nullptr)
		SDL_DestroyTexture(mp_texture);

	delete mp_color;
	delete mp_rect;
}

void Text::SetColor(byte r, byte g, byte b, byte a)
{
	mp_color->r = r;
	mp_color->g = g;
	mp_color->b = b;
	mp_color->a = 255;
	m_needToChange = true;
}

void Text::SetFont(const std::string& id)
{
	Font* font = RessourceManager::GetInstance().GetFont(id);
	if (font == nullptr)
	{
		GCLE_WARN << "Font : " << id << " doesn't exist" << ENDL;
		return;
	}

	mp_font = font;
	m_needToChange = true;
}

void Text::SetText(const std::string& text)
{
	m_text = text;
	m_needToChange = true;
}

void Text::SetPosition(int x, int y)
{
	mp_rect->x = static_cast<float32>(x);
	mp_rect->y = static_cast<float32>(y);
}

void Text::SetFontSize(int32 size) {
	m_fontSize = size;
	m_needToChange = true;
}

int32 Text::GetFontSize() {
	return m_fontSize;
}

Vector2f Text::GetSizes() {
	return { mp_rect->w, mp_rect->h };
}

Font* Text::GetFont() {
	return mp_font;
}