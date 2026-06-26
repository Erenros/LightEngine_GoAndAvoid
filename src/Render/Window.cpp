#include "Window.h"
#include <iostream>
#include <vector>
#include "SDL.h"
#include "SDL_mixer.h"
#include "SDL_ttf.h"
#include "SDL_video.h"
#include "SDL_image.h"
#include "Render/Text.h"
#include "Texture.h"
#include "Engine/Entity.h"
#include "Shape.h"
#include "Engine/GameManager.h"

void Window::Create(const char* pName,int32 width, int32 height, uint32 windowFlags, uint32 rendererFlags, int32 x, int32 y)
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

void Window::ClearWindowWithColor(uint8 r, uint8 g, uint8 b, uint8 a)
{
	SDL_SetRenderDrawColor(mp_Renderer, r, g, b, a); 
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

void Window::DrawTextOnRenderer(Text* text)
{
	SDL_Texture* texture = text->CreateTexture(this);
	if (texture == nullptr)
		return;

	DrawOnRenderer(texture, nullptr,text->GetSDLRect());
}

void Window::DrawOnRenderer(SDL_Texture* pTexture, SDL_Rect* srcrect, SDL_Rect* dstrect){
	SDL_RenderCopy(mp_Renderer, pTexture, srcrect, dstrect);
}

void Window::Draw(gcle::Shape* pShape)
{
	const std::vector<SDL_Vertex*>& verticesPtr = pShape->GetVerticies();

	std::vector<SDL_Vertex> vertices;
	vertices.reserve(verticesPtr.size());
	for (SDL_Vertex* v : verticesPtr)
	{
		vertices.push_back(*v);
	}


	


	if (pShape->GetTexture() == nullptr)
		SDL_RenderGeometry(mp_Renderer, nullptr, vertices.data(), static_cast<int32>(vertices.size()), pShape->GetIndicies().data(), static_cast<int32>(pShape->GetIndicies().size()));
	else {
		Texture* text = pShape->GetTexture();
		SDL_RenderGeometry(mp_Renderer, pShape->GetTexture()->GetSDLTexture(), vertices.data(), static_cast<int32>(vertices.size()), pShape->GetIndicies().data(), static_cast<int32>(pShape->GetIndicies().size()));
	}
}

bool Window::IsInsideWindow(Entity* entity){
	int32 w;
	int32 h;
	SDL_GetWindowSize(mp_Window, &w, &h);
	Vector2f camCenter = GameManager::GetInstance().m_Cam.GetPosition();
	float32 margin = 100.f;

	Vector2f camPos1 = { camCenter.x - margin , camCenter.y - margin };
	Vector2f camPos2 = { camCenter.x + w + margin, camCenter.y + h + margin};


	gcle::Shape* shape = entity->GetRenderShape();

	Vector2f pos1 = shape->GetPosition(0.f, 0.f);
	Vector2f pos2 = shape->GetPosition(1.f, 1.f);

	return pos1.x < camPos2.x && pos1.y < camPos2.y && pos2.x > camPos1.x && pos2.y > camPos1.y;
}
void Window::DrawDebug(gcle::Shape* pShape)
{
	std::vector<SDL_FPoint*> pointsPtr;

	switch (pShape->GetShape())
	{
	case gcle::Shapes::Rectangle:
	{
		auto pSh = static_cast<gcle::Rectangle*>(pShape);
		pointsPtr = pSh->GetHollow();
		break;
	} 
	case gcle::Shapes::Circle:
	{
		pointsPtr = (static_cast<gcle::Circle*>(pShape))->GetHollow();
		break;
	} 
	case gcle::Shapes::Triangle:
	{
		pointsPtr = (static_cast<gcle::Triangle*>(pShape))->GetHollow();
		break;
	}

	default:
		break;
	}


	std::vector<SDL_FPoint> points;
	points.reserve(pointsPtr.size());
	for (SDL_FPoint* p : pointsPtr)
	{
		points.push_back(*p);
	}

	SDL_RenderDrawLinesF(mp_Renderer, points.data(), static_cast<int32>(points.size()));
}
