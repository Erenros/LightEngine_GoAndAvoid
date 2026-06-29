#include "QuadTree.h"


QuadTree::QuadTree(float32 worldMinX, float32 worldMinY, float32 worldMaxX, float32 worldMaxY): m_pool(256) {
	m_root = QuadNode{worldMinX, worldMinY, worldMaxX, worldMaxY , 0};
}

void QuadTree::Insert(Entity* entry){
	m_root.Insert(entry, m_pool);
}

std::vector<ColliderEntry>& QuadTree::Query(ColliderEntry& entry){
	m_queryResult.clear();
	m_querySeen.clear();
	m_querySeen.insert(entry.entity);

	float32 margin = 1.f;
	AABB marginAABB = { entry.aabb.minX - margin, entry.aabb.minY - margin, entry.aabb.maxX + margin, entry.aabb.maxY + margin };
	m_root.Query(marginAABB, m_queryResult, m_querySeen);
	
	return m_queryResult;
}

void QuadTree::Clear() {
	m_root.Clear();
	m_queryResult.clear();
	m_querySeen.clear();
}
