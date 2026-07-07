#include <algorithm>
#include <cmath>
#include <limits>
#include "PhysicsManager.h"
#include "SceneManager.h"
#include "MathGC.h"

#undef min
#undef max

namespace
{
	constexpr float32 PHYSICS_EPSILON = 0.00001f;

	float32 Dot(const Vector2f& a, const Vector2f& b)
	{
		return a.x * b.x + a.y * b.y;
	}

	Vector2f SafeNormal(Vector2f value, Vector2f fallback)
	{
		float32 lengthSquared = (value.x * value.x) + (value.y * value.y);
		if (lengthSquared <= PHYSICS_EPSILON * PHYSICS_EPSILON)
			return fallback;

		return value / std::sqrt(lengthSquared);
	}

	bool TestRectAxis(
		const Vector2f& axis,
		const Vector2f& centerDelta,
		const Vector2f axesA[2],
		const Vector2f axesB[2],
		const Vector2f& extentsA,
		const Vector2f& extentsB,
		float32& minOverlap,
		Vector2f& bestNormal)
	{
		Vector2f normal = SafeNormal(axis, { 1.0f, 0.0f });

		float32 distance = Dot(centerDelta, normal);
		float32 radiusA =
			extentsA.x * std::abs(Dot(axesA[0], normal)) +
			extentsA.y * std::abs(Dot(axesA[1], normal));
		float32 radiusB =
			extentsB.x * std::abs(Dot(axesB[0], normal)) +
			extentsB.y * std::abs(Dot(axesB[1], normal));

		float32 overlap = (radiusA + radiusB) - std::abs(distance);
		if (overlap <= 0.0f)
			return false;

		if (overlap < minOverlap)
		{
			minOverlap = overlap;
			bestNormal = normal * (distance >= 0.0f ? 1.0f : -1.0f); // A -> B
		}

		return true;
	}

	float32 KinematicFactor(Entity* pEntity)
	{
		return pEntity->IsStatic() ? 1.0f : 0.0f;
	}

	bool ShouldBlockMovement(Entity* pEntity, Entity* pOtherEntity)
	{
		// Si les deux objets sont Kinematic, ils sont tous les deux deplacables :
		// on les repousse, mais on ne bloque pas l'input avec CollisionDirection
		// et on ne supprime pas leur vitesse.
		// On bloque uniquement quand l'objet touche un obstacle non Kinematic.
		return pEntity != nullptr &&
			pOtherEntity != nullptr &&
			pEntity->IsStatic() &&
			!pOtherEntity->IsStatic();
	}

	void ApplyBlockingResponse(Collider* pCollider, Collider* pOtherCollider, const Vector2f& correction)
	{
		if (pCollider == nullptr || pOtherCollider == nullptr)
			return;

		Entity* pEntity = pCollider->GetOwner();
		Entity* pOtherEntity = pOtherCollider->GetOwner();

		if (!ShouldBlockMovement(pEntity, pOtherEntity))
			return;

		// La correction indique la direction qui sort l'objet de la collision.
		// On retire seulement la vitesse qui continue a pousser dans l'obstacle,
		// et on pose les flags uniquement contre les objets immobiles.
		pEntity->GetRigidBody().RemoveVelocityAlongNormal(correction);
		pCollider->CollidingOn(correction);
	}
}

PhysicsManager::CollisionFn PhysicsManager::collisionTable[static_cast<int32>(gcle::Shapes::Count) - 1][static_cast<int32>(gcle::Shapes::Count) - 1]
{
	{
		&PhysicsManager::ThrowCheckRectRect,
		&PhysicsManager::ThrowCheckRectCircle,
	},
	{
		&PhysicsManager::ThrowCheckCircleRect,
		&PhysicsManager::ThrowCheckCircleCircle,
	}
};

PhysicsManager::RepulseFn PhysicsManager::repulseTable[static_cast<int32>(RepulseTypes::Count)][static_cast<int32>(RepulseTypes::Count)]
{
	{
		&PhysicsManager::RepulseRectRect,
		&PhysicsManager::RepulseRectCircle,
		&PhysicsManager::RepulseOBB,
	},
	{
		&PhysicsManager::RepulseCircleRect,
		&PhysicsManager::RepulseCircleCircle,
		&PhysicsManager::RepulseOBB,
	},
	{
		&PhysicsManager::RepulseOBB,
		&PhysicsManager::RepulseOBB,
		&PhysicsManager::RepulseOBB
	}
};

PhysicsManager& PhysicsManager::GetInstance()
{
	static PhysicsManager instance;
	return instance;
}

#pragma region EntityHandling

void PhysicsManager::AddEntity(Entity* pEntity)
{
	EntityInfo info = { pEntity, false };
	m_EntitiesToUpdate.push_back(info);
}

void PhysicsManager::RemoveEntity(Entity* pEntity)
{
	for (auto& info : m_EntitiesToUpdate)
	{
		if (info.pEntity->GetId() == pEntity->GetId())
		{
			info.toRemove = true;
		}
	}
}

void PhysicsManager::EntityToRemove(std::vector<EntityInfo>& m_EntitiesToRemove, std::vector<EntityInfo>& m_EntitiesToUpdate)
{
	for (EntityInfo entity : m_EntitiesToRemove) {
		for (int i = (int)m_EntitiesToUpdate.size() - 1; i >= 0; --i)
		{
			if (m_EntitiesToUpdate[i].pEntity == entity.pEntity) {
				m_EntitiesToUpdate.erase(m_EntitiesToUpdate.begin() + i);
			}
		}
	}
	m_EntitiesToRemove.clear();
}

void PhysicsManager::EntityToAdd(std::vector<EntityInfo>& m_EntitiesToAdd, std::vector<EntityInfo>& m_EntitiesToUpdate)
{
	for (EntityInfo entity : m_EntitiesToAdd) {
		m_EntitiesToUpdate.push_back(entity);
	}
	m_EntitiesToAdd.clear();

}

