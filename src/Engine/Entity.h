#pragma once

#include "include.h"
#include "Transform.h"

class Entity
{
public:
	void Destroy();
	bool IsInside(float x, float y) const; 
	bool IsColliding(Entity* other) const;
	bool GoToPosition(int x, int y, float speed = -1.f);
	bool GoToDirection(int x, int y, float speed = -1.f);

public:
	Vector2f GetPosition(float ratioX = 0.5f, float ratioY = 0.5f) const;
	//Shape* GetShape() { return &mShape; }

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
	void Update();
	void Initialize(Vector2f position, Vector2f direction);
	void Repulse(Entity* other);


protected:
	float m_Speed = 0.f;
	bool m_ToDestroy = false;
	int m_Tag = -1;
	bool m_RigidBody = false;

	Transform2D* mp_Transform = nullptr;

};