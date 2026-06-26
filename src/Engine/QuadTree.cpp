#include "QuadTree.h"


QuadTree::QuadTree(float32 worldMinX, float32 worldMinY, float32 worldMaxX, float32 worldMaxY) {
	root = QuadNode{worldMinX, worldMinY, worldMaxX, worldMaxY , 0};
}

void QuadTree::Insert(Entity* entry){
	root.Insert(entry);
}

std::vector<ColliderEntry>& QuadTree::Query(ColliderEntry& entry){
	m_queryResult.clear();
	m_querySeen.clear();
	m_querySeen.insert(entry.entity);

	float32 margin = 1.f;
	AABB marginAABB = { entry.aabb.minX - margin, entry.aabb.minY - margin, entry.aabb.maxX + margin, entry.aabb.maxY + margin };
	root.Query(marginAABB, m_queryResult, m_querySeen);
	
	return m_queryResult;
}

void QuadTree::Clear() {
	root.Clear();
	m_queryResult.clear();
	m_querySeen.clear();
}