void PhysicsManager::EntityToUpdate(std::vector<Collider*>* activeColliders, std::vector<EntityInfo>& m_EntitiesToUpdate)
{
	for (auto& e : m_EntitiesToUpdate) {
		if (e.toRemove == false)
		{
			if (e.pEntity->IsActiveIn(SceneManager::GetInstance().GetCurrentSceneTag()))
			{
				for (auto& collider : e.pEntity->GetColliders())
				{
					if (collider->IsActive())
						activeColliders->push_back(collider);
				}
			}
		}
	}
}

#pragma endregion

#pragma region Update

void PhysicsManager::Update(float32 dt)
{
	EntityToRemove(m_EntitiesToRemove, m_EntitiesToUpdate);
	EntityToAdd(m_EntitiesToAdd, m_EntitiesToUpdate);

	std::vector<Collider*> activeColliders;

	EntityToUpdate(&activeColliders, m_EntitiesToUpdate);

	const std::string& currentScene = SceneManager::GetInstance().GetCurrentSceneTag();

	if (m_activateQuadTree == true) {
		UpdateQuadTree(activeColliders, dt);
	}

	else {
		UpdateWithoutQuadTree(activeColliders, dt);
	}
}

void PhysicsManager::UpdateQuadTree(std::vector<Collider*> activeColliders, float32 dt)
{
	int32 nbrTest = 0;

	m_pairs.clear();
	m_timeBetweenRegeneration += 1;

	GenerateQuadTree(&activeColliders, dt);

	MakeTreePairs(&activeColliders);
	for (auto& pair : m_pairs) {

		nbrTest += 1;

		HandleCollision(pair, dt);

		PendingCorrections();

		for (int32 i = static_cast<int32>(m_EntitiesToUpdate.size()) - 1; i >= 0; i--)
		{
			if (m_EntitiesToUpdate[i].toRemove)
				m_EntitiesToUpdate.erase(m_EntitiesToUpdate.begin() + i);
		}

	}
	m_pairs.clear();
}

void PhysicsManager::UpdateWithoutQuadTree(std::vector<Collider*> activeColliders, float32 dt)
{
	m_pairs.clear();

	MakePairs(&activeColliders);

	for (auto& pair : m_pairs)
	{

		HandleCollision(pair, dt);

		PendingCorrections();
	}

	for (int32 i = static_cast<int32>(m_EntitiesToUpdate.size()) - 1; i >= 0; i--)
	{
		if (m_EntitiesToUpdate[i].toRemove)
			m_EntitiesToUpdate.erase(m_EntitiesToUpdate.begin() + i);
	}

	m_pairs.clear();
}

#pragma region Helpers

void PhysicsManager::GenerateQuadTree(std::vector<Collider*>* activeColliders, float32 dt)
{
	if (m_timeBetweenRegeneration >= m_frameBetweenQuadTreeRegenerations)
	{
		m_quadTree->Clear();

		for (auto& collider : *activeColliders)
		{
			AABB currentAABB = ComputeCurrentAABB(collider);
			AABB finalAABB = currentAABB;

			if (ShouldUseContinuousCollision(collider))
			{
				AABB futureAABB = ComputePredictedAABB(collider, currentAABB, dt);
				finalAABB = UnionAABB(currentAABB, futureAABB);
			}

			collider->SetAABB(finalAABB);
			m_quadTree->Insert(collider);
		}

		m_timeBetweenRegeneration = 0;
	}
}

void PhysicsManager::PendingCorrections()
{
	for (auto& pair : m_PendingCorrections)
	{
		Entity* pEntity = pair.first;
		Vector2f delta = pair.second;

		Vector2f current = pEntity->GetPosition();
		pEntity->SetPosition(current.x + delta.x, current.y + delta.y);
	}
	m_PendingCorrections.clear();
}

void PhysicsManager::MakePairs(std::vector<Collider*>* activeColliders) {
	for (size_t i = 0; i < activeColliders->size(); ++i) {
		Collider* colliderA = (*activeColliders)[i];
		for (size_t j = i + 1; j < activeColliders->size(); ++j) {
			Collider* colliderB = (*activeColliders)[j];
			bool greater = colliderA < colliderB;
			bool notTheSameEntity = colliderA->GetOwner()->GetId() != colliderB->GetOwner()->GetId();
			bool onSameLayer = (Mask::AreOnASameLayer<int32>(*colliderA->GetOwner()->GetCollisionLayer(), *colliderB->GetOwner()->GetCollisionLayer()));

			if (greater && notTheSameEntity && onSameLayer) {
				m_pairs.push_back({ colliderA, colliderB });
			}
		}
	}
}

void PhysicsManager::MakeTreePairs(std::vector<Collider*>* activeColliders)
{
	for (auto& colliderA : *activeColliders) {

		const auto& candidates = m_quadTree->Query(colliderA);

		for (auto& colliderB : candidates) {
			bool greater = colliderA < colliderB;
			bool notTheSameEntity = colliderA->GetOwner()->GetId() != colliderB->GetOwner()->GetId();
			bool onSameLayer = (Mask::AreOnASameLayer<int32>(*colliderA->GetOwner()->GetCollisionLayer(), *colliderB->GetOwner()->GetCollisionLayer()));

			if (greater && notTheSameEntity && onSameLayer) {
				m_pairs.push_back({ colliderA, colliderB });
			}
		}
	}
}

void PhysicsManager::HandleCollision(std::pair<Collider*, Collider*> collider, float32 dt)
{
	Collider* colliderA = collider.first;
	Collider* colliderB = collider.second;

	Entity* entityA = colliderA->GetOwner();
	Entity* entityB = colliderB->GetOwner();

	bool coliding = IsColliding(colliderA, colliderB); 

	if (coliding)
	{
		if (entityA->HasCollider() && entityB->HasCollider())
		{
			ThrowRepulse(colliderA, colliderB);
		}

		if (!entityA->CollidingEntity.contains(entityB->GetId()))
		{
			entityA->OnCollisionEnter(entityB);
			entityA->CollidingEntity.insert({ entityB->GetId(), entityB });

			entityB->OnCollisionEnter(entityA);
			entityB->CollidingEntity.insert({ entityA->GetId(), entityA });
		}
		else
		{
			entityA->OnCollision(entityB);
			entityB->OnCollision(entityA);
		}

		return;
	}

	bool needAntiTunneling =
		ShouldUseContinuousCollision(colliderA) || ShouldUseContinuousCollision(colliderB);

	if (needAntiTunneling)
	{
		if (TryResolveContinuousCollision(colliderA, colliderB, dt))
		{
			return;
		}
	}

	if (entityA->CollidingEntity.contains(entityB->GetId()))
	{
		entityA->OnCollisionExit(entityB);
		entityA->CollidingEntity.erase(entityB->GetId());

		entityB->OnCollisionExit(entityA);
		entityB->CollidingEntity.erase(entityA->GetId());
	}
}

