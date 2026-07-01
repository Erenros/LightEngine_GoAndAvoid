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

#pragma region Update

void PhysicsManager::Update(float64 deltaTime)
{
	EntityToRemove(m_EntitiesToRemove, m_EntitiesToUpdate);
	EntityToAdd(m_EntitiesToAdd, m_EntitiesToUpdate);

	std::vector<Collider*> activeColliders;

	EntityToUpdate(&activeColliders, m_EntitiesToUpdate);

	const std::string& currentScene = SceneManager::GetInstance().GetCurrentSceneTag();

	if (m_activateQuadTree == true) {
		UpdateQuadTree(activeColliders);
	}

	else {
		UpdateWithoutQuadTree(activeColliders);
	}
}

#pragma region Helpers

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
		if (e.pEntity->IsActiveIn(SceneManager::GetInstance().GetCurrentSceneTag())) {
			for (auto& collider : e.pEntity->GetColliders()) {
				if (collider->IsActive())
					activeColliders->push_back(collider);
			}
		}
	}
}

void PhysicsManager::GenerateQuadTree(std::vector<Collider*>* activeColliders) {

	if (m_timeBetweenRegeneration >= m_frameBetweenQuadTreeRegenerations) {
		m_quadTree->Clear();
		for (auto& collider : *activeColliders) {

			Degrees angle = collider->GetShape()->GetRotation();
			if (static_cast<int32>(angle) % 180 == 0) {
				Vector2f pos1 = collider->GetShape()->GetPosition(0.f, 0.f);
				Vector2f pos2 = collider->GetShape()->GetPosition(1.f, 1.f);
				collider->SetAABB({ pos1.x, pos1.y, pos2.x , pos2.y });
			}
			else {
				collider->SetAABB(GetRotatedAABB(collider->GetShape()->GetPosition(), { collider->GetShape()->GetWidth(), collider->GetShape()->GetHeight() }, (angle * DEG_TO_RAD)));
			}
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

		Vector2f current = pEntity->GetShape()->GetPosition(0.5f, 0.5f);
		pEntity->GetShape()->SetPosition(current.x + delta.x, current.y + delta.y, 0.5f, 0.5f);
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
			if (greater && notTheSameEntity) {
				m_pairs.push_back({ colliderA, colliderB });
			}
		}
	}
}

void PhysicsManager::MakeTreePairs(std::vector<Collider*>* activeColliders)
{
	for (auto& collider : *activeColliders) {

		const auto& candidates = m_quadTree->Query(collider);

		for (auto& c : candidates) {
			bool greater = collider < c;
			bool notTheSameEntity = collider->GetOwner()->GetId() != c->GetOwner()->GetId();
			if (greater && notTheSameEntity) {
				m_pairs.push_back({ collider, c });
			}
		}
	}
}

void PhysicsManager::UpdateQuadTree(std::vector<Collider*> activeColliders)
{
	int32 nbrTest = 0;

	m_timeBetweenRegeneration += 1;

	GenerateQuadTree(&activeColliders);
	
	MakeTreePairs(&activeColliders);

	for (auto& collider : m_pairs) {
		nbrTest += 1;
		bool coliding = IsColliding(collider.first, collider.second);


		if (coliding) {
			if (collider.first->GetOwner()->IsRigidBody() && collider.second->GetOwner()->IsRigidBody())
			{
				ThrowRepulse(collider.first, collider.second);
			}
			if (!collider.first->GetOwner()->CollidingEntity.contains(collider.second->GetOwner()->GetId()))
			{
				collider.first->GetOwner()->OnCollisionEnter(collider.second->GetOwner());
				collider.first->GetOwner()->CollidingEntity.insert({ collider.second->GetOwner()->GetId(), collider.second->GetOwner() });

				collider.second->GetOwner()->OnCollisionEnter(collider.first->GetOwner());
				collider.second->GetOwner()->CollidingEntity.insert({ collider.first->GetOwner()->GetId(), collider.first->GetOwner() });
			}

			else
			{
				collider.first->GetOwner()->OnCollision(collider.second->GetOwner());
				collider.second->GetOwner()->OnCollision(collider.first->GetOwner());
			}
		}

		else
		{
			if (collider.first->GetOwner()->CollidingEntity.contains(collider.second->GetOwner()->GetId()))
			{
				collider.first->GetOwner()->OnCollisionExit(collider.second->GetOwner());
				collider.first->GetOwner()->CollidingEntity.erase(collider.second->GetOwner()->GetId());
				collider.second->GetOwner()->OnCollisionExit(collider.first->GetOwner());
				collider.second->GetOwner()->CollidingEntity.erase(collider.first->GetOwner()->GetId());
			}
		}

		PendingCorrections();

		for (int32 i = static_cast<int32>(m_EntitiesToUpdate.size()) - 1; i >= 0; i--)
		{
			if (m_EntitiesToUpdate[i].toRemove)
				m_EntitiesToUpdate.erase(m_EntitiesToUpdate.begin() + i);
		}

	}
	m_pairs.clear();
}

