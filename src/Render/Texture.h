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
	std::string m_Id = "";

public:

	bool IsSprite();

	bool IsTextureInit();

	//Temporaire
	SDL_Texture* GetSDLTexture();

	Texture(Window* pWindow, const std::string& path);
	Texture(Window* pWindow, Asset* pData);
	Texture() = default;

	virtual ~Texture() ;

	void InitTextureWithBuffer(Window* pWindow, Asset* pAsset);
	void InitTexture(Window* pWindow, const std::string& path);
	std::string& GetId();
};