#pragma once 
#include <vector>

#include "include.h"
#include "Entity.h"
#include "Collider.h"
#include "Render/Shape.h"
#include "QuadTree.h"



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
	bool IsColliding(Collider* pCollider1, Collider* pCollider2);
	bool IsInside(Entity* pEntity, Vector2f positionToCheck);
	void ThrowRepulse(Collider* pCollider1, Collider* pCollider2);

private:
	bool CheckAABBAABBCollision(gcle::Rectangle* pRect1, gcle::Rectangle* pRect2);
	bool CheckAABBCircleCollision(gcle::Rectangle* pRect, gcle::Circle* pCircle);
	bool CheckCircleCircleCollision(gcle::Circle* pCircle1, gcle::Circle* pCircle2);
	bool CheckOBBAABBCollision(gcle::Rectangle* pRect1, gcle::Rectangle* pRect2);
	bool CheckOBBOBBCollision(gcle::Rectangle* pRect1, gcle::Rectangle* pRect2);
	bool CheckOBBCircleCollision(gcle::Rectangle* pRect, gcle::Circle* pCircle);

private:
	bool ThrowCheckRectRect(gcle::Shape* a, gcle::Shape* b);
	bool ThrowCheckCircleCircle(gcle::Shape* a, gcle::Shape* b);
	bool ThrowCheckRectCircle(gcle::Shape* a, gcle::Shape* b);
	bool ThrowCheckCircleRect(gcle::Shape* a, gcle::Shape* b);

private:
	void RepulseRectRect(Collider* a, Collider* b);
	void RepulseCircleCircle(Collider* a, Collider* b);
	void RepulseRectCircle(Collider* a, Collider* b);
	void RepulseCircleRect(Collider* a, Collider* b);

	float32 GetRepulseCorrectionMultiplyer(Collider* a, Collider* b);

	void AccumulateCorrection(Entity* pEntity, Vector2f delta);

	void RepulseOBB(Collider* colA, Collider* colB);

private: 
	std::vector<EntityInfo> m_EntitiesToAdd;
	std::vector<EntityInfo> m_EntitiesToUpdate;
	std::vector<EntityInfo> m_EntitiesToRemove;

private:
	std::unordered_map<Entity*, Vector2f> m_PendingCorrections;

private:
	using CollisionFn = bool(PhysicsManager::*)(gcle::Shape*, gcle::Shape*);
	static CollisionFn collisionTable[static_cast<int32>(gcle::Shapes::Count) - 1][static_cast<int32>(gcle::Shapes::Count) - 1];

	using RepulseFn = void(PhysicsManager::*)(Collider* a, Collider* b);
	static RepulseFn repulseTable[static_cast<int32>(RepulseTypes::Count)][static_cast<int32>(RepulseTypes::Count)];

private:
	CollisionInfo m_colDatas;

private:
	bool m_activateQuadTree = true;
	bool m_dynamicQuadTreeSize = false;
	
	Vector2f m_quadTreePos1{ -50000, -50000 };
	Vector2f m_quadTreePos2{50000, 50000};

	QuadTree* m_quadTree = new QuadTree(m_quadTreePos1.x, m_quadTreePos1.y, m_quadTreePos2.x, m_quadTreePos2.y);

	int8 m_frameBetweenQuadTreeRegenerations = 1;
	int8 m_timeBetweenRegeneration = 0;

	std::vector<std::pair<Collider*, Collider*>> m_pairs;
	std::vector<Collider*> m_queryResult;

public:
	void SetActivateQuadTree(bool activate);
	void SetDynamicQuadTreeSize(bool activate);
	void SetQuadTreePos1(Vector2f pos1);
	void SetQuadTreePos2(Vector2f pos2);
	void SetFrameBetweenQuadTreeRegenerations(int8 nbrFrame);

private:
	void EntityToRemove(std::vector<EntityInfo>& m_EntitiesToRemove, std::vector<EntityInfo>& m_EntitiesToUpdate);
	void EntityToAdd(std::vector<EntityInfo>& m_EntitiesToAdd, std::vector<EntityInfo>& m_EntitiesToUpdate);
	void EntityToUpdate(std::vector<Collider*>* activeColliders, std::vector<EntityInfo>& m_EntitiesToUpdate);
	void GenerateQuadTree(std::vector<Collider*>* activeColliders);
	void PendingCorrections();
	void HandleCollision(std::pair<Collider*, Collider*> collider);
	void MakeTreePairs(std::vector<Collider*>* activeColliders);
	void MakePairs(std::vector<Collider*>* activeColliders);

	void UpdateQuadTree(std::vector<Collider*> activeColliders);
	void UpdateWithoutQuadTree(std::vector<Collider*> activeColliders);

public : 
	~PhysicsManager();

	Collider* m_pCurrentColliderA = nullptr;
	Collider* m_pCurrentColliderB = nullptr;
};