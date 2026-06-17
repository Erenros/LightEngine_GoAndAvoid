#pragma once
#include "SDL.h"
#include "Entity.h"
#include "Window.h"


class Camera
{
	public:
		double x;
		double y;

		Entity* followingEntity;

		SDL_Renderer* renderer;

		double zoom = 1;


		void InitRenderer(SDL_Renderer* pRenderer);

		void SetFollowing(Entity* newEntity);
		Entity* GetFollowing();

		void Update();

		void SetZoom(double d);
		double GetZoom();
};

