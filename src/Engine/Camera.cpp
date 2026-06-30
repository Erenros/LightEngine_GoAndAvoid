#include "GameManager.h"
#include "Camera.h"

void Camera::Init(Window* pWindow)
{
	t.SetPosition({ 0.f, 0.f });

	mp_Window = pWindow;

	screenMiddle = { GameManager::GetInstance().GetWindow()->GetWidth() / 2.f, GameManager::GetInstance().GetWindow()->GetHeight() / 2.f };
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



void Camera::Update(Clock& time, std::vector<std::vector<Entity*>>& entities)
{
	if (followingEntity != nullptr)
		t.SetPosition(followingEntity->GetPosition());
	
	for (auto layer : entities)
	{
		for (Entity* e : layer)
		{
			if (e->IsActiveIn(SceneManager::GetInstance().GetCurrentSceneTag())) {
				gcle::Shape* realShape = e->GetShape();

				e->SetRenderPosition((realShape->GetPosition() - GetPosition()) * static_cast<float32>(GetZoom()) + screenMiddle);

				Vector2f realScale = e->GetScale();
				e->GetRenderShape()->SetScale({ realScale.x * static_cast<float32>(GetZoom()), realScale.y * static_cast<float32>(GetZoom()) });
				e->GetRenderShape()->SetRotation(e->GetRotation());
			}
		}
	}
}

void Camera::SetZoom(float32 zoom)
{
	m_zoom = zoom;
}

float32 Camera::GetZoom()
{
	return m_zoom;
}
