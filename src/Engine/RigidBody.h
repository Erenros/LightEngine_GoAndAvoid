#pragma once
#include <string>

#include "Include.h"
#include "Transform.h"

class RigidBody2D
{
public:
	RigidBody2D();
	~RigidBody2D();

	void Initialize(Transform2D* mp_Transform);

	void Update(Clock& timer);

	void AddForce(Vector2f direction, float32 strength, float32 dt);

	void SetForce(Vector2f direction, float32 speed);
	void AddImpulse(Vector2f direction, float32 strength);
	void SetMass(float32 mass) { m_Mass = mass; }

	bool IsActive() const { return IsRigidBody; }
	void SetActive(bool Active) { IsRigidBody = Active; }

	void ClampVelocity();
	void Stop();
	void SetMaxSpeed(float32 speed) { m_MaxSpeed = speed; }

	void SetGravity(float32 strenght, bool isActive) { m_Gravity = strenght; m_UseGravity = isActive; }
	void SetGravity(bool isActive) { m_UseGravity = isActive; }
	void SetGravity(float32 strenght) { m_Gravity = strenght; }

	float32 GetSpeed() const;
	Vector2f GetVelocity() const;

private:
	void ApplyVelocity(float32 dt);
	void ApplyFriction(float32 dt);
	void ApplyGravity(float32 dt);

private:
	Vector2f m_Position;
	Vector2f m_Direction;
	Vector2f m_Velocity;
	float32 m_Mass = 1.0f;
	float32 m_Friction = 0.1f;

	float32 m_MaxSpeed = 500.0f;

	float32 m_Gravity = GRAVITY;
	bool m_UseGravity = true;

	float32 m_Restitution = 0.5f;

	bool IsRigidBody = true;

	Transform2D* mp_Transform = nullptr;

	float64 dt = 0.0f;
};


