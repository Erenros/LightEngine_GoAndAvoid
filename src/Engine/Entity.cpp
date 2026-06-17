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
	if ((GetAsyncKeyState('A') & 0x8001) != 0) {
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

bool Entity::GoToPosition(int x, int y, float speed)
{
    return false;
}

bool Entity::GoToDirection(int x, int y, float speed)
{
    return false;
}

Transform2D& Entity::GetTransform()
{
	return m_Transform;
}

void Entity::SetDirection(float x, float y, float speed)
{
}

void Entity::SetPosition(float x, float y, float ratioX, float ratioY)
{
}