void PhysicsManager::AccumulateCorrection(Entity* pEntity, Vector2f delta)
{
	m_PendingCorrections[pEntity] += delta;
}

bool PhysicsManager::IsColliding(Collider* pCollider1, Collider* pCollider2)
{
	auto* shapeA = pCollider1->GetShape();
	auto* shapeB = pCollider2->GetShape();

	if (shapeA->GetShape() == gcle::Shapes::Triangle || shapeB->GetShape() == gcle::Shapes::Triangle)
	{
		return false;
	}

	int32 typeA = static_cast<int32>(shapeA->GetShape()) - 1;
	int32 typeB = static_cast<int32>(shapeB->GetShape()) - 1;
	int32 maxType = static_cast<int32>(gcle::Shapes::Count) - 1;

	if (typeA < 0 || typeB < 0 || typeA >= maxType || typeB >= maxType)
		return false;

	return (this->*collisionTable[typeA][typeB])(shapeA, shapeB);
}

bool PhysicsManager::IsInside(Shape* renderShape, Vector2f positionToCheck)
{
	if (renderShape == nullptr)
		return false;

	switch (renderShape->GetShape())
	{
	case gcle::Shapes::Rectangle:
	{
		gcle::Rectangle* pRect = static_cast<gcle::Rectangle*>(renderShape);
		return
		{
			positionToCheck.x >= pRect->GetPosition().x - pRect->GetWidth() * 0.5f &&
			positionToCheck.x <= pRect->GetPosition().x + pRect->GetWidth() * 0.5f &&
			positionToCheck.y >= pRect->GetPosition().y - pRect->GetHeight() * 0.5f &&
			positionToCheck.y <= pRect->GetPosition().y + pRect->GetHeight() * 0.5f
		};
	}
	case gcle::Shapes::Circle:
	{
		gcle::Circle* pCircle = static_cast<gcle::Circle*>(renderShape);
		return pCircle->GetPosition().GetDistance(positionToCheck) <= pCircle->GetRadius();
	}
	default:
		break;
	}

	return false;
}

#pragma endregion

#pragma region ContinuousCollision

ContinuousCollisionHit PhysicsManager::SweepColliderAgainstAABB(Collider* movingCollider, Collider* obstacleCollider, float64 dt)
{
	if (movingCollider == nullptr || obstacleCollider == nullptr)
		return {};

	Entity* movingEntity = movingCollider->GetOwner();
	if (movingEntity == nullptr || !movingEntity->HasCollider())
		return {};

	AABB movingAABB = ComputeCurrentAABB(movingCollider);
	AABB obstacleAABB = ComputeCurrentAABB(obstacleCollider);

	float32 movingWidth = movingAABB.maxX - movingAABB.minX;
	float32 movingHeight = movingAABB.maxY - movingAABB.minY;

	if (movingWidth <= PHYSICS_EPSILON || movingHeight <= PHYSICS_EPSILON)
		return {};

	Vector2f movingHalfSize{
		movingWidth * 0.5f,
		movingHeight * 0.5f
	};

	AABB expandedObstacleAABB = ExpandAABB(obstacleAABB, movingHalfSize);

	Vector2f start = movingCollider->GetShape()->GetPosition(0.5f, 0.5f);
	Vector2f velocity = movingEntity->GetRigidBody().GetVelocity();
	Vector2f end = start + velocity * static_cast<float32>(dt);

	return SegmentAABBIntersection(start, end, expandedObstacleAABB);
}

ContinuousCollisionHit PhysicsManager::SegmentAABBIntersection(Vector2f start, Vector2f end, const AABB& target)
{
	m_entryTime = 0.0f;
	m_exitTime = 1.0f;
	m_entryNormal = { 0.0f, 0.0f };

	Vector2f delta = end - start;

	if (std::abs(delta.x) <= PHYSICS_EPSILON && std::abs(delta.y) <= PHYSICS_EPSILON)
		return {};

	if (!TestAxis(start.x, delta.x, target.minX, target.maxX, { -1.0f, 0.0f }, { 1.0f, 0.0f }))
		return {};

	if (!TestAxis(start.y, delta.y, target.minY, target.maxY, { 0.0f, -1.0f }, { 0.0f, 1.0f }))
		return {};

	if (m_entryTime < 0.0f || m_entryTime > 1.0f)
		return {};

	ContinuousCollisionHit hit;
	hit.hit = true;
	hit.time = m_entryTime;
	hit.normal = m_entryNormal;

	return hit;
}

AABB PhysicsManager::ComputeCurrentAABB(Collider* collider)
{
	if (collider == nullptr || collider->GetShape() == nullptr)
		return {};

	gcle::Shape* shape = collider->GetShape();

	if (shape->GetShape() == gcle::Shapes::Circle)
	{
		Vector2f min = shape->GetPosition(0.0f, 0.0f);
		Vector2f max = shape->GetPosition(1.0f, 1.0f);

		return {
			std::min(min.x, max.x),
			std::min(min.y, max.y),
			std::max(min.x, max.x),
			std::max(min.y, max.y)
		};
	}

	Degrees angle = shape->GetRotation();

	if (static_cast<int32>(angle) % 180 == 0)
	{
		Vector2f pos1 = shape->GetPosition(0.0f, 0.0f);
		Vector2f pos2 = shape->GetPosition(1.0f, 1.0f);

		return {
			std::min(pos1.x, pos2.x),
			std::min(pos1.y, pos2.y),
			std::max(pos1.x, pos2.x),
			std::max(pos1.y, pos2.y)
		};
	}

	return GetRotatedAABB(
		shape->GetPosition(),
		{
			shape->GetWidth(),
			shape->GetHeight()
		},
		angle * DEG_TO_RAD
	);
}

