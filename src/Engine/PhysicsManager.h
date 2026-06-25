#pragma once 
#include "include.h"
#include "Entity.h"
#include "Shape.h"
#include "QuadTree.h"
#include <vector>

struct EntityInfo
{
	Entity* pEntity = nullptr;
	bool toRemove = false;
};


struct CollisionInfo {
	float32 penetration = 0.f;
	Vector2f orientation{ 0.f, 0.f };
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
	bool IsColliding(Entity* pEntity1, Entity* pEntity2);
	bool IsInside(Entity* pEntity, Vector2f positionToCheck);
	void Repulse(Entity* pEntity1, Entity* pEntity2);

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
	void RepulseRectRect(gcle::Shape* a, gcle::Shape* b);
	void RepulseCircleCircle(gcle::Shape* a, gcle::Shape* b);
	void RepulseRectCircle(gcle::Shape* a, gcle::Shape* b);
	void RepulseCircleRect(gcle::Shape* a, gcle::Shape* b);

	void RepulseOBB(gcle::Shape* a, gcle::Shape* b);
	
	float32 GetRepulseCorrectionMultiplyer(gcle::Shape* a, gcle::Shape* b);

private:
	std::vector<EntityInfo> m_EntitiesToUpdate;

private:
	using CollisionFn = bool(PhysicsManager::*)(gcle::Shape*, gcle::Shape*);
	static CollisionFn collisionTable[static_cast<int32>(gcle::Shapes::Count) -1][static_cast<int32>(gcle::Shapes::Count) - 1];

	using RepulseFn = void(PhysicsManager::*)(gcle::Shape*, gcle::Shape*);
	static RepulseFn repulseTable[static_cast<int32>(RepulseTypes::Count)][static_cast<int32>(RepulseTypes::Count)];

private:
	CollisionInfo m_colDatas;

private:
	bool m_activateQuadTree = true;
	bool m_dynamicQuadTreeSize = false;
	
	Vector2f m_quadTreePos1{ -50000, -50000 };
	Vector2f m_quadTreePos2{50000, 50000};

	QuadTree* m_quadTree = new QuadTree(m_quadTreePos1.x, m_quadTreePos1.y, m_quadTreePos2.x, m_quadTreePos2.y);

public:
	void SetActivateQuadTree(bool activate);
	void SetDynamicQuadTreeSize(bool activate);
	void SetQuadTreePos1(Vector2f pos1);
	void SetQuadTreePos2(Vector2f pos2);
};

