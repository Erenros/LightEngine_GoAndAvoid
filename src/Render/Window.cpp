#include "Window.h"
#include <iostream>
#include "SDL.h"
#include "SDL_mixer.h"
#include "SDL_ttf.h"

void Window::Create(const char* pName,int width, int height, Uint32 windowFlags, Uint32 rendererFlags, int x, int y)
{
	m_width = width;
	m_height = height;

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) != 0)
	{
		std::cout << "SDL_Init_Error :  " << SDL_GetError() << std::endl;
		return;
	}

	if (IMG_Init(IMG_INIT_PNG) == 0) {
		std::cout << "Error SDL2_image Initialization";
		return;
	}


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

	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	{
		std::cout << "[Initialisation] : Audio Error : " << SDL_GetError() << std::endl;
		return;
	}

	if (TTF_Init() != 0)
	{
		std::cout << "[Initialisation] : Font Error" << std::endl;
		return;
	}
}

void Window::End(){
	SDL_DestroyRenderer(mp_Renderer);
	SDL_DestroyWindow(mp_Window);

	Mix_CloseAudio();
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

void Window::Present()
{
	SDL_RenderPresent(mp_Renderer);
}

void Window::Clear()
{
	SDL_RenderClear(mp_Renderer);
}

void Window::DrawOnRenderer(SDL_Texture* pTexture, SDL_Rect* srcrect, SDL_Rect* dstrect){
	SDL_RenderCopy(mp_Renderer, pTexture, srcrect, dstrect);
}

void Window::Draw(gcle::Shape* pShape)
{
	if (pShape->GetTexture() == nullptr)
		SDL_RenderGeometry(mp_Renderer, nullptr, pShape->GetVerticies().data(), pShape->GetVerticies().size(), pShape->GetIndicies().data(), pShape->GetIndicies().size());
	else
		SDL_RenderGeometry(mp_Renderer, pShape->GetTexture()->GetSDLTexture(), pShape->GetVerticies().data(), pShape->GetVerticies().size(), pShape->GetIndicies().data(), pShape->GetIndicies().size());
}

