#include "Texture.h"
#include "Utils.h"
#include "Window.h"
#include "Surface.h"
#include "Engine/AssetEngine.h"
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <filesystem>

SDL_Texture* Texture::CreateTexture(Window* window)
{
	if (mp_texture != nullptr && !m_textureChanged)
		return mp_texture;

	if (mp_surface == nullptr || !mp_surface->IsSurfaceInit())
	{
		DEBUG_WARN << "Surface is null" << ENDL;
		return mp_texture;
	}

	SDL_DestroyTexture(mp_texture);

	SDL_Texture* texture = SDL_CreateTextureFromSurface(window->GetRenderer(), mp_surface->GetSDL_Surface());
	if (texture == NULL)
	{
		DEBUG_WARN << "Error during creation of the texture" << ENDL;
		return nullptr;
	}

	m_textureChanged = false;
	mp_texture = texture;
	return mp_texture;
}

bool Texture::ToDestroy()
{
	return m_toDestroy;
}

void Texture::Destroy()
{
	m_toDestroy = true;
}

SDL_Texture* Texture::GetSDLTexture()
{
	return mp_texture;
};

Texture::Texture(Window* window, const std::string& path)
{
	InitTexture(window, path);
}

Texture::Texture(Window* window, Asset* asset)
{
	InitTextureWithBuffer(window, asset);
}

Texture::Texture(Window* window, Surface* surface)
{
	InitTextureWithSurface(window, surface);
}

Texture::~Texture() 
{
	SDL_DestroyTexture(mp_texture); 
}

void Texture::InitTextureWithSurface(Window* window, Surface* surface)
{
	if (surface == nullptr || !surface->IsSurfaceInit())
	{
		DEBUG_WARN << "Surface is null" << ENDL;
		return;
	}

	mp_surface = surface;

	SDL_Texture* texture = SDL_CreateTextureFromSurface(window->GetRenderer(), surface->GetSDL_Surface());
	if (texture == NULL)
		DEBUG_WARN << "Error during creation of the texture" << ENDL;

	mp_texture = texture;

	id = mp_surface->GetID();
}

void Texture::InitTextureWithBuffer(Window* window, Asset* asset)
{
	SDL_IOStream* io = SDL_IOFromConstMem(asset->data.data(), asset->data.size());
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

	asset->width = surface->w;
	asset->height = surface->h;

	SDL_Texture* texture = SDL_CreateTextureFromSurface(window->GetRenderer(), surface);
	if (texture == NULL)
		GCLE_WARN << "Error during creation of the texture" << ENDL;

	//SDL_FreeSurface(surface);
	mp_texture = texture;

	id = asset->name;
}

void Texture::InitTexture(Window* window, const std::string& path)
{
	SDL_Surface* surface = IMG_Load(path.c_str());
	if (surface == NULL)
		GCLE_WARN << "Couldn't initialize surface for texture with path : " + path << ENDL;

	SDL_Texture* texture = SDL_CreateTextureFromSurface(window->GetRenderer(), surface);
	if (texture == NULL)
		GCLE_WARN << "Error during creation of the texture with path : " + path << ENDL;


	//SDL_FreeSurface(surface);

	mp_texture = texture;

	std::filesystem::path path2(path);
	id = path2.stem().string();
}