AABB PhysicsManager::ComputePredictedAABB(Collider* collider, const AABB& currentAABB, float32 dt)
{
	Entity* entity = collider->GetOwner();

	if (entity == nullptr || !entity->IsRigidBody())
		return currentAABB;

	Vector2f motion = entity->GetRigidBody().GetVelocity() * dt;
	return TranslateAABB(currentAABB, motion);
}

bool PhysicsManager::TestAxis(float32 startAxis, float32 deltaAxis, float32 minAxis, float32 maxAxis, Vector2f negativeNormal, Vector2f positiveNormal)
{
	if (std::abs(deltaAxis) <= PHYSICS_EPSILON)
	{
		return startAxis >= minAxis && startAxis <= maxAxis;
	}

	float32 invDelta = 1.0f / deltaAxis;
	float32 axisEntry = (minAxis - startAxis) * invDelta;
	float32 axisExit = (maxAxis - startAxis) * invDelta;
	Vector2f axisNormal = negativeNormal;

	if (axisEntry > axisExit)
	{
		std::swap(axisEntry, axisExit);
		axisNormal = positiveNormal;
	}

	if (axisEntry > m_entryTime)
	{
		m_entryTime = axisEntry;
		m_entryNormal = axisNormal;
	}

	m_exitTime = std::min(m_exitTime, axisExit);
	return m_entryTime <= m_exitTime;
}

AABB PhysicsManager::UnionAABB(const AABB& a, const AABB& b)
{
	return {
		std::min(a.minX, b.minX),
		std::min(a.minY, b.minY),
		std::max(a.maxX, b.maxX),
		std::max(a.maxY, b.maxY)
	};
}

AABB PhysicsManager::ExpandAABB(const AABB& aabb, Vector2f amount)
{
	return {
		aabb.minX - amount.x,
		aabb.minY - amount.y,
		aabb.maxX + amount.x,
		aabb.maxY + amount.y
	};
}

bool PhysicsManager::ShouldUseContinuousCollision(Collider* collider) const
{
	if (collider == nullptr)
		return false;

	Entity* entity = collider->GetOwner();

	if (entity == nullptr)
		return false;

	if (!entity->HasCollider())
		return false;

	return entity->GetRigidBody().UseContinuousCollision();
}

AABB PhysicsManager::TranslateAABB(const AABB& aabb, Vector2f delta)
{
	return {
		aabb.minX + delta.x,
		aabb.minY + delta.y,
		aabb.maxX + delta.x,
		aabb.maxY + delta.y
	};
}

bool PhysicsManager::TryResolveContinuousCollision(Collider* colliderA, Collider* colliderB, float64 dt)
{
	if (ShouldUseContinuousCollision(colliderA))
	{
		ContinuousCollisionHit hitA = SweepColliderAgainstAABB(colliderA, colliderB, dt);

		if (hitA.hit)
		{
			ApplyContinuousCollisionResponse(colliderA, hitA, dt);
			return true;
		}
	}

	if (ShouldUseContinuousCollision(colliderB))
	{
		ContinuousCollisionHit hitB = SweepColliderAgainstAABB(colliderB, colliderA, dt);

		if (hitB.hit)
		{
			ApplyContinuousCollisionResponse(colliderB, hitB, dt);
			return true;
		}
	}

	return false;
}

void PhysicsManager::ApplyContinuousCollisionResponse(Collider* movingCollider, const ContinuousCollisionHit& hit, float64 dt)
{
	if (movingCollider == nullptr || !hit.hit)
		return;

	Entity* movingEntity = movingCollider->GetOwner();
	if (movingEntity == nullptr || !movingEntity->HasCollider())
		return;

	Vector2f start = movingCollider->GetShape()->GetPosition(0.5f, 0.5f);
	Vector2f velocity = movingEntity->GetRigidBody().GetVelocity();
	Vector2f motion = velocity * static_cast<float32>(dt);

	float32 safeTime = std::max(0.0f, hit.time - 0.001f);
	Vector2f safeCenter = start + motion * safeTime;

	Vector2f ownerCenter = movingEntity->GetPosition();
	Vector2f colliderCenter = movingCollider->GetShape()->GetPosition(0.5f, 0.5f);
	Vector2f ownerToCollider = colliderCenter - ownerCenter;
	Vector2f safeOwnerCenter = safeCenter - ownerToCollider;

	movingEntity->SetPosition(safeOwnerCenter.x, safeOwnerCenter.y);
	movingEntity->GetRigidBody().RemoveVelocityAlongNormal(hit.normal);
	movingCollider->CollidingOn(hit.normal);
}

#pragma endregion

#pragma endregion

#pragma region Checks

#pragma region ThrowChecks

bool PhysicsManager::ThrowCheckRectRect(gcle::Shape* a, gcle::Shape* b)
{
	int16 angleA = static_cast<int16>(a->GetTransform()->GetDegAngle());
	int16 angleB = static_cast<int16>(b->GetTransform()->GetDegAngle());


	if (angleA != 0) {
		if (angleB != 0)
			return CheckOBBOBBCollision(static_cast<gcle::Rectangle*>(a), static_cast<gcle::Rectangle*>(b));
		else
			return CheckOBBAABBCollision(static_cast<gcle::Rectangle*>(a), static_cast<gcle::Rectangle*>(b));
	}
	else if (angleB != 0) {
		bool hit = CheckOBBAABBCollision(static_cast<gcle::Rectangle*>(b), static_cast<gcle::Rectangle*>(a));
		if (hit) {
			m_colDatas.orientation = -m_colDatas.orientation;
		}
		return hit;
	}


	return CheckAABBAABBCollision(static_cast<gcle::Rectangle*>(a), static_cast<gcle::Rectangle*>(b));
}

bool PhysicsManager::ThrowCheckCircleCircle(gcle::Shape* a, gcle::Shape* b)
{
	return CheckCircleCircleCollision(static_cast<gcle::Circle*>(a), static_cast<gcle::Circle*>(b));
}

