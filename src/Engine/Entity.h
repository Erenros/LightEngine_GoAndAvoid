#pragma once
#include <vector>
#include <unordered_set>
#include "include.h"
#include "Transform.h"
#include "Render/Shape.h"
#include "Render/Texture.h"
#include "Render/Sprite.h"
#include "RigidBody.h"
#include <cmath>

class Collider;

struct Target
{
	Vector2f position;
	float distance = 0.f;
	bool isSet = false;
};

class Entity
{
public:
	void Destroy();
	void Move(Vector2f translation);
	bool GoToPosition(float32 x, float32 y, float32 speed = -1.f);
	bool GoToDirection(float32 x, float32 y, float32 speed = -1.f);

public:
	void SetTag(int32 tag) { m_Tag = tag; }
	
	void SetSpeed(float32 speed) { m_Speed = speed; }
	void SetDirection(float32 x, float32 y, float32 speed = -1.f);

	void SetRigidBody(bool isRigidBody);

	void SetPosition(float32 x, float32 y);
	void SetRenderPosition(Vector2f v, float ratioX = 0.5f, float ratioY = 0.5f);
	void SetRenderPosition(float32 x, float32 y, float ratioX = 0.5f, float ratioY = 0.5f);

	void SetStatic(bool isStatic);
	void SetRenderSize(int shapeType, std::vector<float32> points);

	void SetIsKinematic(bool isKinematic) { m_IsKinematic = isKinematic; }

	void SetScale(Vector2f scale);
	void ScaleBy(Vector2f factor);
	void SetScale(float32 scale) { SetScale({ scale, scale }); }

	void Rotate(Degrees delta);
	void SetRotation(Degrees angle);

	void SetTexture(const std::string& id);

	void AddAnimation(const std::string& id, int32 firstFrame, int32 lastFrame, int32 line, int32 tileWidth, int32 tileHeight, float32 duration = 0.5f);
	void PlayAnimation(const std::string& id, int32 mode = 0);

	void SetLayer(int32 layer) { m_layer = std::clamp(layer, 0, 15); };

public:
	//void SetRenderShape(gcle::Shapes);
	Vector2f GetScale();
	Degrees GetRotation();
	Vector2f GetPosition();
	Vector2f GetRenderPosition();
	int64 GetId() const { return m_Id; }
	int32 GetLayer() const { return m_layer; };
	RigidBody2D& GetRigidBody() { return m_RigidBody; }
	Transform2D& GetTransform2D() { return m_Transform; }
	gcle::Shape* GetRenderShape() { return mp_RenderShape; };

public:
	bool IsStatic() const;
	bool IsColliding(Entity* other);
	bool IsInside(Vector2f position);
	bool ToDestroy() const { return m_ToDestroy; }
	bool IsKinematic() const { return m_IsKinematic; }
	bool IsTag(int32 tag) const { return m_Tag == tag; }
	bool IsRigidBody() const { return m_RigidBody.IsActive(); }

public:
	void AddCollider(Collider* pCollider);
	void RemoveCollider(Collider* pCollider);
	const std::unordered_set<Collider*>& GetColliders() const { return mp_Colliders; }
	Collider* CreateCollider(gcle::Shapes shape, bool isActive, Vector2f position, float32 rotation, Vector2f scale);

public:
	void AddActiveScene(const std::string& sceneTag);
	void RemoveActiveScene(const std::string& sceneTag); 
	bool IsActiveIn(const std::string& sceneTag);

public:
	bool IsWorldText() const { return m_isWorldText; }

protected:
	Entity() = default;
	~Entity();
	 
	virtual void OnUpdate() {};
	virtual void OnDestroy() {};
	virtual void OnInitialize() {};
	virtual void OnCollision(Entity* collidedWith) {};
	virtual void OnCollisionExit(Entity* collidedWith) {};
	virtual void OnCollisionEnter(Entity* collidedWith) {};

private:
	void Initialize();
	void Update(float32 dt);
	void Initialize(gcle::Shapes shape);
	gcle::Shape* GetBaseShape(gcle::Shapes shape);

private:
	void SetDebugLayer(int32 layer) { m_layer = std::clamp(layer, 0, 31); } 

protected:

	Target			m_Target;
	int64			m_Id			= 0;
	int32			m_Tag			= -1;
	float32			m_Speed			= 0.f;
	bool			m_ToDestroy		= false;
	bool			m_IsKinematic	= true;
	Vector2f		m_Direction		= { 0.0f, 0.0f };

private:
	bool m_isStatic			= false;
	bool m_isHighlighted	= false;

protected:
	bool m_isWorldText = false;

private:
	Transform2D  m_Transform;
	gcle::Shape* mp_RenderShape = nullptr;
	RigidBody2D	 m_RigidBody;

private:
	int32 m_layer = 0;
	std::vector<std::string> m_activeScenes;
	std::unordered_set<Collider*> mp_Colliders;
	std::unordered_map<int64, Entity*> CollidingEntity;

private: 
	friend class Scene;
	friend class Camera;
	friend class GameManager;
	friend class PhysicsManager; 

};