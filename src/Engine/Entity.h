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
	void SetTag(int32 tag);
	
	void SetSpeed(float32 speed);
	void SetDirection(float32 x, float32 y, float32 speed = -1.f);

	void SetRigidBody(bool isRigidBody);

	void SetPosition(float32 x, float32 y);
	void SetRenderPosition(Vector2f v, float32 ratioX = 0.5f, float32 ratioY = 0.5f);
	void SetRenderPosition(float32 x, float32 y, float32 ratioX = 0.5f, float32 ratioY = 0.5f);

	void SetStatic(bool isStatic);
	void SetRenderSize(int32 shapeType, std::vector<float32> points); 

	void SetScale(Vector2f scale);
	void ScaleBy(Vector2f factor);
	void SetScale(float32 scale);

	void Rotate(Degrees delta);
	void SetRotation(Degrees angle);

	void SetTexture(const std::string& id);

	void AddAnimation(const std::string& id, int32 firstFrame, int32 lastFrame, int32 line, int32 tileWidth, int32 tileHeight, float32 duration = 0.5f);

	void PlayAnimation(const std::string& id, AnimationMode mode = AnimationMode::IgnoreIfAlreadyPlaying);
	void AddFunctionInFrame(const std::string& animation, int32 frame, std::function<void()> function);

	void StopAnimation(); 
	void RemoveFunctionInFrame(const std::string& animation, int32 frame);

	void SetLayer(int32 layer);

	void SetColor(Color color);
	Color GetColor() const;

public:
	//void SetRenderShape(gcle::Shapes);
	Vector2f GetScale();
	Degrees GetRotation();
	Vector2f GetPosition();
	Vector2f GetRenderPosition();
	int64 GetId() const			 ;
	int32 GetLayer() const		 ;
	RigidBody2D& GetRigidBody()	 ;
	Transform2D& GetTransform2D();
	gcle::Shape* GetRenderShape();

public:
	bool IsStatic() const;
	bool IsColliding(Entity* pOther);
	bool IsInside(Vector2f position);
	bool ToDestroy() const;
	bool IsTag(int32 tag) const;
	bool IsRigidBody() const;

public:
	void AddCollider(Collider* pCollider);
	void RemoveCollider(Collider* pCollider); 
	const std::unordered_set<Collider*>& GetColliders() const;
	Collider* CreateCollider(gcle::Shapes shape, bool isActive, Vector2f relativePosition, float32 rotation, Vector2f scale, bool isTrigger = false);

public:
	void AddActiveScene(const std::string& sceneTag);
	void RemoveActiveScene(const std::string& sceneTag); 
	bool IsActiveIn(const std::string& sceneTag);

public:
	bool IsWorldText() const;

protected:
	Entity() = default;
	~Entity();
	 
	virtual void OnUpdate() {};
	virtual void OnDestroy() {};
	virtual void OnInitialize() {};

	virtual void OnCollision(Entity* pCollidedWith) {};
	virtual void OnCollisionExit(Entity* pCollidedWith) {};
	virtual void OnCollisionEnter(Entity* pCollidedWith) {};

	virtual void OnTrigger(Entity* pOther) {};
	virtual void OnTriggerExit(Entity* pOther) {};
	virtual void OnTriggerEnter(Entity* pOther) {};

private:
	void Initialize();
	void Update(float32 dt);
	void Initialize(gcle::Shapes shape);
	gcle::Shape* GetBaseShape(gcle::Shapes shape);

private:
	void SetDebugLayer(int32 layer);

protected:

	Target			m_Target;
	int64			m_Id			= 0;
	int32			m_Tag			= -1;
	float32			m_Speed			= 0.f;
	bool			m_ToDestroy		= false; 
	Vector2f		m_Direction		= { 0.0f, 0.0f };

private:
	bool m_IsStatic			= false;
	bool m_IsHighlighted	= false;

protected:
	bool m_IsWorldText = false;

private:
	Transform2D  m_Transform;
	gcle::Shape* mp_RenderShape = nullptr;
	RigidBody2D	 m_RigidBody;

private:
	int32 m_Layer = 0;
	std::vector<std::string> m_ActiveScenes;
	std::unordered_set<Collider*> mp_Colliders;
	std::unordered_map<int64, Entity*> m_CollidingEntity;
	std::unordered_map<int64, Entity*> m_TriggeringEntity;

private: 
	friend class Scene;
	friend class Camera;
	friend class GameManager;
	friend class PhysicsManager; 

};