bool PhysicsManager::ThrowCheckRectCircle(gcle::Shape* a, gcle::Shape* b)
{
	int16 angle = static_cast<int16>(a->GetTransform()->GetDegAngle()) % 180;

	if (angle != 0)
	{
		return CheckOBBCircleCollision(static_cast<gcle::Rectangle*>(a), static_cast<gcle::Circle*>(b));
	}
	else
	{
		return CheckAABBCircleCollision(static_cast<gcle::Rectangle*>(a), static_cast<gcle::Circle*>(b));
	}
}

bool PhysicsManager::ThrowCheckCircleRect(gcle::Shape* a, gcle::Shape* b)
{
	int16 angle = static_cast<int16>(b->GetTransform()->GetDegAngle()) % 180;
	if (angle != 0) {
		bool hit = CheckOBBCircleCollision(static_cast<gcle::Rectangle*>(b), static_cast<gcle::Circle*>(a));
		if (hit)
			m_colDatas.orientation = -m_colDatas.orientation;
		return hit;

	}
	return CheckAABBCircleCollision(static_cast<gcle::Rectangle*>(b), static_cast<gcle::Circle*>(a));
}

#pragma endregion

#pragma region ActualsChecks

bool PhysicsManager::CheckAABBAABBCollision(gcle::Rectangle* pRect1, gcle::Rectangle* pRect2)
{
	float32 x1 = pRect1->GetPosition(0, 0).x;
	float32 y1 = pRect1->GetPosition(0, 0).y;
	float32 w1 = pRect1->GetWidth();
	float32 h1 = pRect1->GetHeight();

	float32 x2 = pRect2->GetPosition(0, 0).x;
	float32 y2 = pRect2->GetPosition(0, 0).y;
	float32 w2 = pRect2->GetWidth();
	float32 h2 = pRect2->GetHeight();


	return x1 + w1 >= x2 &&
		x1 <= x2 + w2 &&
		y1 + h1 >= y2 &&
		y1 <= y2 + h2;
}

bool PhysicsManager::CheckAABBCircleCollision(gcle::Rectangle* pRect, gcle::Circle* pCircle)
{
	float32 rx = pRect->GetPosition(0.0f, 0.0f).x;
	float32 ry = pRect->GetPosition(0.0f, 0.0f).y;
	float32 rw = pRect->GetWidth();
	float32 rh = pRect->GetHeight();

	float32 cx = pCircle->GetPosition().x;
	float32 cy = pCircle->GetPosition().y;

	float32 testX = cx;
	float32 testY = cy;

	if (cx < rx)
		testX = rx;
	else if (cx > rx + rw)
		testX = rx + rw;

	if (cy < ry)
		testY = ry;
	else if (cy > ry + rh)
		testY = ry + rh;

	Vector2f test({ testX, testY });

	float32 dist = pCircle->GetPosition().GetDistance(test);

	return (dist <= pCircle->GetRadius());
}

bool PhysicsManager::CheckCircleCircleCollision(gcle::Circle* pCircle1, gcle::Circle* pCircle2)
{
	Vector2f pos1 = pCircle1->GetPosition();
	Vector2f pos2 = pCircle2->GetPosition();
	float32 distance = pos1.GetDistance(pos2);
	return (distance <= (pCircle1->GetRadius() + pCircle2->GetRadius()));
}

bool PhysicsManager::CheckOBBAABBCollision(gcle::Rectangle* pRect1, gcle::Rectangle* pRect2) {

	Vector2f axes[2];

	float32 radRotation = pRect1->GetTransform()->GetRadAngle();
	axes[0] = { std::cos(radRotation), std::sin(radRotation) };
	axes[1] = { -std::sin(radRotation), std::cos(radRotation) };


	//OBB values
	Vector2f obbExtents{ pRect1->GetWidth() / 2, pRect1->GetHeight() / 2 };

	//AABB values
	Vector2f aabbExtents{ pRect2->GetWidth() / 2, pRect2->GetHeight() / 2 };

	Vector2f T = pRect2->GetPosition() - pRect1->GetPosition();

	Matrix relativeRotation(2, 2);
	Matrix absoluteRotation(2, 2);


	const float64 Epsilon = 1e-16;

	for (int i = 0; i < 2; i++) {
		relativeRotation.m_matrix[0][i] = axes[i].x;
		relativeRotation.m_matrix[1][i] = axes[i].y;

		for (int j = 0; j < 2; j++) {
			absoluteRotation.m_matrix[i][j] = std::abs(relativeRotation.m_matrix[j][i]) + static_cast<float32>(Epsilon);
		}
	}


	float ra = 0.f, rb = 0.f;

	float32 minOverlap = std::numeric_limits<float32>::max();
	Vector2f collisionNormal;


	ra = aabbExtents.x;
	rb = obbExtents.x * absoluteRotation.m_matrix[0][0] + obbExtents.y * absoluteRotation.m_matrix[0][1];
	float32 overlapX = (ra + rb) - std::abs(T.x);
	if (overlapX <= 0.f)
		return false;

	if (overlapX < minOverlap) {
		minOverlap = overlapX;
		collisionNormal = { (T.x > 0.f ? 1.f : -1.f), 0.f };
	}

	ra = aabbExtents.y;
	rb = obbExtents.x * absoluteRotation.m_matrix[1][0] + obbExtents.y * absoluteRotation.m_matrix[1][1];
	float32 overlapY = (ra + rb) - std::abs(T.y);
	if (overlapY <= 0.f)
		return false;

	if (overlapY < minOverlap) {
		minOverlap = overlapY;
		collisionNormal = { 0.f, (T.y > 0.f ? 1.f : -1.f) };
	}




	ra = aabbExtents.x * absoluteRotation.m_matrix[0][0] + aabbExtents.y * absoluteRotation.m_matrix[0][1];
	rb = obbExtents.x;
	float32 t_obbX = T.x * relativeRotation.m_matrix[0][0] + T.y * relativeRotation.m_matrix[1][0];
	float32 overlapObbX = (ra + rb) - std::abs(t_obbX);
	if (overlapObbX <= 0.f)
		return false;

	if (overlapObbX < minOverlap) {
		minOverlap = overlapObbX;
		float32 sign = (t_obbX > 0.f) ? 1.f : -1.f;
		collisionNormal = { axes[0].x * sign, axes[0].y * sign };
	}

	ra = aabbExtents.x * absoluteRotation.m_matrix[1][0] + aabbExtents.y * absoluteRotation.m_matrix[1][1];
	rb = obbExtents.y;
	float32 t_obbY = T.x * relativeRotation.m_matrix[0][1] + T.y * relativeRotation.m_matrix[1][1];
	float32 overlapObbY = (ra + rb) - std::abs(t_obbY);
	if (overlapObbY <= 0.f)
		return false;

	if (overlapObbY < minOverlap) {
		minOverlap = overlapObbY;
		float32 sign = (t_obbY > 0.f) ? 1.f : -1.f;
		collisionNormal = { axes[1].x * sign, axes[1].y * sign };
	}

	m_colDatas.orientation = collisionNormal;
	m_colDatas.penetration = minOverlap;

	return true;
}

