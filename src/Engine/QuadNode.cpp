#include "QuadNode.h"
#include "Entity.h"

Vector2f QuadNode::GetSize(){
	return m_size;
}

Vector2f QuadNode::GetPosition() {
	return m_pos;
}


QuadNode::QuadNode(float32 x1, float32 y1, float32 x2, float32 y2) {
	m_bounds = { x1, y1, x2, y2 };
}

QuadNode::~QuadNode(){
	for (int8 i = 0; i < 4; i++) {
		delete m_childs[i];
	}
}

void QuadNode::Subdivide(){
	Vector2f mid{ (m_bounds.minX + m_bounds.maxX) * 0.5f, (m_bounds.minY + m_bounds.maxY) * 0.5f };
	
	m_childs[0] = new QuadNode(m_bounds.minX, m_bounds.minY, mid.x, mid.y);
	m_childs[1] = new QuadNode(mid.x, m_bounds.minY, m_bounds.maxX, mid.y);
	m_childs[2] = new QuadNode(m_bounds.minX, mid.y, mid.x, m_bounds.maxY);
	m_childs[3] = new QuadNode(mid.x, mid.y, m_bounds.maxX, m_bounds.maxY);

	for (auto& e : m_entities) {
		for (auto& child : m_childs) {
			if (child->m_bounds.overlaps(e.aabb)) {
				child->m_entities.push_back(e);
			}
		}
	}
	m_entities.clear();
}

bool QuadNode::IsLeaf(){
	return m_childs[0] == nullptr;
}

void QuadNode::Insert(Entity* entity){
	AABB aabb;
	if (entity->GetShape()->GetShape() == gcle::Shapes::Rectangle) {
		Vector2f pos1 = entity->GetPosition(0.f, 0.f);
		Vector2f pos2 = entity->GetPosition(1.f, 1.f);
		aabb = { pos1.x, pos1.y, pos2.x , pos2.y};
	}
	else if (entity->GetShape()->GetShape() == gcle::Shapes::Circle) {
		Vector2f pos1 = entity->GetPosition(0, 0);
		Vector2f pos2 = entity->GetPosition(1.f, 1.f);
		aabb = { pos1.x, pos1.y, pos2.x, pos2.y };
	}

	if (m_bounds.overlaps(aabb))
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

void QuadNode::Query(AABB& range, std::vector<ColliderEntry>& results){
	if (!m_bounds.overlaps(range)) {
		return;
	}
	
	if (IsLeaf()) {
		for (auto& e : m_entities) {
			if (e.aabb.overlaps(range)) {
				results.push_back(e);
			}
		}
	}
	else {
		for (auto& c : m_childs) {
			c->Query(range, results);
		}
	}
}

void QuadNode::Clear(){
	m_entities.clear();
	for (auto& c : m_childs) {
		c->Clear();
	}
}