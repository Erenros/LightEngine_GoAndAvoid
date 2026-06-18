#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <string>

class Window;

class Texture
{
private:

	SDL_Texture* mp_texture = nullptr;

public:

	bool IsTextureInit() { return mp_texture == nullptr ? false : true; }

	//Temporaire
	SDL_Texture* GetSDLTexture() { return mp_texture; };;

	Texture(Window* window, const std::string& path);
	Texture() = default;

	~Texture() { SDL_DestroyTexture(mp_texture); };

	void InitTexture(Window* window, const std::string& path);
};