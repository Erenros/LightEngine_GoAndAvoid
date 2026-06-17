#pragma once

#include "include.h"
#include "Transform.h"
#include "Shape.h"

using namespace gcle;

struct Target
{
	Vector2f position;
	float distance;
	bool isSet;
};


class Entity
{
public:
	void Destroy();
	bool GoToPosition(int x, int y, float speed = -1.f);
	bool GoToDirection(int x, int y, float speed = -1.f);

public:
	Transform2D& GetTransform();
	Shape* GetShape() { return mp_Shape; }

public:
	void SetTag(int tag) { m_Tag = tag; }
	void SetSpeed(float speed) { m_Speed = speed; }
	void SetDirection(float x, float y, float speed = -1.f);
	void SetRigidBody(bool isRigitBody) { m_RigidBody = isRigitBody; }
	void SetPosition(float x, float y, float ratioX = 0.5f, float ratioY = 0.5f);

public:
	bool IsRigidBody() const { return m_RigidBody; }
	bool IsTag(int tag) const { return m_Tag == tag; }
	bool ToDestroy() const { return m_ToDestroy; }

protected:
	Entity() = default;
	~Entity() = default;

	virtual void OnUpdate() {};
	virtual void OnCollision(Entity* collidedWith) {};
	virtual void OnInitialize() {};
	virtual void OnDestroy() {};

private:
	void Update(Timer& timer);
	void Initialize(Shape& shape, Transform2D& transform);


protected:
	float m_Speed = 0.f;
	bool m_ToDestroy = false;
	int m_Tag = -1;
	Target mTarget;
	bool m_RigidBody = false;
	Shape* mp_Shape = nullptr;

	Transform2D m_Transform;

private:
	friend class GameManager;
};