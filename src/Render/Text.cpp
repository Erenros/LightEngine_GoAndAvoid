#include "Text.h"
#include "Utils.h"
#include "Window.h"


SDL_Texture* Text::CreateTexture(Window* window)
{
	if (mp_texture != nullptr)
		return mp_texture;

	if (mp_font == nullptr || !mp_font->IsFontInit())
	{
		DEBUG_WARN << "Font is nullptr can't create texture for text" << ENDL;
		return nullptr;
	}

	SDL_Surface* surface = TTF_RenderText_Solid(mp_font->GetSDLFont(), m_text.c_str(), m_color);
	SDL_Texture* text_texture = SDL_CreateTextureFromSurface(window->GetRenderer(), surface);

	mp_texture = text_texture;

	SDL_FreeSurface(surface);

	return text_texture;
}