#include "QuadTree.h"


QuadTree::QuadTree(float32 worldMinX, float32 worldMinY, float32 worldMaxX, float32 worldMaxY) {
	root = QuadNode{worldMinX, worldMinY, worldMaxX, worldMaxY , 0};
}

void QuadTree::Insert(Entity* entry){
	root.Insert(entry);
}

std::vector<ColliderEntry>& QuadTree::Query(ColliderEntry& entry){
	root.Query(entry.aabb, m_queryResult, m_querySeen);

	int16 i = 0;
	for (auto& r : m_queryResult) {
		if (r.entity == entry.entity) {
			m_queryResult.erase(m_queryResult.begin() + i);
			return m_queryResult;
		}
		i++;
	}
	return m_queryResult;
}

void QuadTree::Clear() {
	root.Clear();
	m_queryResult.clear();
	m_querySeen.clear();
}
