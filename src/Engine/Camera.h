#pragma once
#include "SDL.h"
#include "Entity.h"
#include "Window.h"
#include "Vector2.hpp"
#include "Transform.h"
#include "Timer.h"

#include <vector>


class Camera
{
	public:
		Transform2D t;

		Entity* followingEntity = nullptr;

		SDL_Renderer* renderer;

		double zoom = 1;

		// Cette fonction doit être lancée sinon la caméra ne marchera pas
		void Init(SDL_Renderer* pRenderer);

		void SetFollowing(Entity* newEntity);
		Entity* GetFollowing();

		void SetPosition(Vector2f v);
		Vector2f GetPosition();

		void Update(Timer& time, std::vector<Entity*>& entities);

		void SetZoom(double d);
		double GetZoom();

private:
	Vector2f screenMiddle;
};

