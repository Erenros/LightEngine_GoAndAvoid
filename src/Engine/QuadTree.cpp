#include "QuadTree.h"


QuadTree::QuadTree(float32 worldMinX, float32 worldMinY, float32 worldMaxX, float32 worldMaxY) {
	root = QuadNode{worldMinX, worldMinY, worldMaxX, worldMaxY };
}

void QuadTree::Insert(Entity* entry){
	root.Insert(entry);
}

std::vector<ColliderEntry> QuadTree::Query(ColliderEntry& entry){
	std::vector<ColliderEntry> results;
	root.Query(entry.aabb, results);

	int16 i = 0;
	for (auto& r : results) {
		if (r.entity == entry.entity) {
			results.erase(results.begin() + i);
			return results;
		}
		i++;
	}
	return results;
}

void QuadTree::Clear() {
	root.Clear();
}
