#include "Text.h"
#include "Utils.h"
#include "Window.h"
#include "Engine/RessourceManager.h"

#include <SDL_ttf.h>


SDL_Texture* Text::CreateTexture(Window* window)
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

	SDL_Surface* surface = TTF_RenderText_Blended(mp_font->GetSDLFont(), m_text.c_str(), *mp_color);
	SDL_Texture* text_texture = SDL_CreateTextureFromSurface(window->GetRenderer(), surface);

	mp_texture = text_texture;

	SDL_FreeSurface(surface);
	
	mp_rect->w = m_text.size() * m_fontSize * 0.6;
	mp_rect->h = m_fontSize;


	return text_texture;
}

Text::Text(Font* font, const std::string& text, Vector2f pos, int32 fontSize, byte r, byte g, byte b, byte a) :
	mp_font(font),
	m_text(text),
	m_fontSize(fontSize)
{
	mp_color = GCLE_NEW SDL_Color(r, g, b, a);
	mp_rect = GCLE_NEW SDL_Rect(pos.x, pos.y,text.size() * fontSize * 0.6, fontSize);
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
	mp_color->a = a;
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
	mp_rect->x = x;
	mp_rect->y = y;
}


void Text::SetFontSize(int32 size) {
	m_fontSize = size;
	m_needToChange = true;
}

int32 Text::GetFontSize() {
	return m_fontSize;
}

Vector2f Text::GetSizes(){
	return { static_cast<float32>(mp_rect->w), static_cast<float32>(mp_rect->h)};
}

Font* Text::GetFont() {
	return mp_font;
}