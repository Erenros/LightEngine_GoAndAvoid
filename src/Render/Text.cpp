#include "Text.h"
#include "Utils.h"
#include "Window.h"
#include "Engine/RessourceManager.h"

#include <SDL3_image/SDL_Image.h>
#include <SDL3/SDL.h>

SDL_Texture* Text::GetTexture(Window* pWindow)
{
	if (mp_Texture != nullptr && !m_NeedToChange)
		return mp_Texture;

	m_NeedToChange = false;
	SDL_DestroyTexture(mp_Texture);

	if (mp_Font == nullptr || !mp_Font->IsFontInit())
	{
		GCLE_WARN << "Font is nullptr can't create texture for text" << ENDL;
		return nullptr;
	}


	//SDL_Surface* surface = TTF_RenderText_Blended(mp_font->GetSDLFont(), m_Text.c_str(), *mp_color);
	int32 width = 0;
	int32 height = 0;

	mp_Font->GetTextSize(m_Text, width, height);

	float32 factor = static_cast<float32>(m_FontSize) / static_cast<float32>(mp_Font->GetFontSize());

	mp_Rect->w = width * factor;
	mp_Rect->h = height * factor;

	SDL_Surface* textSurface = SDL_CreateSurface(static_cast<int32>(width * factor), static_cast<int32>(height * factor), SDL_PIXELFORMAT_RGBA32);
	SDL_SetSurfaceBlendMode(textSurface, SDL_BLENDMODE_ADD);
	
	int32 x = 0;
	for (auto& charactere : m_Text) {
		GlyphInfo& info = mp_Font->GetGlypInfo(charactere);
		SDL_Rect SrcRect = { info.x, info.y, info.advanceX, info.height };
		SDL_Rect DistRect = {static_cast<int32>(x * factor), 0, static_cast<int32>(info.advanceX * factor), static_cast<int32>(info.height * factor)};

		SDL_SetSurfaceColorMod(mp_Font->GetFontSurface(), mp_Color->r, mp_Color->g, mp_Color->b);
		SDL_SetSurfaceAlphaMod(mp_Font->GetFontSurface(), mp_Color->a);

		SDL_BlitSurfaceScaled(mp_Font->GetFontSurface(), &SrcRect, textSurface, &DistRect, SDL_SCALEMODE_LINEAR);
		x += info.advanceX;
	}
	SDL_SetSurfaceBlendMode(textSurface, SDL_BLENDMODE_BLEND);

	mp_Texture = SDL_CreateTextureFromSurface(pWindow->GetRenderer(), textSurface);
	SDL_SetTextureScaleMode(mp_Texture, SDL_SCALEMODE_NEAREST);

	SDL_DestroySurface(textSurface);

	return mp_Texture;
}

SDL_FRect* Text::GetSDLRect()
{
	return mp_Rect;
}

Text::Text(Font* pFont, const std::string& text, Vector2f pos, int32 fontSize, byte r, byte g, byte b, byte a):
	mp_Font(pFont),
	m_Text(text),
	m_FontSize(fontSize)
{
	mp_Color = GCLE_NEW SDL_Color(r, g, b, a);

	int32 width = 0;
	int32 height = 0;

	mp_Font->GetTextSize(m_Text, width, height);

	float32 factor = static_cast<float32>(m_FontSize) / static_cast<float32>(mp_Font->GetFontSize());
	mp_Rect = GCLE_NEW SDL_FRect(pos.x, pos.y,width * factor, height * factor);
}

Text::~Text()
{
	if (mp_Texture != nullptr)
		SDL_DestroyTexture(mp_Texture);

	delete mp_Color;
	delete mp_Rect;
}

void Text::SetColor(byte r, byte g, byte b, byte a)
{
	mp_Color->r = r;
	mp_Color->g = g;
	mp_Color->b = b;
	mp_Color->a = a;
	m_NeedToChange = true;
}

void Text::SetFont(const std::string& id)
{
	Font* font = RessourceManager::GetInstance().GetFont(id);
	if (font == nullptr)
	{
		GCLE_WARN << "Font : " << id << " doesn't exist" << ENDL;
		return;
	}

	mp_Font = font;
	m_NeedToChange = true;
}

void Text::SetText(const std::string& text)
{
	m_Text = text;
	m_NeedToChange = true;
}

void Text::SetPosition(int32 x, int32 y)
{
	mp_Rect->x = static_cast<float32>(x);
	mp_Rect->y = static_cast<float32>(y);
}

void Text::SetFontSize(int32 size) {
	m_FontSize = size;
	
	mp_Rect->h = size;
	mp_Rect->w = static_cast<float32>(m_Text.size()) * static_cast<float32>(size) * 0.6f;

	m_NeedToChange = true;
}

int32 Text::GetFontSize() {
	return m_FontSize;
}

Vector2f Text::GetSizes() {
	return { mp_Rect->w, mp_Rect->h };
}

Font* Text::GetFont() {
	return mp_Font;
}