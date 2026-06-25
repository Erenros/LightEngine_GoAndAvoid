#pragma once
#include "include.h"
#include "Render/Shape.h"

using namespace gcle;

class Entity;   // <-- AJOUT : forward declaration

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
	Collider(Shape* shape);
	~Collider() = default;

	void CollidingOn(Vector2f direction);
	void CollidingOnX(float32 direction);
	void CollidingOnY(float32 direction);

	void StoppedColliding();

	bool IsActive() { return m_IsActive; }
	void SetActive(bool activate) { m_IsActive = activate; }

	Shape* GetShape() { return m_shape; }
	const Shape* GetShape() const { return m_shape; }

	const CollisionDirection& GetCollisionDirection() const { return m_CollisionDirection; }

	Entity* GetOwner() const { return m_pOwner; }
	void SetOwner(Entity* pOwner) { m_pOwner = pOwner; }

private:
	Shape* m_shape;

	Entity* m_pOwner = nullptr;

	bool m_IsActive = false;

	bool IsColliding = false;
	CollisionDirection m_CollisionDirection;
};