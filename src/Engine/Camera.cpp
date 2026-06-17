#include "Camera.h"


void Camera::InitRenderer(SDL_Renderer* pRenderer)
{
	renderer = pRenderer;
}

void Camera::SetFollowing(Entity* newEntity)
{
	followingEntity = newEntity;
}

Entity* Camera::GetFollowing()
{
	return followingEntity;
}

void Camera::Update()
{
	DEBUG_INFO << "Camera update" << ENDL;

	if (followingEntity == nullptr)
	{
		DEBUG_INFO << "Zoom is : " << zoom << ENDL;
		SDL_Rect box(v.x+117, v.y+323, 100 * zoom, 100 * zoom);
	}

	else
	{
		v.x = followingEntity->GetTransform().GetPosition().x;
		v.y = followingEntity->GetTransform().GetPosition().y;

		SDL_Rect box(v.x, v.y, 50 * zoom, 50 * zoom);
	}
}

void Camera::SetZoom(double d)
{
	zoom = d;
}

double Camera::GetZoom()
{
	return zoom;
}
