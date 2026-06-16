#pragma once

#include <SDL.h>

class Window {
	SDL_Window* mp_Window = nullptr;
	SDL_Renderer* mp_Renderer = nullptr;


public:

	//Constructors
	Window() = default;
	Window(const char* pName, int width, int height, Uint32 windowFlags, Uint32 rendererFlags, int x, int y) {
		Create(pName, width, height, windowFlags, rendererFlags, x, y);
	};


	//Destructors
	~Window() {
		End();
	};



	//Create a Window
	void Create(const char* pName, int width, int height, Uint32 windowFlags = SDL_WINDOW_SHOWN, Uint32 rendererFlags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC, int x = SDL_WINDOWPOS_UNDEFINED, int y = SDL_WINDOWPOS_UNDEFINED);
	
	
	//Destroy a window
	void End();

	//Getter
	SDL_Window* GetWindow() { return mp_Window; }
	SDL_Renderer* GetRenderer() { return mp_Renderer; }

	//Draw

	//TODO changer SDL_Rect pour le vect2 custom
	void DrawOnRenderer(SDL_Texture* texture, SDL_Rect* srcrect, SDL_Rect* dstrect);

};