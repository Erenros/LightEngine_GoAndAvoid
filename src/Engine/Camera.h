#pragma once
#include "SDL.h"
#include "Entity.h"
#include "Window.h"
#include "Vector2.hpp"


class Camera
{
	public:
		Vector2f v = { 0.f, 0.f };

		Entity* followingEntity = nullptr;

		SDL_Renderer* renderer;

		double zoom = 1;


		void InitRenderer(SDL_Renderer* pRenderer);

		void SetFollowing(Entity* newEntity);
		Entity* GetFollowing();

		void Update();

		void SetZoom(double d);
		double GetZoom();
};

