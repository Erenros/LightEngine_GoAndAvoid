#include "Window.h"
#include <iostream>
#include "SDL.h"

void Window::Create(const char* pName,int width, int height, Uint32 windowFlags, Uint32 rendererFlags, int x, int y){
	mp_Window = SDL_CreateWindow(pName, x, y, width, height, windowFlags);
	if (mp_Window == nullptr) {
		std::cerr << "Window failed to create" << std::endl;
		return;
	}

	mp_Renderer = SDL_CreateRenderer(mp_Window, -1, rendererFlags);
	if (mp_Renderer == nullptr) {
		std::cerr << "Renderer failed to create" << std::endl;
		return;
	}
}

void Window::End(){
	SDL_DestroyRenderer(mp_Renderer);
	SDL_DestroyWindow(mp_Window);
}

void Window::DrawOnRenderer(SDL_Texture* pTexture, SDL_Rect* srcrect, SDL_Rect* dstrect){
	SDL_RenderCopy(mp_Renderer, pTexture, srcrect, dstrect);
}
