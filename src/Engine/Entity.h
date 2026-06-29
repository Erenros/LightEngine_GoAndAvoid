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
	bool GoToPosition(float32 x, float32 y, float32 speed = -1.f);
	bool GoToDirection(float32 x, float32 y, float32 speed = -1.f);

public:
	gcle::Shape* GetShape() { return mp_Shape; }
	gcle::Shape* GetRenderShape() { return mp_RenderShape; };
	Vector2f GetPosition(float32 ratioX = 0.5f, float32 ratioY = 0.5f);
	int64 GetId() const { return m_Id; }
	Vector2f GetScale();
	Degrees GetRotation();
	RigidBody2D& GetRigidBody() { return m_RigidBody; }

public:
	void SetTag(int32 tag) { m_Tag = tag; }
	void SetSpeed(float32 speed) { m_Speed = speed; }
	void SetDirection(float32 x, float32 y, float32 speed = -1.f);
	void SetRigidBody(bool isRigidBody);
	void SetPosition(float32 x, float32 y, float32 ratioX = 0.5f, float32 ratioY = 0.5f);

	void SetRenderPosition(float32 x, float32 y, float ratioX = 0.5f, float ratioY = 0.5f);
	void SetRenderPosition(Vector2f v, float ratioX = 0.5f, float ratioY = 0.5f);

	void SetRenderSize(int shapeType, std::vector<float32> points);

	Vector2f GetRenderPosition();
	bool IsKinematic() { return m_IsKinematic; }
	void SetIsKinematic(bool isKinematic) { m_IsKinematic = isKinematic; }

	void SetScale(Vector2f scale);
	void SetScale(float32 scale) { SetScale({ scale, scale }); }
	void ScaleBy(Vector2f factor);

	void SetRotation(Degrees angle);
	void Rotate(Degrees delta);

	void SetTexture(const std::string& id);

	void AddAnimation(const std::string& id, int32 firstFrame, int32 lastFrame, int32 line, int32 tileWidth, int32 tileHeight, float32 duration = 0.5f);
	void PlayAnimation(const std::string& id, int32 mode = 0);

	int32 GetLayer() { return m_layer; };
	void SetLayer(int32 layer) { m_layer = std::clamp(layer, 0, 15); };

public:
	bool IsRigidBody() const { return m_RigidBody.IsActive(); }
	bool ToDestroy() const { return m_ToDestroy; }
	bool IsTag(int32 tag) const { return m_Tag == tag; }
	bool IsColliding(Entity* other);
	bool IsInside(Vector2f position);

public:
	void AddCollider(Collider* pCollider);
	void RemoveCollider(Collider* pCollider);
	Collider* CreateCollider(gcle::Shapes shape, bool isActive, Vector2f position, Vector2f scale);
	const std::unordered_set<Collider*>& GetColliders() const { return mp_Colliders; }

protected:
	Entity() = default;
	~Entity();


	virtual void OnUpdate() {};
	virtual void OnCollisionEnter(Entity* collidedWith) {};
	virtual void OnCollision(Entity* collidedWith) {};
	virtual void OnCollisionExit(Entity* collidedWith) {};
	virtual void OnInitialize() {};
	virtual void OnDestroy() {};

private:
	void Update(float32 dt);
	void Initialize(gcle::Shapes shape);
	gcle::Shape* GetBaseShape(gcle::Shapes shape);

public:
	void AddActiveScene(const std::string& sceneTag);
	void RemoveActiveScene(const std::string& sceneTag);

	bool IsActiveIn(const std::string& sceneTag);


protected:

	int64			m_Id = 0;
	Vector2f		m_Direction = { 0.0f, 0.0f };
	float32			m_Speed = 0.f;
	bool			m_ToDestroy = false;
	bool			m_IsKinematic = true;
	int32			m_Tag = -1;
	Target			m_Target;

private:
	gcle::Shape* mp_Shape = nullptr;
	gcle::Shape* mp_RenderShape = nullptr;
	RigidBody2D		m_RigidBody;

private:
	std::unordered_map<int64, Entity*> CollidingEntity;
	std::unordered_set<Collider*> mp_Colliders;
	std::vector<std::string> m_activeScenes;
	
	int32 m_layer = 0;

private:

	void SetDebugLayer(int32 layer) { m_layer = std::clamp(layer, 0, 31); }

	friend class Scene;
	friend class GameManager;
	friend class Camera;
	friend class PhysicsManager;

};