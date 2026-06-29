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
	Collider() = default;
	~Collider() = default;

	void Initialize(Shape* shape, Vector2f position, Entity* owner);

	void CollidingOn(Vector2f direction);
	void CollidingOnX(float32 direction);
	void CollidingOnY(float32 direction);

	void StoppedColliding();

	bool IsActive() { return m_IsActive; }
	void SetActive(bool activate) { m_IsActive = activate; }

	Shape* GetShape() { return m_shape; }
	const Shape* GetShape() const { return m_shape; }

	const CollisionDirection& GetCollisionDirection() const { return m_CollisionDirection; }

	Entity* GetOwner() const { return mp_Owner; }
	void SetOwner(Entity* pOwner) { mp_Owner = pOwner; }

private:
	Shape* m_shape;

	Entity* mp_Owner = nullptr;

	bool m_IsActive = false;

	CollisionDirection m_CollisionDirection;

private:
	bool m_inQuerySeen = false;

public:
	bool GetInQuerySeen();
	void SetInQuerySeen(bool seen);
};