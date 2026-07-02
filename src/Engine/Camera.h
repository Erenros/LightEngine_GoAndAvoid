#pragma once
#include "Entity.h"
#include "Render/Window.h"
#include "Vector2.hpp"
#include "Transform.h"
#include "Timer.h"

#include <vector>


class Camera
{
	public:
		~Camera();
		void Init(Window* pWindow);

		void SetFollowing(Entity* newEntity);
		Entity* GetFollowing();

		void SetPosition(Vector2f v);
		Vector2f GetPosition();
		 
		void SetZoom(float32 zoom);
		float32 GetZoom();

		Vector2f GetScreenMousePosition();
		Vector2f GetMouseScreenToWorldPosition();

		void SetActive(bool isActive);
		bool IsActive() const;

		uint64 GetId() const;

private:
		void Update(Clock& time, std::vector<std::vector<Entity*>>& entities);

private:
	Window*			mp_Window = nullptr;
	Vector2f		screenMiddle;
	float32			m_zoom = 1;
	Transform2D		transform;
	Entity*			m_followingEntity = nullptr;

	uint64			m_Id = 0;

	bool m_isActive = false;

private: 
	friend class GameManager;
};

