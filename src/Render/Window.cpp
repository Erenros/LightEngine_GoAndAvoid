#include "Window.h" 
#include <iostream>
#include <vector>

#include <SDL3/SDL.h>
#include <SDL3_mixer/SDL_mixer.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3_image/SDL_image.h>

#include "Shape.h"
#include "Texture.h" 
#include "Render/Text.h"
#include "Engine/Entity.h"
#include "Engine/GameManager.h"

constexpr float32 RENDER_TARGET_WIDTH = 1920.f;
constexpr float32 RENDER_TARGET_HEIGHT = 1080.f;

void Window::Create(const char* pName, int32 width, int32 height, uint32 windowFlags, uint32 rendererFlags, int32 x, int32 y)
{
	m_width = width;
	m_height = height;

	if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_AUDIO))
	{
		std::cout << "SDL_Init_Error :  " << SDL_GetError() << std::endl;
		return;
	}
	 
	SDL_WindowFlags sdlWindowFlags = 0;
	if (windowFlags & SDL_WINDOW_FLAGS::WINDOW_RESIZABLE)
		sdlWindowFlags |= SDL_WINDOW_RESIZABLE; 

	mp_Window = SDL_CreateWindow(pName, width, height, sdlWindowFlags);
	if (mp_Window == nullptr) {
		std::cerr << "Window failed to create : " << SDL_GetError() << std::endl;
		return;
	}

	SDL_SetWindowPosition(mp_Window, x, y);

	mp_Renderer = SDL_CreateRenderer(mp_Window, nullptr);
	if (mp_Renderer == nullptr) {
		std::cerr << "Renderer failed to create : " << SDL_GetError() << std::endl;
		return;
	}

	if (rendererFlags & SDL_RENDERER_FLAGS::RENDERER_PRESENTVSYNC)
		SDL_SetRenderVSync(mp_Renderer, 1);

	if (!Audio::Init())
	{
		std::cout << "[Initialisation] : Audio Error : " << SDL_GetError() << std::endl;
		return;
	}

	if (!TTF_Init())
	{
		std::cout << "[Initialisation] : Font Error" << std::endl;
		return;
	}

	// Create a Render Target

	mp_RenderTarget = SDL_CreateTexture(mp_Renderer, SDL_PIXELFORMAT_BGRA8888, SDL_TEXTUREACCESS_TARGET, static_cast<int32>(RENDER_TARGET_WIDTH), static_cast<int32>(RENDER_TARGET_HEIGHT));
	SDL_SetTextureScaleMode(mp_RenderTarget, SDL_SCALEMODE_LINEAR);

	if (!mp_RenderTarget)
	{
		std::cout << "CreateTexture failed: " << SDL_GetError() << '\n';
	}

	if (!SDL_SetRenderTarget(mp_Renderer, mp_RenderTarget))
	{
		std::cout << "SetRenderTarget failed: " << SDL_GetError() << '\n';
	}

	SDL_SetRenderDrawColor(mp_Renderer, 255, 0, 0, 255);

	SDL_RenderClear(mp_Renderer);

	SDL_SetRenderTarget(mp_Renderer, NULL);

	mp_dst = new SDL_FRect();
}

Vector2f Window::GetMousePositionOnRenderTarget()
{
	Vector2u mousePos = GetMousePosition();

	float32 scaleX = RENDER_TARGET_WIDTH / mp_dst->w;
	float32 scaleY = RENDER_TARGET_HEIGHT / mp_dst->h;

	float32 x = (static_cast<float32>(mousePos.x) - mp_dst->x) * scaleX;
	float32 y = (static_cast<float32>(mousePos.y) - mp_dst->y) * scaleY;

	return Vector2f{ x, y };
}

void Window::ClearWindowWithColor(uint8 r, uint8 g, uint8 b, uint8 a)
{
	SDL_SetRenderDrawColor(mp_Renderer, r, g, b, a);
}

void Window::End()
{
	SDL_DestroyTexture(mp_RenderTarget);
	SDL_DestroyRenderer(mp_Renderer);
	SDL_DestroyWindow(mp_Window);

	Audio::Shutdown();
	TTF_Quit();
	SDL_Quit();

	delete mp_dst;
	mp_dst = nullptr;
}

