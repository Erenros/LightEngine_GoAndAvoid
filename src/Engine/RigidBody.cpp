#include <string>
#include <iostream>
#include <cstdlib>

#include "RigidBody.h"

RigidBody2D::RigidBody2D()
{
}

RigidBody2D::~RigidBody2D()
{
}

void RigidBody2D::Initialize(Transform2D* transform)
{
	mp_Transform = transform;
}

void RigidBody2D::Update(Clock& timer)
{
	float32 dt = static_cast<float32>(timer.GetDeltaTime());
	dt = 0.016f;

	ApplyFriction(dt);
	ApplyGravity(dt);
	ClampVelocity();
	ApplyVelocity(dt);


}

void RigidBody2D::AddForce(Vector2f direction, float32 strength, float32 dt)
{
	m_Velocity += direction.Normalized() * (strength / m_Mass) * dt;
}

void RigidBody2D::SetForce(Vector2f direction, float32 strenght)
{
	Vector2f dir = direction.Normalized();
	m_Velocity = dir * (strenght / m_Mass);
}

void RigidBody2D::AddImpulse(Vector2f direction, float32 strength)
{
	Vector2f dir = direction.Normalized();
	m_Velocity += dir * strength / m_Mass;
}

void RigidBody2D::ClampVelocity()
{
	float32 speedSq = m_Velocity.x * m_Velocity.x + m_Velocity.y * m_Velocity.y;
	float32 maxSpeedSq = m_MaxSpeed * m_MaxSpeed;

	if (speedSq > maxSpeedSq)
	{
		float32 scale = m_MaxSpeed / std::sqrt(speedSq);
		m_Velocity.x *= scale;
		m_Velocity.y *= scale;
	}
}

float32 RigidBody2D::GetSpeed() const
{
	return std::sqrt(m_Velocity.x * m_Velocity.x + m_Velocity.y * m_Velocity.y);
}

Vector2f RigidBody2D::GetVelocity() const
{
	return m_Velocity;
}

void RigidBody2D::RemoveVelocityAlongNormal(const Vector2f& normal)
{
	float32 lengthSq = normal.x * normal.x + normal.y * normal.y;

	if (lengthSq <= 0.0001f)
		return;

	Vector2f n = normal / std::sqrt(lengthSq);

	float32 dot = m_Velocity.Dot(n);

	// Si dot < 0, l'objet va contre la normale, donc il rentre dans l'obstacle.
	// Si dot > 0, l'objet s'éloigne déjà, donc on ne touche pas à sa velocity.
	if (dot < 0.0f)
	{
		m_Velocity -= n * dot;
	}
}

void RigidBody2D::ApplyVelocity(float32 dt)
{
	Vector2f pos = mp_Transform->GetPosition();
	pos += m_Velocity * dt;
	mp_Transform->SetPosition(pos);
}

void RigidBody2D::ApplyFriction(float32 dt)
{
	m_Velocity.x *= std::pow(1.0f - m_Friction.x, dt);
	m_Velocity.y *= std::pow(1.0f - m_Friction.y, dt);

	if (std::abs(m_Velocity.x) < 0.01f)
		m_Velocity.x = 0.f;

	if (std::abs(m_Velocity.y) < 0.01f)
		m_Velocity.y = 0.f;
}

void RigidBody2D::ApplyGravity(float32 dt)
{
	if (!m_UseGravity) return;
	m_Velocity.y += m_Gravity * dt;
}