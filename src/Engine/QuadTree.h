#pragma once
#include "include.h"
#include "QuadNode.h"
#include "Collider.h"

class QuadTree {

	QuadNode m_root;
	QuadNodePool m_pool;
	std::vector<ColliderEntry> m_queryResult;
	std::vector <Entity*> m_seenEntities;

public:


	QuadTree(float32 worldMinX, float32 worldMinY, float32 worldMaxX, float32 worldMaxY);
	QuadTree() = default;


	~QuadTree() = default;

	void Insert(Collider* entry);

	void Clear();

	std::vector<ColliderEntry>& Query(ColliderEntry& entry);
	friend class PhysicsManager;
};