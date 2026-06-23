#include "Texture.h"
#include "Utils.h"
#include "Window.h"
#include <SDL.h>
#include <SDL_image.h>


Texture::Texture(Window* window, const std::string& path)
{
    InitTexture(window, path);
}

Texture::~Texture() { SDL_DestroyTexture(mp_texture); }

void Texture::InitTexture(Window* window, const std::string& path)
{
    SDL_Surface* surface = IMG_Load(path.c_str());
    if (surface == NULL)
        DEBUG_WARN << "Couldn't initialize surface for texture with path : " + path << ENDL;

    SDL_Texture* texture = SDL_CreateTextureFromSurface(window->GetRenderer(), surface);
    if (texture == NULL)
        DEBUG_WARN << "Error during creation of the texture with path : " + path << ENDL;

    SDL_FreeSurface(surface);


    mp_texture = texture;
}
