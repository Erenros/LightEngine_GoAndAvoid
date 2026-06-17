#include "Entity.h"
#include <Windows.h>


void Entity::Initialize(Shape& shape, Transform2D& transform)
{
    m_Transform = transform;
	mp_Shape = &shape;

	mTarget.isSet = false;

	OnInitialize();
}

void Entity::Update(Timer& timer)
{
	double dt = timer.GetChronoTime();
	float distance = dt * m_Speed;
	Vector2f translation = m_Transform.GetDirection() * distance;
	mp_Shape->SetPosition(m_Transform.GetPosition().x, m_Transform.GetPosition().y);

	if ((GetAsyncKeyState('A') & 0x8001) != 0) 
	{
		Vector2f pos = m_Transform.GetPosition();
		m_Transform.SetPosition({pos.x + 1, pos.y + 1});
	}

	if (m_Transform.isSet)
	{
		float x1 = m_Transform.GetPosition().x;
		float y1 = m_Transform.GetPosition().y;

		float x2 = x1 + m_Transform.GetDirection().x* mTarget.distance;
		float y2 = y1 + m_Transform.GetDirection().y * mTarget.distance;

		mTarget.distance -= distance;

		if (mTarget.distance <= 0.f)
		{
			SetPosition(mTarget.position.x, mTarget.position.y, 0.5f, 0.5f);
			m_Transform.SetDirection(Vector2f({ 0,0 }));
			mTarget.isSet = false;
		}

		m_Transform.SetDirty();
	}

	OnUpdate();
}

void Entity::Destroy()
{
}

bool Entity::GoToPosition(int32 x, int32 y, float32 speed)
{
	return false;
}

bool Entity::GoToDirection(int32 x, int32 y, float32 speed)
{
	return false;
} 

Transform2D& Entity::GetTransform()
{
	return m_Transform;
}
Vector2f Entity::GetPosition(float32 ratioX, float32 ratioY)
{
	return Vector2f();
}

void Entity::SetDirection(float32 x, float32 y, float32 speed)
{
}

void Entity::SetPosition(float32 x, float32 y, float32 ratioX, float32 ratioY)
{
}

