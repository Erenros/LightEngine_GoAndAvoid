#include "Font.h"
#include "Utils.h"

Font::Font(const std::string& path, int size)
{
	InitFont(path, size);
}

void Font::InitFont(const std::string& path, int size)
{
    TTF_Font* font = TTF_OpenFont(path.c_str(), size);
    if (font == NULL)
    {
        DEBUG_WARN << "Couldn't initialize font with path" + path << ENDL;
        return;
    }
    
    mp_font = font;
    return;
}

void Font::SetFontSize(int size)
{
    TTF_SetFontSize(mp_font, size);
}
