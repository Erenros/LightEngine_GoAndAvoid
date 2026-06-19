#pragma once

#include <SDL.h> 
#include "Render/Shape.h"

class Window 
{
public:
	 
	Window() = default;
	Window(const char* pName, int width, int height, Uint32 windowFlags, Uint32 rendererFlags, int x, int y) 
	{
		Create(pName, width, height, windowFlags, rendererFlags, x, y);
	};
	 
	~Window() {
		End();
	};


	 
	void Create(const char* pName, 
		int width, 
		int height, 
		Uint32 windowFlags = SDL_WINDOW_SHOWN, 
		Uint32 rendererFlags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC, 
		int x = SDL_WINDOWPOS_UNDEFINED, 
		int y = SDL_WINDOWPOS_UNDEFINED
	);

	void Present();
	void Clear();
	
	 
	void End();
	 
	SDL_Window* GetWindow() { return mp_Window; }
	SDL_Renderer* GetRenderer() { return mp_Renderer; }
	  
	void DrawOnRenderer(SDL_Texture* texture, SDL_Rect* srcrect, SDL_Rect* dstrect); 
	void Draw(gcle::Shape* pShape);

public:
	uint32 GetWidth() { return m_width; }
	uint32 GetHeight() { return m_height; }

private:
	SDL_Window* mp_Window = nullptr;
	SDL_Renderer* mp_Renderer = nullptr;
	uint32 m_width, m_height = 0;
};