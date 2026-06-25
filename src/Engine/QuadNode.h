#pragma once
#include <vector>
#include <memory>
#include <array>
#include "include.h"

#define maxEntities 8
#define maxDepth 7


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


class QuadNode {
	AABB m_bounds;
	int depth = 0;

	std::vector<ColliderEntry> m_entities;  //if a leaf
	QuadNode* m_childs[4];   //if had children

	Vector2f GetPosition();
	Vector2f GetSize();

	QuadNode() = default;
	QuadNode(float32 x, float32 y, float32 w, float32 h);

	~QuadNode();

	void Subdivide();
	bool IsLeaf();

	void Insert(Entity* entity);
	void Query(AABB& range, std::vector<ColliderEntry>& results);

	void Clear();


	friend class QuadTree;
};