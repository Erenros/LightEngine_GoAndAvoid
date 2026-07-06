#include "Font.h"
#include "Utils.h"

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

Font::Font(const std::string& path)
{
    InitFont(path);
}

Font::~Font()
{
    TTF_CloseFont(mp_font);
}

void Font::InitFont(const std::string& path)
{
    TTF_Font* font = TTF_OpenFont(path.c_str(), 72);
    if (font == NULL)
    {
        GCLE_WARN << "Couldn't initialize font with path" + path << ENDL;
        return;
    }

    TTF_SetFontSize(font, 72);
    mp_font = font;
    return;
}

void Font::SetFontSize(int size)
{
    TTF_SetFontSize(mp_font, size);
}