#pragma once 
#include "include.h"
#include "Entity.h"
#include "Collider.h"
#include "Shape.h"

#include <vector>
#include <unordered_map>

struct EntityInfo
{
	Entity* pEntity = nullptr;
	bool toRemove = false;
};


struct CollisionInfo {
	float32 penetration = 0.0f;
	Vector2f orientation{ 0, 0 };
};

enum class RepulseTypes {
	AABB,
	Circle,
	OOB,

	Count
};

class PhysicsManager
{
public:
	static PhysicsManager& GetInstance();

	void AddEntity(Entity* pEntity);
	void RemoveEntity(Entity* pEntity);
	void Update(float64 deltaTime);
	bool IsColliding(Collider* pCollider1, Collider* pCollider2);
	bool IsInside(Entity* pEntity, Vector2f positionToCheck);
	void Repulse(Collider* pCollider1, Collider* pCollider2);

	//Entity* GetEntityById(int64 id);

private:
	bool CheckAABBAABBCollision(gcle::Rectangle* pRect1, gcle::Rectangle* pRect2);
	bool CheckAABBCircleCollision(gcle::Rectangle* pRect, gcle::Circle* pCircle);
	bool CheckCircleCircleCollision(gcle::Circle* pCircle1, gcle::Circle* pCircle2);
	bool CheckOBBAABBCollision(gcle::Rectangle* pRect1, gcle::Rectangle* pRect2);
	bool CheckOBBOBBCollision(gcle::Rectangle* pRect1, gcle::Rectangle* pRect2);
	bool CheckOBBCircleCollision(gcle::Rectangle* pRect, gcle::Circle* pCircle);

private:
	bool CheckRectRect(gcle::Shape* a, gcle::Shape* b);
	bool CheckCircleCircle(gcle::Shape* a, gcle::Shape* b);
	bool CheckRectCircle(gcle::Shape* a, gcle::Shape* b);
	bool CheckCircleRect(gcle::Shape* a, gcle::Shape* b);

private:
	void RepulseRectRect(Collider* a, Collider* b);
	void RepulseCircleCircle(Collider* a, Collider* b);
	void RepulseRectCircle(Collider* a, Collider* b);
	void RepulseCircleRect(Collider* a, Collider* b);

	void RepulseAABBOBB(Collider* a, Collider* b);
	void RepulseOBBAABB(Collider* a, Collider* b);

	void RepulseOBBCircle(Collider* a, Collider* b);

	void RepulseCircleOBB(Collider* a, Collider* b);

	void RepulseOBBOBB(Collider* a, Collider* b);

	float32 GetRepulseCorrectionMultiplyer(Collider* a, Collider* b);

	void AccumulateCorrection(Entity* pEntity, Vector2f delta);

private:
	std::vector<EntityInfo> m_EntitiesToUpdate;

private:
	std::unordered_map<Entity*, Vector2f> m_PendingCorrections;

private:
	using CollisionFn = bool(PhysicsManager::*)(gcle::Shape*, gcle::Shape*);
	static CollisionFn collisionTable[static_cast<int32>(gcle::Shapes::Count) - 1][static_cast<int32>(gcle::Shapes::Count) - 1];

	using RepulseFn = void(PhysicsManager::*)(Collider* a, Collider* b);
	static RepulseFn repulseTable[static_cast<int32>(RepulseTypes::Count)][static_cast<int32>(RepulseTypes::Count)];

private:
	CollisionInfo colDatas;

	Collider* m_pCurrentColliderA = nullptr;
	Collider* m_pCurrentColliderB = nullptr;
};