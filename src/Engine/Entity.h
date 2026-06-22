#pragma once
#include <vector>
#include "include.h"
#include "Transform.h"
#include "Render/Shape.h"
#include "Render/Texture.h"
#include "RigidBody.h"

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
	 
	void SetScale(Vector2f scale);
	void SetScale(float32 scale) { SetScale({ scale, scale }); }
	void ScaleBy(Vector2f factor);
	 
	void SetRotation(Degrees angle);
	void Rotate(Degrees delta);

	void SetTexture(Texture* pTexture);

public:
	bool IsRigidBody() const { return m_RigidBody.IsActive(); }
	bool ToDestroy() const { return m_ToDestroy; }
	bool IsTag(int32 tag) const { return m_Tag == tag; }
	bool IsColliding(Entity* other);
	bool IsInside(Vector2f position);

protected:
	Entity() = default;
	~Entity();


	virtual void OnUpdate() {};
	virtual void OnCollisionEnter(Entity* collidedWith) {};
	virtual void OnCollision(Entity* collidedWith) {};
	//virtual void OnCollisionExit(Entity* collidedWith) {};
	virtual void OnInitialize() {};
	virtual void OnDestroy() {};

private:
	void Update(Clock& timer);
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
	int32			m_Tag = -1;
	Target			m_Target;


private:
	gcle::Shape*	mp_Shape = nullptr;
	gcle::Shape*	mp_RenderShape = nullptr;
	RigidBody2D		m_RigidBody;

private:
	bool m_OnCollisionEnter = true;
	bool m_WasOnCollision	= false;
	bool m_OnCollisionExit	= false;


private:
	std::vector<std::string> m_activeScenes;


private:

	friend class Scene;
	friend class GameManager;
	friend class Camera;
	friend class PhysicsManager; 

};