bool PhysicsManager::CheckOBBOBBCollision(gcle::Rectangle* pRect1, gcle::Rectangle* pRect2) {
	Vector2f axes1[2];
	Vector2f axes2[2];

	float32 radRotation1 = pRect1->GetTransform()->GetRadAngle();
	float32 radRotation2 = pRect2->GetTransform()->GetRadAngle();

	axes1[0] = { std::cos(radRotation1), std::sin(radRotation1) };
	axes1[1] = { -std::sin(radRotation1), std::cos(radRotation1) };

	axes2[0] = { std::cos(radRotation2), std::sin(radRotation2) };
	axes2[1] = { -std::sin(radRotation2), std::cos(radRotation2) };


	Vector2f extents1{ pRect1->GetWidth() / 2, pRect1->GetHeight() / 2 };
	Vector2f extents2{ pRect2->GetWidth() / 2, pRect2->GetHeight() / 2 };

	Vector2f T = pRect2->GetPosition() - pRect1->GetPosition();

	Matrix relativeRotation(2, 2);
	Matrix absoluteRotation(2, 2);

	const float64 Epsilon = 1e-16;

	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 2; j++) {
			relativeRotation.m_matrix[j][i] = axes1[i].x * axes2[j].x + axes1[i].y * axes2[j].y;
			absoluteRotation.m_matrix[i][j] = std::abs(relativeRotation.m_matrix[j][i]) + static_cast<float32>(Epsilon);
		}
	}


	float32 minOverlap = std::numeric_limits<float32>::max();
	Vector2f collisionNormal;


	float32 ra = 0.f, rb = 0.f;
	float32 sign;


	ra = extents1.x;
	rb = extents2.x * absoluteRotation.m_matrix[0][0] + extents2.y * absoluteRotation.m_matrix[0][1];
	float32 t1_X = T.x * axes1[0].x + T.y * axes1[0].y;
	float32 overlap = (ra + rb) - std::abs(t1_X);
	if (overlap <= 0.f)
		return false;

	if (overlap < minOverlap) {
		minOverlap = overlap;
		sign = (t1_X > 0.f) ? 1.f : -1.f;
		collisionNormal = { axes1[0].x * sign, axes1[0].y * sign };
	}

	ra = extents1.y;
	rb = extents2.x * absoluteRotation.m_matrix[1][0] + extents2.y * absoluteRotation.m_matrix[1][1];
	float32 t1_Y = T.x * axes1[1].x + T.y * axes1[1].y;
	overlap = (ra + rb) - std::abs(t1_Y);
	if (overlap <= 0.f)
		return false;

	if (overlap < minOverlap) {
		minOverlap = overlap;
		sign = (t1_Y > 0.f) ? 1.f : -1.f;
		collisionNormal = { axes1[1].x * sign, axes1[1].y * sign };
	}

	ra = extents1.x * absoluteRotation.m_matrix[0][0] + extents1.y * absoluteRotation.m_matrix[1][0];
	rb = extents2.x;
	float32 t2_X = T.x * axes2[0].x + T.y * axes2[0].y;
	overlap = (ra + rb) - std::abs(t2_X);
	if (overlap <= 0.f)
		return false;

	if (overlap < minOverlap) {
		minOverlap = overlap;
		sign = (t2_X > 0.f) ? 1.f : -1.f;
		collisionNormal = { axes2[0].x * sign, axes2[0].y * sign };
	}


	ra = extents1.x * absoluteRotation.m_matrix[0][1] + extents1.y * absoluteRotation.m_matrix[1][1];
	rb = extents2.y;
	float32 t2_Y = T.x * axes2[1].x + T.y * axes2[1].y;
	overlap = (ra + rb) - std::abs(t2_Y);
	if (overlap <= 0.f)
		return false;

	if (overlap < minOverlap) {
		minOverlap = overlap;
		sign = (t2_Y > 0.f) ? 1.f : -1.f;
		collisionNormal = { axes2[1].x * sign, axes2[1].y * sign };
	}

	m_colDatas.penetration = minOverlap;
	m_colDatas.orientation = collisionNormal;

	return true;
}

bool PhysicsManager::CheckOBBCircleCollision(gcle::Rectangle* pRect, gcle::Circle* pCircle) {

	Vector2f rectPos = pRect->GetPosition();
	Vector2f extents{ pRect->GetWidth() / 2 , pRect->GetHeight() / 2 };
	float32 angle = pRect->GetTransform()->GetRadAngle();

	Vector2f circlePos = pCircle->GetPosition();
	float32 radius = pCircle->GetRadius();

	Vector2f T = circlePos - rectPos;

	float32 cos = std::cos(angle);
	float32 sin = std::sin(angle);

	Vector2f localCirclePos;
	localCirclePos.x = T.x * cos + T.y * sin;
	localCirclePos.y = -T.x * sin + T.y * cos;

	Vector2f closestPoint;
	closestPoint.x = std::max(-extents.x, std::min(localCirclePos.x, extents.x));
	closestPoint.y = std::max(-extents.y, std::min(localCirclePos.y, extents.y));

	float32 deltaX = localCirclePos.x - closestPoint.x;
	float32 deltaY = localCirclePos.y - closestPoint.y;

	float32 distanceCarree = (deltaX * deltaX) + (deltaY * deltaY);

	if (distanceCarree > (radius * radius))
		return false;

	float32 distance = (distanceCarree == 0.f) ? 0.f : std::sqrt(distanceCarree);
	Vector2f localNormal;
	m_colDatas.penetration = radius - distance;

	if (distance == 0.f) {
		localNormal = { 1.f, 1.f };
		m_colDatas.penetration = radius;
	}
	else {
		localNormal = { deltaX / distance, deltaY / distance };
	}

	Vector2f worldNormal;
	worldNormal.x = localNormal.x * cos - localNormal.y * sin;
	worldNormal.y = localNormal.x * sin + localNormal.y * cos;

	m_colDatas.orientation = worldNormal;

	if (distanceCarree <= (radius * radius)) {
		return true;
	}
	return false;

}

