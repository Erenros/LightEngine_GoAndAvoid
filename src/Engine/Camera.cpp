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
			gcle::Shape* realShape = e->GetShape();

			e->SetRenderPosition((realShape->GetPosition() - GetPosition()) * static_cast<float32>(GetZoom()) + screenMiddle);

			Vector2f realScale = e->GetScale();
			e->GetRenderShape()->SetScale({ realScale.x * static_cast<float32>(GetZoom()), realScale.y * static_cast<float32>(GetZoom()) });
			e->GetRenderShape()->SetRotation(e->GetRotation());

			/*
			if(i == 0)
			{
				std::cout << "Position X de realShape : " << realShape->GetPosition().x << std::endl;
				std::cout << "Position X de RenderShape : " << m_entities[i]->GetRenderShape()->GetPosition().x << std::endl;
			}

			int mode = 0;

			std::vector<float32> values;


			if (realShape->GetShape() == gcle::Shapes::Rectangle)
			{
				gcle::Rectangle* rect = static_cast<gcle::Rectangle*>(realShape);
				values.push_back(rect->GetWidth());
				values.push_back(rect->GetHeight());

				mode = 0;
			}

			else if (realShape->GetShape() == gcle::Shapes::Circle)
			{
				gcle::Circle* circ = static_cast<gcle::Circle*>(realShape);
				values.push_back(circ->GetRadius());

				mode = 1;
			}

			else if (realShape->GetShape() == gcle::Shapes::Triangle)
			{
				gcle::Triangle* tri = static_cast<gcle::Triangle*>(realShape);

				for(short i = 0; i < 3; i++)
				{
					values.push_back(tri->GetTrianglePoints()[0].x);
					values.push_back(tri->GetTrianglePoints()[0].y);
				}

				mode = 2;
			}

			m_entities[i]->SetRenderSize(mode, values);*/

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
