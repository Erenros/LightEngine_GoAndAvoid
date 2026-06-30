#pragma once
#include <string>

class Window;
struct Asset;
struct SDL_Texture;

class Texture
{
protected:

	SDL_Texture* mp_texture = nullptr;

	bool m_isSprite = false;

public:

	std::string id = "";
	bool IsSprite() { return m_isSprite; };

	bool IsTextureInit() { return mp_texture == nullptr ? false : true; }

	//Temporaire
	SDL_Texture* GetSDLTexture() 
	{ 
		return mp_texture; 
	};

	Texture(Window* window, const std::string& path);
	Texture(Window* window, Asset* data);
	Texture() = default;

	virtual ~Texture() ;

	void InitTextureWithBuffer(Window* window, Asset* asset);
	void InitTexture(Window* window, const std::string& path);
};