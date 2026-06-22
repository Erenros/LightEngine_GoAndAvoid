#include "Text.h"
#include "Utils.h"
#include "Window.h"

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

	SDL_Surface* surface = TTF_RenderText_Solid(mp_font->GetSDLFont(), m_text.c_str(), *m_color);
	SDL_Texture* text_texture = SDL_CreateTextureFromSurface(window->GetRenderer(), surface);

	mp_texture = text_texture;

	SDL_FreeSurface(surface);

	return text_texture;
}

Text::Text(Font* font, std::string& text, int x, int y, int w, int h, byte r, byte g, byte b) :
	mp_font(font),
	m_text(text)/*,
	m_color({r,g,b}),
	m_rect({x,y,w,h})*/
{
	m_color = new SDL_Color(r, g, b);
	m_rect = new SDL_Rect(x, y, w, h);
}

Text::~Text()
{
	if (mp_texture != nullptr) 
		SDL_DestroyTexture(mp_texture);
}

void Text::SetPosition(int x, int y)
{
	m_rect->x = x;
	m_rect->y = y;
}
void Text::SetWidth(int w)
{
	m_rect->w = w;
}
void Text::SetHeight(int h)
{
	m_rect->h = h;
} 