void Window::Present()
{
	SDL_SetRenderTarget(mp_Renderer, nullptr);

	SDL_SetRenderDrawColor(mp_Renderer, 20, 20, 20, 255);
	SDL_RenderClear(mp_Renderer);

	int windowW, windowH;
	SDL_GetWindowSize(mp_Window, &windowW, &windowH);

	constexpr float32 aspect = 1920.f / 1080.f;

	if (static_cast<float32>(windowW) / windowH > aspect)
	{
		mp_dst->h = static_cast<float32>(windowH);
		mp_dst->w = windowH * aspect;
		mp_dst->x = (windowW - mp_dst->w) * 0.5f;
		mp_dst->y = 0.f;
	}
	else
	{
		mp_dst->w = static_cast<float32>(windowW);
		mp_dst->h = windowW / aspect;
		mp_dst->x = 0.f;
		mp_dst->y = (windowH - mp_dst->h) * 0.5f;
	}

	SDL_RenderTexture(mp_Renderer, mp_RenderTarget, nullptr, mp_dst);

	SDL_RenderPresent(mp_Renderer);
}

void Window::Clear()
{
	SDL_SetRenderTarget(mp_Renderer, mp_RenderTarget);

	SDL_SetRenderDrawColor(mp_Renderer, 30, 30, 30, 255);

	SDL_RenderClear(mp_Renderer);
}

void Window::DrawTextOnRenderer(Text* text)
{
	SDL_Texture* texture = text->GetTexture(this);
	if (texture == nullptr)
		return;

	DrawOnRenderer(texture, nullptr, text->GetSDLRect());
}

void Window::DrawOnRenderer(SDL_Texture* pTexture, SDL_FRect* srcrect, SDL_FRect* dstrect) {
	SDL_RenderTexture(mp_Renderer, pTexture, srcrect, dstrect);
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

bool Window::IsInsideWindow(Entity* entity) {
	if (entity->GetRenderShape() == nullptr)
		return false;
	Vector2f camPos = SceneManager::GetInstance().GetCurrentScene()->GetCurrentCamera()->GetPosition();
	float32 margin = 50.f;

	AABB entityAABB;
	if (static_cast<int32>(entity->GetRenderShape()->GetRotation()) % 180 != 0) {
		entityAABB = GetRotatedAABB(entity->GetRenderPosition(), { entity->GetRenderShape()->GetWidth(), entity->GetRenderShape()->GetHeight() }, entity->GetRenderShape()->GetRotation() * DEG_TO_RAD);
		entityAABB = { entityAABB.minX - margin, entityAABB.minY - margin, entityAABB.maxX + margin, entityAABB.maxY + margin };
	}
	else
		entityAABB = { entity->GetRenderShape()->GetPosition(0.f, 0.f).x - margin , entity->GetRenderShape()->GetPosition(0.f, 0.f).y - margin , entity->GetRenderShape()->GetPosition(1.f, 1.f).x + margin, entity->GetRenderShape()->GetPosition(1.f, 1.f).y + margin };

	AABB windowAABB = { -margin, -margin, margin + RENDER_TARGET_WIDTH , margin + RENDER_TARGET_HEIGHT };


	return windowAABB.overlaps(entityAABB);
}

void Window::DrawDebug(gcle::Shape* pShape, Vector2f offset)
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
		points.push_back(SDL_FPoint{ p->x + offset.x, p->y + offset.y });
	}

	SDL_RenderLines(mp_Renderer, points.data(), static_cast<int32>(points.size()));
}

Vector2u Window::GetMousePosition()
{
	float32 x = 0.f, y = 0.f;
	SDL_GetMouseState(&x, &y);
	return Vector2u{ static_cast<int32>(x), static_cast<int32>(y) };
}

Vector2f Window::GetWindowSize()
{
	Vector2u size;
	SDL_GetWindowSize(mp_Window, &size.x, &size.y);
	return Vector2f{ static_cast<float32>(size.x), static_cast<float32>(size.y) };
}