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
	const SDL_Rect box(followingEntity->GetTransform().GetPosition().x, followingEntity->GetTransform().GetPosition().y, 50 * zoom, 50 * zoom);

	SDL_RenderSetViewport(renderer, &box);
}

void Camera::SetZoom(double d)
{
	zoom = d;
}

double Camera::GetZoom()
{
	return zoom;
}
