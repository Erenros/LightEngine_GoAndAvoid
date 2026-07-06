#include "Surface.h"
#include <SDL.h>
#include <SDL_image.h>
#include <include.h>
#include "Window.h"
#include "Engine/AssetEngine.h"

std::string Surface::GetID()
{
	return m_id;
}

SDL_Surface* Surface::GetSDL_Surface()
{
	return mp_surface;
}

bool Surface::IsSurfaceInit()
{
	return mp_surface == nullptr ? false : true;
}

Surface::Surface(Window* window, Asset* asset)
{
	InitSurfaceWithBuffer(window, asset);
}

Surface::Surface(Window* window, const std::string& path)
{
	InitSurface(window, path);
}

Surface::~Surface()
{
	SDL_FreeSurface(mp_surface);
}

void Surface::InitSurfaceWithBuffer(Window* window, Asset* asset)
{
	SDL_RWops* rw = SDL_RWFromMem(asset->data.data(), static_cast<int>(asset->data.size()));
	if (rw == NULL)
	{
		DEBUG_WARN << "Can't read data to create surface" << ENDL;
		return;
	}

	SDL_Surface* surface = IMG_Load_RW(rw, 1);
	if (surface == NULL)
	{
		DEBUG_WARN << "Can't Create Surface" << ENDL;
		return;
	}

	SDL_FreeRW(rw);

	asset->width = surface->w;
	asset->height = surface->h;

	mp_surface = surface;

	m_id = asset->name;
}

void Surface::InitSurface(Window* window, const std::string& path)
{
	SDL_Surface* surface = IMG_Load(path.c_str());
	if (surface == NULL)
		DEBUG_WARN << "Couldn't initialize surface for texture with path : " + path << ENDL;

	std::filesystem::path path2(path);
	m_id = path2.stem().string();
}
