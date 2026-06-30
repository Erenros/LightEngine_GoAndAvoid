#pragma once
#include <string>

#include "Include.h"
#include "Transform.h"

#undef max

class RigidBody2D
{
public:
	RigidBody2D();
	~RigidBody2D();

	void Initialize(Transform2D* mp_Transform);

	void Update(float32 dt);

	void AddForce(Vector2f direction, float32 strength, float32 dt);

	void SetForce(Vector2f direction, float32 speed);
	void AddImpulse(Vector2f direction, float32 strength);
	void SetMass(float32 mass) { m_Mass = std::max(0.0001f, mass); }

	bool IsActive() const { return IsRigidBody; }
	void SetActive(bool Active) { IsRigidBody = Active; }

	void ClampVelocity();
	void Stop() { m_Velocity = { 0,0 }; }
	void SetMaxSpeed(float32 speed) { m_MaxSpeed = speed; }

	void SetGravity(float32 strenght, bool isActive) { m_Gravity = strenght; m_UseGravity = isActive; }
	void SetGravity(bool isActive) { m_UseGravity = isActive; }
	void SetGravity(float32 strenght) { m_Gravity = strenght; }

	float32 GetSpeed() const;
	Vector2f GetVelocity() const;

	void SetVelocity(Vector2f velocity) { m_TempVelocity = velocity; m_TempVelHasChanged = true; }

	void ZeroVelocityX(bool right);
	void ZeroVelocityY(bool down);

	void ZeroVelocityX() { m_Velocity.x = 0; /*m_TempVelHasChanged = true;*/ }
	void ZeroVelocityY() { m_Velocity.y = 0; /*m_TempVelHasChanged = true; */}

	void RemoveVelocityAlongNormal(const Vector2f& normal);

	void SetDampingStrenght(float32 strenght) { m_Friction = { strenght, strenght }; }
	void SetDampingOnXAxis(Vector2f strenght) { m_Friction = strenght; }
	void SetDampingOnYAxis(Vector2f strenght) { m_Friction = strenght; }

private:
	void ApplyVelocity(float32 dt);
	void ApplyFriction(float32 dt);
	void ApplyGravity(float32 dt);

private:
	Vector2f m_Position;
	Vector2f m_Direction;
	Vector2f m_Velocity;
	Vector2f m_TempVelocity;
	float32 m_Mass = 1.0f;
	Vector2f m_Friction = {0.1f, 0.1f};

	float32 m_MaxSpeed = 500.0f;

	float32 m_Gravity = GRAVITY;
	bool m_UseGravity = false;

	float32 m_Restitution = 0.5f;

	bool IsRigidBody = false;

	bool m_TempVelHasChanged = false;

	Transform2D* mp_Transform = nullptr;

	float64 dt = 0.0f;

};