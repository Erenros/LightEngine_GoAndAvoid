#include "GameManager.h"
#include "Camera.h"

static uint64 sId = 0;

void Camera::Init(Window* pWindow)
{
	transform.Initialize({ 0.0f, 0.0f }, 0); 

	mp_Window = pWindow;

	screenMiddle = { GameManager::GetInstance().GetWindow()->GetWindowSize().x / 2.f, GameManager::GetInstance().GetWindow()->GetWindowSize().y / 2.f };

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

	screenMiddle = mp_Window->GetWindowSize() * 0.5f;
	
	for (auto& layer : entities)
	{
		for (Entity* entity : layer)
		{
			//entity->Update(time);

			gcle::Shape* realShape = entity->GetShape();

			entity->SetRenderPosition((realShape->GetPosition() - GetPosition()) * static_cast<float32>(GetZoom()) + screenMiddle);

			//Vector2f realScale = entity->GetScale();
			//entity->GetRenderShape()->SetScale({ realScale.x * static_cast<float32>(GetZoom()), realScale.y * static_cast<float32>(GetZoom()) });
			//entity->GetRenderShape()->SetRotation(entity->GetRotation());

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

Vector2f Camera::GetMousePosition()
{
	return GameManager::GetInstance().GetWindow()->GetMousePositionOnRenderTarget();
}

Vector2f Camera::GetMouseScreenToWorldPosition()
{ 
	Vector2f windowSize = mp_Window->GetWindowSize();
	Vector2u mousePos = mp_Window->GetMousePosition(); 

	Vector2f screenCenter = windowSize * 0.5f;

	return transform.GetPosition() + (Vector2f{ (float)mousePos.x, (float)mousePos.y } - screenCenter) / m_zoom;
}