void PhysicsManager::UpdateWithoutQuadTree(std::vector<Collider*> activeColliders) {

	auto MakePairKey = [](int64 idA, int64 idB) -> uint64
		{
			if (idA > idB) std::swap(idA, idB);
			return (static_cast<uint64>(idA) << 32) ^ static_cast<uint64>(idB);
		};

	std::unordered_set<uint64> collidingPairsThisFrame;

	for (auto it1 = activeColliders.begin(); it1 != activeColliders.end(); ++it1)
	{
		Collider* collider1 = *it1;
		Entity* entity = collider1->GetOwner();

		for (auto it2 = std::next(it1); it2 != activeColliders.end(); ++it2)
		{
			Collider* collider2 = *it2;
			Entity* otherEntity = collider2->GetOwner();

			if (entity == otherEntity)
				continue;

			if (!IsColliding(collider1, collider2))
				continue;

			if (entity->IsRigidBody() && otherEntity->IsRigidBody())
				ThrowRepulse(collider1, collider2);

			collidingPairsThisFrame.insert(MakePairKey(entity->GetId(), otherEntity->GetId()));

			if (!entity->CollidingEntity.contains(otherEntity->GetId()))
			{
				entity->OnCollisionEnter(otherEntity);
				entity->CollidingEntity.insert({ otherEntity->GetId(), otherEntity });

				otherEntity->OnCollisionEnter(entity);
				otherEntity->CollidingEntity.insert({ entity->GetId(), entity });
			}
			else
			{
				entity->OnCollision(otherEntity);
				otherEntity->OnCollision(entity);
			}
		}
	}

	PendingCorrections();

	for (auto& info : m_EntitiesToUpdate)
	{
		Entity* entity = info.pEntity;

		for (auto idIt = entity->CollidingEntity.begin(); idIt != entity->CollidingEntity.end(); )
		{
			uint64 key = MakePairKey(entity->GetId(), idIt->first);

			if (collidingPairsThisFrame.contains(key))
			{
				++idIt;
				continue;
			}

			Entity* otherEntity = idIt->second;
			idIt = entity->CollidingEntity.erase(idIt);
			entity->OnCollisionExit(otherEntity);
		}
	}

	// 5) Suppression des entites marquees a retirer.
	for (int32 i = static_cast<int32>(m_EntitiesToUpdate.size()) - 1; i >= 0; i--)
	{
		if (m_EntitiesToUpdate[i].toRemove)
			m_EntitiesToUpdate.erase(m_EntitiesToUpdate.begin() + i);
	}
}

#pragma endregion

#pragma endregion




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


bool PhysicsManager::IsInside(Entity* pEntity, Vector2f positionToCheck)
{
	if (pEntity == nullptr || pEntity->GetShape() == nullptr)
		return false;

	switch (pEntity->GetShape()->GetShape())
	{
	case gcle::Shapes::Rectangle:
	{
		gcle::Rectangle* pRect = static_cast<gcle::Rectangle*>(pEntity->GetRenderShape());
		return
		{
			positionToCheck.x >= pRect->GetPosition().x - pRect->GetWidth()  * 0.5f &&
			positionToCheck.x <= pRect->GetPosition().x + pRect->GetWidth()  * 0.5f &&
			positionToCheck.y >= pRect->GetPosition().y - pRect->GetHeight() * 0.5f &&
			positionToCheck.y <= pRect->GetPosition().y + pRect->GetHeight() * 0.5f
		};
	}
	case gcle::Shapes::Circle:
	{
		gcle::Circle* pCircle = static_cast<gcle::Circle*>(pEntity->GetRenderShape());
		return pCircle->GetPosition().GetDistance(positionToCheck) <= pCircle->GetRadius();
	}
	default:
		break;
	}

	return false;
} 

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


bool PhysicsManager::ThrowCheckRectRect(gcle::Shape* a, gcle::Shape* b)
{
	int16 angleA = static_cast<int16>(a->GetTransform()->GetDegAngle());
	int16 angleB = static_cast<int16>(b->GetTransform()->GetDegAngle()) ;


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

	if (angle != 0) {
		return CheckOBBCircleCollision(static_cast<gcle::Rectangle*>(a), static_cast<gcle::Circle*>(b));
	}

	return CheckAABBCircleCollision(static_cast<gcle::Rectangle*>(a), static_cast<gcle::Circle*>(b));
}

bool PhysicsManager::ThrowCheckCircleRect(gcle::Shape* a, gcle::Shape* b)
{
	int16 angle = static_cast<int16>(b->GetTransform()->GetDegAngle()) % 180;
	if (angle != 0) {
		bool hit =CheckOBBCircleCollision(static_cast<gcle::Rectangle*>(b), static_cast<gcle::Circle*>(a));
		if (hit)
			m_colDatas.orientation = -m_colDatas.orientation;
		return hit;

	}
	return CheckAABBCircleCollision(static_cast<gcle::Rectangle*>(b), static_cast<gcle::Circle*>(a));
}


