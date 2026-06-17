#pragma once

#include "include.h"
#include "Transform.h"
#include "Shape.h"


struct Target
{
	Vector2f position;
	float distance;
	bool isSet;
};


class Entity
{
private:
	void Initialize(Shape& shape, Transform2D& transform);
	void Update(Timer& timer);

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
	void SetRigidBody(bool isRigidBody) { m_RigidBody = isRigidBody; }
	void SetPosition(float x, float y, float ratioX = 0.5f, float ratioY = 0.5f);
	void SetPosition(Vector2f v);
	void SetRenderPosition(float x, float y);
	void SetRenderPosition(Vector2f v);

	Vector2f GetPosition();
	Vector2f GetRenderPosition();

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


protected:
	float m_Speed = 0.f;
	bool m_ToDestroy = false;
	int m_Tag = -1;
	Target mTarget;
	bool m_RigidBody = false;
	Shape* mp_Shape = nullptr;

	Transform2D m_Transform;

	Vector2f m_RenderPosition = { 0.f, 0.f };

private:
	friend class GameManager;
};