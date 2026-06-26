#include "QuadNode.h"
#include "Entity.h"


QuadNode::QuadNode(float32 x1, float32 y1, float32 x2, float32 y2, int32 d) {
	m_bounds = { x1, y1, x2, y2 };
	depth = d;
}

QuadNode::~QuadNode(){
	if (m_childs[0] == nullptr)
		return;
	for (int8 i = 0; i < 4; i++) {
		delete m_childs[i];
	}
}

void QuadNode::Subdivide(){
	Vector2f mid{ (m_bounds.minX + m_bounds.maxX) * 0.5f, (m_bounds.minY + m_bounds.maxY) * 0.5f };
	int32 d = depth + 1;
	m_childs[0] = new QuadNode(m_bounds.minX, m_bounds.minY, mid.x, mid.y, d);
	m_childs[1] = new QuadNode(mid.x, m_bounds.minY, m_bounds.maxX, mid.y, d);
	m_childs[2] = new QuadNode(m_bounds.minX, mid.y, mid.x, m_bounds.maxY, d);
	m_childs[3] = new QuadNode(mid.x, mid.y, m_bounds.maxX, m_bounds.maxY, d);

	for (auto& e : m_entities) {
		for (auto& child : m_childs) {
			if (child->m_bounds.overlaps(e.aabb)) {
				child->m_entities.push_back(e);
			}
		}
	}
	m_entities.clear();

	for (auto& child : m_childs) {
		if (static_cast<int32>(child->m_entities.size()) > maxEntities && child->depth < maxDepth) {
			child->Subdivide();
		}
	}
}

bool QuadNode::IsLeaf(){
	return m_childs[0] == nullptr;
}

void QuadNode::Insert(Entity* entity){
	AABB aabb;
	Vector2f pos1 = entity->GetPosition(0.f, 0.f);
	Vector2f pos2 = entity->GetPosition(1.f, 1.f);
	aabb = { pos1.x, pos1.y, pos2.x , pos2.y};

	if (!m_bounds.overlaps(aabb))
		return;

	if (IsLeaf()) {
		m_entities.push_back(ColliderEntry{aabb, entity});
		if (static_cast<int32>(m_entities.size()) > maxEntities && depth < maxDepth) {
			Subdivide();
		}
	}
	else {
		for (auto* c : m_childs) {
			c->Insert(entity);
		}
	}
}

void QuadNode::Query(AABB& range, std::vector<ColliderEntry>& results, std::unordered_set<Entity*>& seen){
	if (!m_bounds.overlaps(range)) {
		return;
	}
	
	if (IsLeaf()) {
		for (auto& e : m_entities) {
			if (seen.insert(e.entity).second) {
				results.push_back(e);
			}
		}
	}
	else {
		for (auto& c : m_childs) {
			c->Query(range, results, seen);
		}
	}
}

void QuadNode::Clear(){
	m_entities.clear();
	if (IsLeaf())
		return;
	for (auto& c : m_childs) {
		c->Clear();
	}
}