void PhysicsManager::RepulseRectRect(Collider* colA, Collider* colB)
{
	gcle::Shape* a = colA->GetShape();
	gcle::Shape* b = colB->GetShape();

	gcle::Rectangle* pRect1 = static_cast<gcle::Rectangle*>(a);
	gcle::Rectangle* pRect2 = static_cast<gcle::Rectangle*>(b);

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
			delta1.x -= correction * a->GetOwner()->IsStatic();
			delta2.x += correction * b->GetOwner()->IsStatic();

		}
		else
		{
			delta1.x += correction * a->GetOwner()->IsStatic();
			delta2.x -= correction * b->GetOwner()->IsStatic();

		}

		if (!colA->GetOwner()->IsStatic() || !colB->GetOwner()->IsStatic())
		{
			a->GetOwner()->GetRigidBody().ZeroVelocityX();
			b->GetOwner()->GetRigidBody().ZeroVelocityX();
		}
	}
	else
	{
		float32 correction = overlapY * correctionMultiplyer;

		if (y1 < y2)
		{
			delta1.y -= correction * a->GetOwner()->IsStatic();
			delta2.y += correction * b->GetOwner()->IsStatic();

		}
		else
		{
			delta1.y += correction * a->GetOwner()->IsStatic();
			delta2.y -= correction * b->GetOwner()->IsStatic();

		}

		if (!colA->GetOwner()->IsStatic() || !colB->GetOwner()->IsStatic())
		{
			a->GetOwner()->GetRigidBody().ZeroVelocityY();
			b->GetOwner()->GetRigidBody().ZeroVelocityY();
		}
	}

	ApplyBlockingResponse(colA, colB, delta1);
	ApplyBlockingResponse(colB, colA, delta2);

	AccumulateCorrection(a->GetOwner(), delta1);
	AccumulateCorrection(b->GetOwner(), delta2);

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

	Vector2f delta1 = translation * KinematicFactor(a->GetOwner());
	Vector2f delta2 = -translation * KinematicFactor(b->GetOwner());

	AccumulateCorrection(a->GetOwner(), delta1);
	AccumulateCorrection(b->GetOwner(), delta2);

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
		Vector2f deltaRect = -translation * KinematicFactor(a->GetOwner());
		Vector2f deltaCircle = translation * KinematicFactor(b->GetOwner());

		ApplyBlockingResponse(colA, colB, deltaRect);
		ApplyBlockingResponse(colB, colA, deltaCircle);

		AccumulateCorrection(a->GetOwner(), deltaRect);
		AccumulateCorrection(b->GetOwner(), deltaCircle);

		return;
	}

	float32 length = std::sqrt(dist);
	Vector2f normal = delta / length;
	float32  overlap = (pCircle->GetRadius() - length) * correctionMultiplyer;

	Vector2f translation = normal * overlap;
	Vector2f deltaRect = -translation * KinematicFactor(a->GetOwner());
	Vector2f deltaCircle = translation * KinematicFactor(b->GetOwner());

	ApplyBlockingResponse(colA, colB, deltaRect);
	ApplyBlockingResponse(colB, colA, deltaCircle);

	AccumulateCorrection(a->GetOwner(), deltaRect);
	AccumulateCorrection(b->GetOwner(), deltaCircle);
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

	Vector2f normal = SafeNormal(m_colDatas.orientation, { 1.0f, 0.0f }); // colA -> colB
	float32 penetration = std::max(static_cast<float32>(m_colDatas.penetration), 0.0f);

	if (penetration <= 0.0f)
		return;

	Vector2f correction = normal * penetration * GetRepulseCorrectionMultiplyer(colA, colB);

	Vector2f deltaA = -correction * KinematicFactor(a->GetOwner());
	Vector2f deltaB = correction * KinematicFactor(b->GetOwner());

	ApplyBlockingResponse(colA, colB, deltaA);
	ApplyBlockingResponse(colB, colA, deltaB);

	AccumulateCorrection(a->GetOwner(), deltaA);
	AccumulateCorrection(b->GetOwner(), deltaB);
}

float32 PhysicsManager::GetRepulseCorrectionMultiplyer(Collider* colA, Collider* colB)
{
	gcle::Shape* a = colA->GetShape();
	gcle::Shape* b = colB->GetShape();

	if (a->GetOwner()->IsStatic() && b->GetOwner()->IsStatic())
		return 0.5;
	else
		return 1.0;
}

void PhysicsManager::SetActivateQuadTree(bool activate){
	m_activateQuadTree = activate;
}

void PhysicsManager::SetDynamicQuadTreeSize(bool activate){
	m_dynamicQuadTreeSize = activate;
}

void PhysicsManager::SetQuadTreePos1(Vector2f pos1){
	m_quadTreePos1 = pos1;
}

void PhysicsManager::SetQuadTreePos2(Vector2f pos2){
	m_quadTreePos2 = pos2;
}

void PhysicsManager::SetFrameBetweenQuadTreeRegenerations(int8 nbrFrame){
	m_frameBetweenQuadTreeRegenerations = nbrFrame;
}

PhysicsManager::~PhysicsManager(){
	delete m_quadTree;
}
