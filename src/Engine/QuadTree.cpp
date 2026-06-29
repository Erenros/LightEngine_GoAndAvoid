#include "QuadTree.h"
#include "Entity.h"

QuadTree::QuadTree(float32 worldMinX, float32 worldMinY, float32 worldMaxX, float32 worldMaxY): m_pool(256) {
	m_root = QuadNode{worldMinX, worldMinY, worldMaxX, worldMaxY , 0};
}

void QuadTree::Insert(Collider* entry){
	m_root.Insert(entry, m_pool);
}

std::vector<ColliderEntry>& QuadTree::Query(ColliderEntry& entry){

	for (auto& r : m_seenEntities) {
		r->SetInQuerySeen(false);
	}
	m_seenEntities.clear();
	m_queryResult.clear();

	entry.entity->SetInQuerySeen(true);
	m_seenEntities.push_back(entry.entity);


	float32 margin = 1.f;
	AABB marginAABB = { entry.aabb.minX - margin, entry.aabb.minY - margin, entry.aabb.maxX + margin, entry.aabb.maxY + margin };
	m_root.Query(marginAABB, m_queryResult, m_seenEntities);
	
	return m_queryResult;
}

void QuadTree::Clear(){
	for (auto& r : m_seenEntities) {
		r->SetInQuerySeen(false);
	}
	m_seenEntities.clear();
	m_queryResult.clear();
	m_pool.Reset();
	m_root.Clear();
}
