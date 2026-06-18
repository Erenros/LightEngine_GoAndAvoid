#pragma once

#include "include.h"
#include "Transform.h"
#include "Render/Shape.h"

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
	bool GoToPosition(float32 x, float32 y, float32 speed = -1.f);
	bool GoToDirection(float32 x, float32 y, float32 speed = -1.f);

public: 
	Shape* GetShape() { return mp_Shape; }
	Vector2f GetPosition(float32 ratioX = 0.5f, float32 ratioY = 0.5f);
	int64 GetId() const { return m_Id; }

public:
	void SetTag(int32 tag) { m_Tag = tag; }
	void SetSpeed(float32 speed) { m_Speed = speed; }
	void SetDirection(float32 x, float32 y, float32 speed = -1.f);
	void SetRigidBody(bool isRigitBody);
	void SetPosition(float32 x, float32 y, float32 ratioX = 0.5f, float32 ratioY = 0.5f);

public:
	bool IsRigidBody() const { return m_RigidBody; }
	bool ToDestroy() const { return m_ToDestroy; }
	bool IsTag(int32 tag) const { return m_Tag == tag; }
	bool IsColliding(Entity* other);
	bool IsInside(Vector2f position);

protected:
	Entity() = default;
	~Entity() = default;

	virtual void OnUpdate() {};
	virtual void OnCollision(Entity* collidedWith) {};
	virtual void OnInitialize() {};
	virtual void OnDestroy() {};

private:
	void Update(Timer& timer);
	void Repulse(Entity* other);
	void Initialize(Shape& shape);


protected:

	int64			m_Id = 0;
	Vector2f		m_Direction = { 0.0f, 0.0f };
	float32			m_Speed = 0.f;
	bool			m_ToDestroy = false;
	int32			m_Tag = -1;
	Target			m_Target;
	bool			m_RigidBody = false;
	Shape*			mp_Shape = nullptr;

private:

	friend class Scene;
	friend class GameManager;
	friend class PhysicsManager;
};