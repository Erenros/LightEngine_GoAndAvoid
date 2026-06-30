#pragma once
#include <vector>
#include "include.h"
#include <unordered_set>
#include "Utils.h"


#define maxEntities 8
#define maxDepth 9

class Collider;


class QuadNodePool;

class QuadNode {
	AABB m_bounds;
	int32 m_depth = 0;

	std::vector<Collider*> m_entities;  //if a leaf
	QuadNode* m_childs[4] = { nullptr, nullptr, nullptr, nullptr };   //if had children


public:
	QuadNode() = default;
	QuadNode(float32 x, float32 y, float32 w, float32 h, int32 d);
	
	~QuadNode();

private:
	void Subdivide(QuadNodePool& pool);
	bool IsLeaf();

	void Insert(Collider* entity, QuadNodePool& pool);
	void Query(AABB& range, std::vector<Collider*>& results, std::vector<Collider*>& seen);

	void Clear();


	friend class QuadTree;
	friend class PhysicsManager;
	friend class QuadNodePool;

};


class QuadNodePool {
	std::vector<std::vector<QuadNode>> m_blocks;
	std::vector<QuadNode>* m_currentBlock = nullptr;
	int32 m_blockSize;
	int32 m_index = 0;

	void AddBlock();

public:

	QuadNodePool(int32 size);
	

	QuadNode* Get(float32 x, float32 y, float32 x2, float32 y2, int32 depth);
	void Reset();
};