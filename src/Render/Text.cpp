#include "Text.h"
#include "Utils.h"
#include "Window.h"
#include "Engine/RessourceManager.h"

#include <SDL_ttf.h>


SDL_Texture* Text::CreateTexture(Window* window)
{
	if (mp_texture != nullptr)
		return mp_texture;

	if (mp_font == nullptr || !mp_font->IsFontInit())
	{
		DEBUG_WARN << "Font is nullptr can't create texture for text" << ENDL;
		return nullptr;
	}

	SDL_Surface* surface = TTF_RenderText_Solid(mp_font->GetSDLFont(), m_text.c_str(), *mp_color);
	SDL_Texture* text_texture = SDL_CreateTextureFromSurface(window->GetRenderer(), surface);

	mp_texture = text_texture;

	SDL_FreeSurface(surface);

	return text_texture;
}

Text::Text(Font* font, const std::string& text, int x, int y, int w, int h, byte r, byte g, byte b, byte a) :
	mp_font(font),
	m_text(text)
{
	mp_color = new SDL_Color(r, g, b, a);
	mp_rect = new SDL_Rect(x, y, w, h);
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
	delete mp_color;
	mp_color = new SDL_Color(r, g, b, a);
}

void Text::SetFont(const std::string& id)
{
	Font* font = RessourceManager::GetInstance().GetFont(id);
	if (font == nullptr)
	{
		DEBUG_WARN << "Font : " << id << " doesn't exist" << ENDL;
		return;
	}

	mp_font = font;
}

void Text::SetText(const std::string& text)
{
	m_text = text;
}

void Text::SetPosition(int x, int y)
{
	mp_rect->x = x;
	mp_rect->y = y;
}
void Text::SetWidth(int w)
{
	mp_rect->w = w;
}
void Text::SetHeight(int h)
{
	mp_rect->h = h;
} 