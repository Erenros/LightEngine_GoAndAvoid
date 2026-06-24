#pragma once
#include "include.h"

struct CollisionDirection
{
	bool IsCollidingOnTop = false;
	bool IsCollidingOnBottom = false;
	bool IsCollidingOnRight = false;
	bool IsCollidingOnLeft = false;
};


class Collider
{
public:
	Collider() = default;
	~Collider() = default;

	void CollidingOn(Vector2f direction);
	void CollidingOnX(float32 direction);
	void CollidingOnY(float32 direction);

	void StoppedColliding();

	bool IsActive() { return m_IsActive; }
	void SetActive(bool activate) { m_IsActive = activate; }

private:
	bool m_IsActive = false;

	bool IsColliding = false;
	CollisionDirection m_CollisionDirection;
};
