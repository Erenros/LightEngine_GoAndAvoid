#pragma once
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

		float32 m_zoom = 1;

		// Cette fonction doit être lancée sinon la caméra ne marchera pas
		void Init(Window* pWindow);

		void SetFollowing(Entity* newEntity);
		Entity* GetFollowing();

		void SetPosition(Vector2f v);
		Vector2f GetPosition();

		void Update(Timer& time, std::vector<Entity*>& entities);

		void SetZoom(float32 zoom);
		float32 GetZoom();

private:
	Window* mp_Window = nullptr;
	Vector2f screenMiddle;
};

