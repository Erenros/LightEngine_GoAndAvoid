#include "Entity.h"
#include "Core/InputManager.h"

void Entity::Initialize(Shape& shape)
{ 
	m_Direction = { 0.0f, 0.0f };
	m_Speed = 0.f;
	m_ToDestroy = false;
	m_Tag = -1;
	m_Target;
	m_RigidBody = false;

	mp_Shape = &shape;

	mp_RenderShape = new Shape(shape);

	m_Target.isSet = false;

	OnInitialize();
}

void Entity::Update(Timer& timer)
{
	double dt = timer.GetChronoTime();
	float distance = dt * m_Speed;
	Vector2f translation = m_Direction * distance;

	mp_Shape->Move(translation);

	if (m_Target.isSet)
	{
		float32 x1 = GetPosition(0.5f, 0.5f).x;
		float32 y1 = GetPosition(0.5f, 0.5f).y;

		float32 x2 = x1 + m_Direction.x * m_Target.distance;
		float32 y2 = y1 + m_Direction.y * m_Target.distance;

		m_Target.distance -= distance;

		if (m_Target.distance <= 0)
		{
			SetPosition(m_Target.position.x, m_Target.position.y, 0.5f, 0.5f);
			m_Direction = Vector2f({ 0.f, 0.f });
			m_Target.isSet = false;
		}
	}

	OnUpdate();
}

void Entity::Destroy()
{
	m_ToDestroy = true;
	OnDestroy();
}

bool Entity::GoToPosition(float32 x, float32 y, float32 speed)
{
	if (GoToDirection(x, y, speed) == false)
		return false;

	Vector2f position = mp_Shape->GetPosition(0.5f, 0.5f);

	m_Target.position = { x, y }; 
	m_Target.distance = position.GetDistance({ x, y }); 
	m_Target.isSet = true;

	return true;
}

bool Entity::GoToDirection(float32 x, float32 y, float32 speed)
{
	Vector2f position = mp_Shape->GetPosition(0.5f, 0.5f);
	Vector2f direction = Vector2f({ x - position.x, y - position.y });

	direction = direction.Normalized();

	SetDirection(direction.x, direction.y, speed);

	return true;
} 
 
Vector2f Entity::GetPosition(float32 ratioX, float32 ratioY)
{
	return mp_Shape->GetPosition(ratioX, ratioY);
}

void Entity::SetDirection(float32 x, float32 y, float32 speed)
{
	if (speed > 0)
	{
		m_Speed = speed;
	}

	m_Direction = { x, y };
}

void Entity::SetPosition(float32 x, float32 y, float32 ratioX, float32 ratioY)
{
	mp_Shape->SetPosition(x, y, ratioX, ratioY);
}

void Entity::SetRenderPosition(float32 x, float32 y, float ratioX, float ratioY)
{
	mp_RenderShape->SetPosition(x, y, ratioX, ratioY);
}

void Entity::SetRenderPosition(Vector2f v, float ratioX, float ratioY)
{
	mp_RenderShape->SetPosition(v.x, v.y, ratioX, ratioY);
}

void Entity::SetRenderSize(int shapeType, std::vector<float32> points)
{
	if (shapeType == 0)
	{
		mp_RenderShape->SetWidth(points[0]);
		mp_RenderShape->SetHeight(points[1]);
	}

	else if (shapeType == 1)
	{
		mp_RenderShape->SetRadius(points[0]);
	}

	else if (shapeType == 2)
	{
		std::vector<Vector2f> newTrianglePoints;
		newTrianglePoints.push_back({ points[0], points[1] });
		newTrianglePoints.push_back({ points[2], points[3] });
		newTrianglePoints.push_back({ points[4], points[5] });

		mp_RenderShape->SetTrianglePoints(newTrianglePoints);
	}
}

Vector2f Entity::GetRenderPosition()
{
	return mp_RenderShape->GetPosition();
}

