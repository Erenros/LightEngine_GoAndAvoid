#include "Entity.h"

void Entity::Initialize(Vector2f position, Degrees angle)
{
    m_Transform = Transform2D();
    m_Transform.Initialize(position, angle);

	mTarget.isSet = false;

	OnInitialize();
}

void Entity::Update(Timer* timer)
{
	double dt = timer->GetChronoTime();
	float distance = dt * m_Speed;
	Vector2f translation = m_Transform.GetDirection() * distance;
	//mShape.move(translation);

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
