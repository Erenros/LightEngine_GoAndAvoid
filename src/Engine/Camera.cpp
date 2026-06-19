#include "Camera.h"


void Camera::Init(SDL_Renderer* pRenderer)
{
	t.SetPosition({ 0.f, 0.f });

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



void Camera::SetPosition(Vector2f v)
{
	t.SetPosition(v);
}

Vector2f Camera::GetPosition()
{
	return t.GetPosition();
}



void Camera::Update()
{
	//DEBUG_INFO << "Camera update" << ENDL;

	t.SetPosition(followingEntity->GetPosition());
	//DEBUG_INFO << t.GetPosition().x << " and " << t.GetPosition().y << ENDL;
}

void Camera::SetZoom(double d)
{
	zoom = d;
}

double Camera::GetZoom()
{
	return zoom;
}