#pragma endregion

#pragma endregion

#pragma region Repulse

#pragma region ThrowRepulse

void PhysicsManager::ThrowRepulse(Collider* pCollider1, Collider* pCollider2)
{
	auto* shapeA = pCollider1->GetShape();
	auto* shapeB = pCollider2->GetShape();

	m_pCurrentColliderA = pCollider1;
	m_pCurrentColliderB = pCollider2;

	RepulseTypes typeA;
	RepulseTypes typeB;

	switch (shapeA->GetShape()) {
	case gcle::Shapes::Circle:
		typeA = RepulseTypes::Circle;
		break;
	case gcle::Shapes::Rectangle:
		if (static_cast<int32>(shapeA->GetTransform()->GetDegAngle()) % 180 != 0)
			typeA = RepulseTypes::OOB;
		else
			typeA = RepulseTypes::AABB;
		break;
	case gcle::Shapes::Triangle:
		return;
		break;
	}

	switch (shapeB->GetShape()) {
	case gcle::Shapes::Circle:
		typeB = RepulseTypes::Circle;
		break;
	case gcle::Shapes::Rectangle:
		if (static_cast<int32>(shapeB->GetTransform()->GetDegAngle()) % 180 != 0)
			typeB = RepulseTypes::OOB;
		else
			typeB = RepulseTypes::AABB;
		break;
	case gcle::Shapes::Triangle:
		return;
		break;
	}


	(this->*repulseTable[static_cast<int32>(typeA)][static_cast<int32>(typeB)])(pCollider1, pCollider2);
}

#pragma endregion

#pragma region ActualRepulse

void PhysicsManager::RepulseRectRect(Collider* colA, Collider* colB)
{
	Entity* a = colA->GetOwner();
	Entity* b = colB->GetOwner();

	gcle::Rectangle* pRect1 = static_cast<gcle::Rectangle*>(colA->GetShape());
	gcle::Rectangle* pRect2 = static_cast<gcle::Rectangle*>(colB->GetShape());

	float32 x1 = pRect1->GetPosition(0.0f, 0.0f).x;
	float32 y1 = pRect1->GetPosition(0.0f, 0.0f).y;
	float32 w1 = pRect1->GetWidth();
	float32 h1 = pRect1->GetHeight();

	float32 x2 = pRect2->GetPosition(0.0f, 0.0f).x;
	float32 y2 = pRect2->GetPosition(0.0f, 0.0f).y;
	float32 w2 = pRect2->GetWidth();
	float32 h2 = pRect2->GetHeight();

	float32 overlapX = std::min(x1 + w1, x2 + w2) - std::max(x1, x2);
	float32 overlapY = std::min(y1 + h1, y2 + h2) - std::max(y1, y2);

	if (overlapX <= 0.0f || overlapY <= 0.0f)
		return;

	Vector2f delta1 = { 0.f, 0.f };
	Vector2f delta2 = { 0.f, 0.f };

	float32 correctionMultiplyer = GetRepulseCorrectionMultiplyer(colA, colB);

	if (overlapX < overlapY)
	{
		float32 correction = overlapX * correctionMultiplyer;

		if (x1 < x2)
		{
			delta1.x -= correction * a->IsStatic();
			delta2.x += correction * b->IsStatic();

		}
		else
		{
			delta1.x += correction * a->IsStatic();
			delta2.x -= correction * b->IsStatic();

		}

		if (!colA->GetOwner()->IsStatic() || !colB->GetOwner()->IsStatic())
		{
			a->GetRigidBody().ZeroVelocityX();
			b->GetRigidBody().ZeroVelocityX();
		}
	}
	else
	{
		float32 correction = overlapY * correctionMultiplyer;

		if (y1 < y2)
		{
			delta1.y -= correction * a->IsStatic();
			delta2.y += correction * b->IsStatic();

		}
		else
		{
			delta1.y += correction * a->IsStatic();
			delta2.y -= correction * b->IsStatic();

		}

		if (!a->IsStatic() || !b->IsStatic())
		{
			a->GetRigidBody().ZeroVelocityY();
			b->GetRigidBody().ZeroVelocityY();
		}
	}

	ApplyBlockingResponse(colA, colB, delta1);
	ApplyBlockingResponse(colB, colA, delta2);

	AccumulateCorrection(a, delta1);
	AccumulateCorrection(b, delta2);

}

void PhysicsManager::RepulseCircleCircle(Collider* colA, Collider* colB)
{
	gcle::Shape* a = colA->GetShape();
	gcle::Shape* b = colB->GetShape();

	Vector2f distance = a->GetPosition(0.5f, 0.5f) - b->GetPosition(0.5f, 0.5f);
	float32 sqrLength = (distance.x * distance.x) + (distance.y * distance.y);
	float32 length = std::sqrt(sqrLength);

	float32 radius1 = a->GetRadius();
	float32 radius2 = b->GetRadius();
	float32 penetration = (radius1 + radius2) - length;

	if (penetration <= 0.0f)
		return;

	Vector2f normal = SafeNormal(distance, { 1.0f, 0.0f }); // direction B -> A
	Vector2f translation = normal * penetration * GetRepulseCorrectionMultiplyer(colA, colB);

	Vector2f delta1 = translation * KinematicFactor(colA->GetOwner());
	Vector2f delta2 = -translation * KinematicFactor(colB->GetOwner());

	AccumulateCorrection(colA->GetOwner(), delta1);
	AccumulateCorrection(colB->GetOwner(), delta2);

	ApplyBlockingResponse(colA, colB, delta1);
	ApplyBlockingResponse(colB, colA, delta2);
}

