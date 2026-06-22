#pragma once

#include <string>

class Window;
struct SDL_Texture;

class Texture
{
private:

	SDL_Texture* mp_texture = nullptr;

public:

	bool IsTextureInit() { return mp_texture == nullptr ? false : true; }

	//Temporaire
	SDL_Texture* GetSDLTexture() 
	{ 
		return mp_texture; 
	};

	Texture(Window* window, const std::string& path);
	Texture() = default;

	~Texture() ;

	void InitTexture(Window* window, const std::string& path);
};