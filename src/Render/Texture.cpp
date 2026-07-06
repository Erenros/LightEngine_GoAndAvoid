#include "Texture.h"
#include "Utils.h"
#include "Window.h"
#include "Engine/AssetEngine.h"
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <filesystem>

bool Texture::IsSprite()
{ 
	return m_isSprite; 
}
bool Texture::IsTextureInit()
{
	return mp_texture == nullptr ? false : true;
}
;

SDL_Texture* Texture::GetSDLTexture()
{
	return mp_texture;
}

Texture::Texture(Window* pWindow, const std::string& path)
{
	InitTexture(pWindow, path);
}

Texture::Texture(Window* pWindow, Asset* pAsset)
{
	InitTextureWithBuffer(pWindow, pAsset);
}

Texture::~Texture() { SDL_DestroyTexture(mp_texture); }

void Texture::InitTextureWithBuffer(Window* pWindow, Asset* pAsset)
{
	SDL_IOStream* io = SDL_IOFromConstMem(pAsset->data.data(), pAsset->data.size());
	if (io == NULL)
	{
		GCLE_WARN << "Can't read data to create texture" << ENDL;
		return;
	}

	SDL_Surface* surface = IMG_Load_IO(io, true); 
	if (surface == NULL)
	{
		GCLE_WARN << "Can't Create Surface" << ENDL;
		return;
	}

	pAsset->width = surface->w;
	pAsset->height = surface->h;

	SDL_Texture* texture = SDL_CreateTextureFromSurface(pWindow->GetRenderer(), surface);
	if (texture == NULL)
		GCLE_WARN << "Error during creation of the texture" << ENDL;

	SDL_DestroySurface(surface);
	mp_texture = texture;

	m_Id = pAsset->name;
}

void Texture::InitTexture(Window* pWindow, const std::string& path)
{
	SDL_Surface* surface = IMG_Load(path.c_str());
	if (surface == NULL)
		GCLE_WARN << "Couldn't initialize surface for texture with path : " + path << ENDL;

	SDL_Texture* texture = SDL_CreateTextureFromSurface(pWindow->GetRenderer(), surface);
	if (texture == NULL)
		GCLE_WARN << "Error during creation of the texture with path : " + path << ENDL;

	SDL_DestroySurface(surface);


	mp_texture = texture;

	std::filesystem::path path2(path);
	m_Id = path2.stem().string();
}

std::string& Texture::GetId()
{
	return m_Id;
}