void PhysicsManager::RepulseRectCircle(Collider* colA, Collider* colB)
{
	gcle::Shape* a = colA->GetShape();
	gcle::Shape* b = colB->GetShape();

	gcle::Rectangle* pRect = static_cast<gcle::Rectangle*>(a);
	gcle::Circle* pCircle = static_cast<gcle::Circle*>(b);

	float32 rx = pRect->GetPosition(0.0f, 0.0f).x;
	float32 ry = pRect->GetPosition(0.0f, 0.0f).y;
	float32 rw = pRect->GetWidth();
	float32 rh = pRect->GetHeight();

	Vector2f circlePos = pCircle->GetPosition(0.5f, 0.5f);
	Vector2f rectPos = pRect->GetPosition(0.5f, 0.5f);

	float32 nearestX = std::max(rx, std::min(circlePos.x, rx + rw));
	float32 nearestY = std::max(ry, std::min(circlePos.y, ry + rh));
	Vector2f nearest({ nearestX, nearestY });

	Vector2f delta = circlePos - nearest;
	float32 dist = delta.x * delta.x + delta.y * delta.y;

	float32 correctionMultiplyer = GetRepulseCorrectionMultiplyer(colA, colB);

	if (dist == 0.0f)
	{
		float32 overlapL = circlePos.x - rx;
		float32 overlapR = (rx + rw) - circlePos.x;
		float32 overlapT = circlePos.y - ry;
		float32 overlapB = (ry + rh) - circlePos.y;

		float32 minOverlap = std::min({ overlapL, overlapR, overlapT, overlapB });
		Vector2f newPos = circlePos;
		Vector2f normal({ 0.0f, 0.0f });

		if (minOverlap == overlapL) { newPos.x = rx - pCircle->GetRadius();       normal = { -1.0f,  0.0f }; }
		else if (minOverlap == overlapR) { newPos.x = rx + rw + pCircle->GetRadius();  normal = { 1.0f,  0.0f }; }
		else if (minOverlap == overlapT) { newPos.y = ry - pCircle->GetRadius();       normal = { 0.0f, -1.0f }; }
		else { newPos.y = ry + rh + pCircle->GetRadius(); normal = { 0.0f,  1.0f }; }

		Vector2f translation = (newPos - circlePos) * correctionMultiplyer;
		Vector2f deltaRect = -translation * KinematicFactor(colA->GetOwner());
		Vector2f deltaCircle = translation * KinematicFactor(colB->GetOwner());

		ApplyBlockingResponse(colA, colB, deltaRect);
		ApplyBlockingResponse(colB, colA, deltaCircle);

		AccumulateCorrection(colA->GetOwner(), deltaRect);
		AccumulateCorrection(colB->GetOwner(), deltaCircle);

		return;
	}

	float32 length = std::sqrt(dist);
	Vector2f normal = delta / length;
	float32  overlap = (pCircle->GetRadius() - length) * correctionMultiplyer;

	Vector2f translation = normal * overlap;
	Vector2f deltaRect = -translation * KinematicFactor(colA->GetOwner());
	Vector2f deltaCircle = translation * KinematicFactor(colB->GetOwner());

	ApplyBlockingResponse(colA, colB, deltaRect);
	ApplyBlockingResponse(colB, colA, deltaCircle);

	AccumulateCorrection(colA->GetOwner(), deltaRect);
	AccumulateCorrection(colB->GetOwner(), deltaCircle);
}

void PhysicsManager::RepulseCircleRect(Collider* colA, Collider* colB)
{
	std::swap(m_pCurrentColliderA, m_pCurrentColliderB);
	RepulseRectCircle(colB, colA);
	std::swap(m_pCurrentColliderA, m_pCurrentColliderB);
}

void PhysicsManager::RepulseOBB(Collider* colA, Collider* colB)
{
	gcle::Shape* a = colA->GetShape();
	gcle::Shape* b = colB->GetShape();

	Vector2f normal = SafeNormal(m_colDatas.orientation, { 1.0f, 0.0f });
	float32 penetration = std::max(static_cast<float32>(m_colDatas.penetration), 0.0f);

	if (penetration <= 0.0f)
		return;

	Vector2f correction = normal * penetration * GetRepulseCorrectionMultiplyer(colA, colB);

	Vector2f deltaA = -correction * KinematicFactor(colA->GetOwner());
	Vector2f deltaB = correction * KinematicFactor(colB->GetOwner());

	ApplyBlockingResponse(colA, colB, deltaA);
	ApplyBlockingResponse(colB, colA, deltaB);

	AccumulateCorrection(colA->GetOwner(), deltaA);
	AccumulateCorrection(colB->GetOwner(), deltaB);
}

#pragma endregion

#pragma endregion

float32 PhysicsManager::GetRepulseCorrectionMultiplyer(Collider* colA, Collider* colB)
{
	gcle::Shape* a = colA->GetShape();
	gcle::Shape* b = colB->GetShape();

	if (colA->GetOwner()->IsStatic() && colB->GetOwner()->IsStatic())
		return 0.5;
	else
		return 1.0;
}

#pragma region Sets

void PhysicsManager::SetActivateQuadTree(bool activate) {
	m_activateQuadTree = activate;
}

void PhysicsManager::SetDynamicQuadTreeSize(bool activate) {
	m_dynamicQuadTreeSize = activate;
}

void PhysicsManager::SetQuadTreePos1(Vector2f pos1) {
	m_quadTreePos1 = pos1;
}

void PhysicsManager::SetQuadTreePos2(Vector2f pos2) {
	m_quadTreePos2 = pos2;
}

void PhysicsManager::SetFrameBetweenQuadTreeRegenerations(int8 nbrFrame) {
	m_frameBetweenQuadTreeRegenerations = nbrFrame;
}

bool PhysicsManager::IsQuadTreeActive() const
{
	return m_activateQuadTree;
}

#pragma endregion

PhysicsManager::~PhysicsManager() {
	delete m_quadTree;
}

