#pragma once
#include <string>

class Window;
class Surface;
struct Asset;
struct SDL_Texture;
struct SDL_Surface;

class Texture
{
protected:

	SDL_Texture* mp_texture = nullptr;
	Surface* mp_surface;

	bool m_isSprite = false;
	bool m_textureChanged = false;

	bool m_toDestroy = false;

public:

	std::string id = "";

	SDL_Texture* CreateTexture(Window* window);

	bool IsSprite() { return m_isSprite; };

	bool IsTextureInit() { return mp_texture == nullptr ? false : true; }

	bool ToDestroy();
	void Destroy();

	SDL_Texture* GetSDLTexture();

	Texture(Window* window, Surface* surface);
	Texture() = default;

	virtual ~Texture() ;

	void InitTextureWithSurface(Window* window, Surface* surface);



	//Maybe not really usefull now
	Texture(Window* window, const std::string& path);
	//Maybe not really usefull now
	Texture(Window* window, Asset* data);
	void InitTextureWithBuffer(Window* window, Asset* asset);
	void InitTexture(Window* window, const std::string& path);
};