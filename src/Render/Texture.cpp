#include "Texture.h"
#include "Utils.h"
#include "Window.h"
#include "Engine/AssetEngine.h"
#include <SDL.h>
#include <SDL_image.h>
#include <filesystem>

Texture::Texture(Window* window, const std::string& path)
{
	InitTexture(window, path);
}

Texture::Texture(Window* window, Asset* asset)
{
	InitTextureWithBuffer(window, asset);
}

Texture::~Texture() { SDL_DestroyTexture(mp_texture); }

void Texture::InitTextureWithBuffer(Window* window, Asset* asset)
{
	SDL_RWops* rw = SDL_RWFromMem(asset->data.data(), static_cast<int>(asset->data.size()));
	if (rw == NULL)
	{
		DEBUG_WARN << "Can't read data to create texture" << ENDL;
		return;
	}

	SDL_Surface* surface = IMG_Load_RW(rw, 1);
	if (surface == NULL)
	{
		DEBUG_WARN << "Can't Create Surface" << ENDL;
		return;
	}

	SDL_Texture* texture = SDL_CreateTextureFromSurface(window->GetRenderer(), surface);
	if (texture == NULL)
		DEBUG_WARN << "Error during creation of the texture" << ENDL;

	SDL_FreeSurface(surface);
	mp_texture = texture;

	id = asset->name;
}

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
	
	std::filesystem::path path2(path);
	id = path2.stem().string();
}
