#pragma once
#include <vector>
#include "include.h"
#include <unordered_set>

#define maxEntities 8
#define maxDepth 9


class Entity;

struct AABB {
	float32 minX, minY, maxX, maxY;

	bool overlaps(const AABB& other) {
		return minX < other.maxX && minY < other.maxY && maxX > other.minX && maxY > other.minY;
	}

	bool include(const AABB& other) {
		return other.minX >= minX && other.maxX <= maxX && other.minY >= minY && other.maxY <= maxY;
	}
};
struct ColliderEntry{
	AABB aabb;
	Entity* entity;
};
class QuadNodePool;

class QuadNode {
	AABB m_bounds;
	int32 m_depth = 0;

	std::vector<ColliderEntry> m_entities;  //if a leaf
	QuadNode* m_childs[4] = { nullptr, nullptr, nullptr, nullptr };   //if had children

	QuadNode() = default;
	QuadNode(float32 x, float32 y, float32 w, float32 h, int32 d);

	~QuadNode();

	void Subdivide(QuadNodePool& pool);
	bool IsLeaf();

	void Insert(Entity* entity, QuadNodePool& pool);
	void Query(AABB& range, std::vector<ColliderEntry>& results, std::unordered_set<Entity*>& seen);

	void Clear();


	friend class QuadTree;
	friend class PhysicsManager;
	friend class QuadNodePool;

};


class QuadNodePool {
	std::vector<QuadNode> m_pool;
	int32 m_index = 0;

public:

	QuadNodePool(int32 size);
	

	QuadNode* Get(float32 x, float32 y, float32 x2, float32 y2, int32 depth);
	void Reset();
};