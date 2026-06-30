#include "GameManager.h"
#include "Camera.h"

static uint64 sId = 0;

void Camera::Init(Window* pWindow)
{
	transform.Initialize({ 0.0f, 0.0f }, 0); 

	mp_Window = pWindow;

	screenMiddle = { 1920.f / 2.f, 1080.f / 2.f };

	m_Id = sId++;
}

void Camera::SetFollowing(Entity* newEntity)
{
	m_followingEntity = newEntity;
}

Entity* Camera::GetFollowing()
{
	return m_followingEntity;
} 

void Camera::SetPosition(Vector2f v)
{
	transform.SetPosition(v);
}

Vector2f Camera::GetPosition()
{
	return transform.GetPosition();
} 

void Camera::Update(Clock& time, std::vector<std::vector<Entity*>>& entities)
{
	if (m_followingEntity != nullptr)
		transform.SetPosition(m_followingEntity->GetPosition());

	screenMiddle = Vector2f{ 1920.0f, 1080.0f } * 0.5f;
	
	for (auto& layer : entities)
	{
		for (Entity* entity : layer)
		{
			if (e->IsActiveIn(SceneManager::GetInstance().GetCurrentSceneTag())) {
				entity->SetRenderPosition((entity->GetPosition() - GetPosition()) + screenMiddle);

				Vector2f realScale = entity->GetScale();
				entity->GetRenderShape()->SetScale({ realScale.x * static_cast<float32>(GetZoom()), realScale.y * static_cast<float32>(GetZoom()) });
				entity->GetRenderShape()->SetRotation(entity->GetRotation());
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

Vector2f Camera::GetScreenMousePosition()
{
	return GameManager::GetInstance().GetWindow()->GetMousePositionOnRenderTarget();
}

Vector2f Camera::GetMouseScreenToWorldPosition()
{ 
	constexpr float32 RENDER_TARGET_WIDTH = 1920.f;
	constexpr float32 RENDER_TARGET_HEIGHT = 1080.f;

	Vector2f mousePosOnTarget = mp_Window->GetMousePositionOnRenderTarget();
	Vector2f screenCenter = Vector2f{ RENDER_TARGET_WIDTH, RENDER_TARGET_HEIGHT } *0.5f;

	return transform.GetPosition() + (mousePosOnTarget - screenCenter) / m_zoom;
} 

void Camera::SetActive(bool isActive)
{
	m_isActive = isActive;
}

bool Camera::IsActive() const
{
	return m_isActive;
}

uint64 Camera::GetId() const
{
	return m_Id;
}