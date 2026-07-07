#pragma once
#include <vector>
#include <unordered_set>

#include "GameObject.h"

#include "RigidBody.h"


class Collider;


class Entity : public GameObject
{

private:
	void Initialize(gcle::Shapes shape) override;
	void Initialize();
	void Update(float32 dt);

public:
	void Destroy();

public:
	void SetTag(int32 tag) { m_Tag = tag; }

	void SetRigidBody(bool isRigidBody);

	void SetStatic(bool isStatic);

	void SetLayer(int32 layer) { m_layer = std::clamp(layer, 0, 15); };

protected:
	Entity() = default;
	~Entity();
	 
	virtual void OnUpdate() {};
	virtual void OnDestroy() {};
	virtual void OnInitialize() {};
	virtual void OnCollision(Entity* collidedWith) {};
	virtual void OnCollisionExit(Entity* collidedWith) {};
	virtual void OnCollisionEnter(Entity* collidedWith) {};

public:
	int32* GetCollisionLayer() { return &m_Mask; }
	int64 GetId() const { return m_Id; }
	int32 GetLayer() const { return m_layer; };
	RigidBody2D& GetRigidBody() { return m_RigidBody; }
	Transform2D& GetTransform2D() { return m_Transform; }
	gcle::Shape* GetRenderShape() { return mp_RenderShape; };

public:
	bool IsStatic() const;
	bool IsColliding(Entity* other);
	bool ToDestroy() const { return m_ToDestroy; } 
	bool IsTag(int32 tag) const { return m_Tag == tag; }
	bool IsRigidBody() const { return m_RigidBody.IsActive(); }
	bool HasCollider();

public:
	void AddCollider(Collider* pCollider);
	void RemoveCollider(Collider* pCollider); 
	const std::unordered_set<Collider*>& GetColliders() const { return mp_Colliders; }
	Collider* CreateCollider(gcle::Shapes shape, bool isActive, Vector2f relativePosition, float32 relativeRotation, Vector2f relativeScale);

public:
	bool IsWorldText() const { return m_isWorldText; }

protected:
	int64			m_Id			= 0;
	int32			m_Tag			= -1;
	bool			m_ToDestroy		= false; 

private:
	bool m_isStatic			= false;
	bool m_isHighlighted	= false;

protected:
	bool m_isWorldText = false;

private:
	RigidBody2D	 m_RigidBody;

private:
	std::unordered_set<Collider*> mp_Colliders;
	std::unordered_map<int64, Entity*> CollidingEntity;

private:
	friend class Scene;
	friend class Camera;
	friend class GameManager;
	friend class PhysicsManager;
};