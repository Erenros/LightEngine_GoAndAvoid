#include "QuadNode.h"
#include "Entity.h"
#include "Collider.h"
#include "Utils.h";

QuadNode::QuadNode(float32 x1, float32 y1, float32 x2, float32 y2, int32 d) {
	m_bounds = { x1, y1, x2, y2 };
	m_depth = d;
}

QuadNode::~QuadNode(){
	if (m_childs[0] == nullptr)
		return;
}

void QuadNode::Subdivide(QuadNodePool& pool){
	Vector2f mid{ (m_bounds.minX + m_bounds.maxX) * 0.5f, (m_bounds.minY + m_bounds.maxY) * 0.5f };
	int32 d = m_depth + 1;

	m_childs[0] = pool.Get(m_bounds.minX, m_bounds.minY, mid.x, mid.y, d);
	m_childs[1] = pool.Get(mid.x, m_bounds.minY, m_bounds.maxX, mid.y, d);
	m_childs[2] = pool.Get(m_bounds.minX, mid.y, mid.x, m_bounds.maxY, d);
	m_childs[3] = pool.Get(mid.x, mid.y, m_bounds.maxX, m_bounds.maxY, d);

	for (auto& e : m_entities) {
		for (auto& child : m_childs) {
			if (child->m_bounds.overlaps(e->GetAABB())) {
				child->m_entities.push_back(e);
			}
		}
	}
	m_entities.clear();

	for (auto& child : m_childs) {
		if (static_cast<int32>(child->m_entities.size()) > maxEntities && child->m_depth < maxDepth) {
			child->Subdivide(pool);
		}
	}
}

bool QuadNode::IsLeaf(){
	return m_childs[0] == nullptr;
}

void QuadNode::Insert(Collider* collider, QuadNodePool& pool){
	AABB aabb = collider->GetAABB();

	if (!m_bounds.overlaps(aabb))
		return;

	if (IsLeaf()) {
		m_entities.push_back(collider);
		if (static_cast<int32>(m_entities.size()) > maxEntities && m_depth < maxDepth) {
			Subdivide(pool);
		}
	}
	else {
		for (auto* c : m_childs) {
			c->Insert(collider, pool);
		}
	}
}

void QuadNode::Query(AABB& range, std::vector<Collider*>& results, std::vector<Collider*>& seen){
	if (!m_bounds.overlaps(range)) {
		return;
	}
	
	if (IsLeaf()) {
		for (auto& e : m_entities) {
			if (!e->GetInQuerySeen()) {
				e->SetInQuerySeen(true);
				seen.push_back(e);
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
	m_childs[0] = nullptr;
	m_childs[1] = nullptr;
	m_childs[2] = nullptr;
	m_childs[3] = nullptr;
}



void QuadNodePool::AddBlock(){
	m_blocks.emplace_back(m_blockSize);
	m_currentBlock = &m_blocks.back();
	m_index = 0;
}

QuadNodePool::QuadNodePool(int32 size): m_blockSize(size){
	AddBlock();
}

QuadNode* QuadNodePool::Get(float32 x, float32 y, float32 x2, float32 y2, int32 depth){
	if (m_index >= static_cast<int32>(m_currentBlock->size())) {
		AddBlock();
	}
	QuadNode* node = &(*m_currentBlock)[m_index++];
	node->m_bounds = {x, y, x2, y2};
	node->m_depth = depth;
	node->m_entities.clear();
	node->m_childs[0] = nullptr;
	node->m_childs[1] = nullptr;
	node->m_childs[2] = nullptr;
	node->m_childs[3] = nullptr;
	return node;
}

void QuadNodePool::Reset(){
	m_index = 0;
	if (m_blocks.size() > 1) {
		m_blocks.resize(1);
	}
	m_currentBlock = &m_blocks[0];
}


