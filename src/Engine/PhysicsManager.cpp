#include "PhysicsManager.h"
#include "SceneManager.h"
#include "MathGC.h"

#undef min
#undef max

PhysicsManager::CollisionFn PhysicsManager::collisionTable[static_cast<int32>(gcle::Shapes::Count) - 1][static_cast<int32>(gcle::Shapes::Count) - 1]
{
	{
		&PhysicsManager::CheckRectRect,
		&PhysicsManager::CheckRectCircle,
	},
	{
		&PhysicsManager::CheckCircleRect,
		&PhysicsManager::CheckCircleCircle,
	}
};

PhysicsManager::RepulseFn PhysicsManager::repulseTable[static_cast<int32>(RepulseTypes::Count)][static_cast<int32>(RepulseTypes::Count)]
{
	{
		&PhysicsManager::RepulseRectRect,
		&PhysicsManager::RepulseRectCircle,
		&PhysicsManager::RepulseAABBOBB,
	},
	{
		&PhysicsManager::RepulseCircleRect,
		&PhysicsManager::RepulseCircleCircle,
		&PhysicsManager::RepulseCircleOBB,
	},
	{
		&PhysicsManager::RepulseOBBAABB,
		&PhysicsManager::RepulseOBBCircle,
		&PhysicsManager::RepulseOBBOBB,
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

void PhysicsManager::Update(float64 deltaTime)
{
	const std::string& currentScene = SceneManager::GetInstance().GetCurrentSceneTag();

	auto MakePairKey = [](int64 idA, int64 idB) -> uint64
		{
			if (idA > idB) std::swap(idA, idB);
			return (static_cast<uint64>(idA) << 32) ^ static_cast<uint64>(idB);
		};

	// 1) Collecte des colliders actifs des entit?s actives dans la sc?ne courante.
	std::vector<Collider*> activeColliders;
	for (auto& info : m_EntitiesToUpdate)
	{
		Entity* entity = info.pEntity;
		if (!entity->IsActiveIn(currentScene))
			continue;

		for (Collider* pCollider : entity->GetColliders())
		{
			if (pCollider->IsActive()) {
				activeColliders.push_back(pCollider);
			}
		}
	}

	// 2) Test de chaque paire de colliders, d?duplique par paire d'ENTIT?S.
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
				Repulse(collider1, collider2);

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

	// 3) Application des corrections de position accumulees : une seule fois par
	//    entite, meme si plusieurs de ses colliders ont ete corriges cette frame.
	for (auto& pair : m_PendingCorrections)
	{
		Entity* pEntity = pair.first;
		Vector2f delta = pair.second;

		Vector2f current = pEntity->GetShape()->GetPosition(0.5f, 0.5f);
		pEntity->GetShape()->SetPosition(current.x + delta.x, current.y + delta.y, 0.5f, 0.5f);
	}
	m_PendingCorrections.clear();

	// 4) Sortie de collision : une paire d'entites ne "sort" que si plus aucun
	//    de leurs colliders ne se touche cette frame.
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

	return (this->*collisionTable[typeA][typeB])(shapeA, shapeB);
}


bool PhysicsManager::IsInside(Entity* pEntity, Vector2f positionToCheck)
{

	switch (pEntity->GetShape()->GetShape())
	{
	case gcle::Shapes::Rectangle:
	{
		gcle::Rectangle* pRect = static_cast<gcle::Rectangle*>(pEntity->GetShape());
		return
		{
			positionToCheck.x >= pRect->GetPosition().x &&
			positionToCheck.x <= pRect->GetPosition().x + pRect->GetWidth() &&
			positionToCheck.y >= pRect->GetPosition().y &&
			positionToCheck.y <= pRect->GetPosition().y + pRect->GetHeight()
		};
	}
	case gcle::Shapes::Circle:
	{
		gcle::Circle* pCircle = static_cast<gcle::Circle*>(pEntity->GetShape());
		return pCircle->GetPosition().GetDistance(positionToCheck) <= pCircle->GetRadius();
	}
	default:
		break;
	}

	return false;
}

void PhysicsManager::Repulse(Collider* pCollider1, Collider* pCollider2)
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


	return {
		x1 + w1 >= x2 &&
		x1 <= x2 + w2 &&
		y1 + h1 >= y2 &&
		y1 <= y2 + h2
	};
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


	Matrix3x3 rotation = pRect1->GetTransform()->GetMatrix();

	//OBB values
	Vector2f obbExtents{ pRect1->GetWidth() / 2, pRect1->GetHeight() / 2 };

	//AABB values
	Vector2f aabbExtents{ pRect2->GetWidth() / 2, pRect2->GetHeight() / 2 };

	Vector2f T = pRect2->GetPosition() - pRect1->GetPosition();

	Matrix relativeRotation(2, 2);
	Matrix absoluteRotation(2, 2);

	absoluteRotation.m_matrix[0][0] = rotation[0][0];
	absoluteRotation.m_matrix[1][0] = rotation[1][0];
	absoluteRotation.m_matrix[0][1] = rotation[0][1];
	absoluteRotation.m_matrix[1][1] = rotation[1][1];

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
		collisionNormal = { (T.x * 0.f ? 1.f : -1.f), 0.f };
		collisionNormal.x = (T.x > 0.f) ? 1.0f : -1.0f;
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
		collisionNormal = { axes[1].x * sign, axes[1].y };
	}

	colDatas.orientation = collisionNormal;
	colDatas.penetration = minOverlap;

	DEBUG_INFO << "ca touche obb aabb" << ENDL;
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

	/*Matrix3x3 rotation1 = pRect1->GetTransform()->GetMatrix();
	Matrix3x3 rotation2 = pRect2->GetTransform()->GetMatrix();*/

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

	float ra = 0.f, rb = 0.f;

	ra = extents1.x;
	rb = extents2.x * absoluteRotation.m_matrix[0][0] + extents2.y * absoluteRotation.m_matrix[0][1];
	float32 t1_X = std::abs(T.x * axes1[0].x + T.y * axes1[0].y);
	if (t1_X > ra + rb)
		return false;

	ra = extents1.y;
	rb = extents2.x * absoluteRotation.m_matrix[1][0] + extents2.y * absoluteRotation.m_matrix[1][1];
	float32 t1_Y = std::abs(T.x * axes1[1].x + T.y * axes1[1].y);
	if (t1_Y > ra + rb)
		return false;

	ra = extents1.x * absoluteRotation.m_matrix[0][0] + extents1.y * absoluteRotation.m_matrix[1][0];
	rb = extents2.x;
	float32 t2_X = std::abs(T.x * axes2[0].x + T.y * axes2[0].y);
	if (t2_X > ra + rb)
		return false;

	ra = extents1.x * absoluteRotation.m_matrix[0][1] + extents1.y * absoluteRotation.m_matrix[1][1];
	rb = extents2.y;
	float32 t2_Y = std::abs(T.x * axes2[1].x + T.y * axes2[1].y);
	if (t2_Y > ra + rb)
		return false;



	DEBUG_INFO << "ca touche obb obb" << ENDL;
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

	if (distanceCarree <= (radius * radius)) {
		DEBUG_INFO << "ca touche obb circle" << ENDL;
		return true;
	}
	return false;

}


bool PhysicsManager::CheckRectRect(gcle::Shape* a, gcle::Shape* b)
{
	int16 angleA = static_cast<int16>(a->GetTransform()->GetDegAngle()) % 180;
	int16 angleB = static_cast<int16>(b->GetTransform()->GetDegAngle()) % 180;

	if (angleA != 0) {
		if (angleB != 0)
			return CheckOBBOBBCollision(static_cast<gcle::Rectangle*>(a), static_cast<gcle::Rectangle*>(b));
		else
			return CheckOBBAABBCollision(static_cast<gcle::Rectangle*>(a), static_cast<gcle::Rectangle*>(b));
	}
	else if (angleB != 0)
		return CheckOBBAABBCollision(static_cast<gcle::Rectangle*>(b), static_cast<gcle::Rectangle*>(a));

	return CheckAABBAABBCollision(static_cast<gcle::Rectangle*>(a), static_cast<gcle::Rectangle*>(b));
}

bool PhysicsManager::CheckCircleCircle(gcle::Shape* a, gcle::Shape* b)
{
	return CheckCircleCircleCollision(static_cast<gcle::Circle*>(a), static_cast<gcle::Circle*>(b));
}

bool PhysicsManager::CheckRectCircle(gcle::Shape* a, gcle::Shape* b)
{
	int16 angle = static_cast<int16>(a->GetTransform()->GetDegAngle()) % 180;
	if (angle != 0) {
		return CheckOBBCircleCollision(static_cast<gcle::Rectangle*>(a), static_cast<gcle::Circle*>(b));
	}

	return CheckAABBCircleCollision(static_cast<gcle::Rectangle*>(a), static_cast<gcle::Circle*>(b));
}

bool PhysicsManager::CheckCircleRect(gcle::Shape* a, gcle::Shape* b)
{
	int16 angle = static_cast<int16>(b->GetTransform()->GetDegAngle()) % 180;
	if (angle != 0) {
		return CheckOBBCircleCollision(static_cast<gcle::Rectangle*>(b), static_cast<gcle::Circle*>(a));
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
			delta1.x -= correction * a->GetOwner()->IsKinematic();
			delta2.x += correction * b->GetOwner()->IsKinematic();

			a->GetOwner()->GetRigidBody().ZeroVelocityX(false);
			b->GetOwner()->GetRigidBody().ZeroVelocityX(false);

			if (a->GetOwner()->IsKinematic())
				m_pCurrentColliderA->CollidingOnX(-correction);
			if (b->GetOwner()->IsKinematic())
				m_pCurrentColliderB->CollidingOnX(correction);
		}
		else
		{
			delta1.x += correction * a->GetOwner()->IsKinematic();
			delta2.x -= correction * b->GetOwner()->IsKinematic();

			b->GetOwner()->GetRigidBody().ZeroVelocityX(true);
			a->GetOwner()->GetRigidBody().ZeroVelocityX(true);

			if (a->GetOwner()->IsKinematic())
				m_pCurrentColliderA->CollidingOnX(correction);
			if (b->GetOwner()->IsKinematic())
				m_pCurrentColliderB->CollidingOnX(-correction);
		}
	}
	else
	{
		float32 correction = overlapY * correctionMultiplyer;

		if (y1 < y2)
		{
			delta1.y -= correction * a->GetOwner()->IsKinematic();
			delta2.y += correction * b->GetOwner()->IsKinematic();

			a->GetOwner()->GetRigidBody().ZeroVelocityY(false);
			b->GetOwner()->GetRigidBody().ZeroVelocityY(false);

			if (a->GetOwner()->IsKinematic())
				m_pCurrentColliderA->CollidingOnY(-correction);
			if (b->GetOwner()->IsKinematic())
				m_pCurrentColliderB->CollidingOnY(correction);
		}
		else
		{
			delta1.y += correction * a->GetOwner()->IsKinematic();
			delta2.y -= correction * b->GetOwner()->IsKinematic();

			a->GetOwner()->GetRigidBody().ZeroVelocityY(true);
			b->GetOwner()->GetRigidBody().ZeroVelocityY(true);

			if (a->GetOwner()->IsKinematic())
				m_pCurrentColliderA->CollidingOnY(correction);
			if (b->GetOwner()->IsKinematic())
				m_pCurrentColliderB->CollidingOnY(-correction);
		}
	}

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

	float32 overlap = (length - (radius1 + radius2)) * GetRepulseCorrectionMultiplyer(colA, colB);

	Vector2f normal = distance / length;

	Vector2f translation = normal * overlap;

	Vector2f delta1 = -translation * a->GetOwner()->IsKinematic();
	Vector2f delta2 = translation * b->GetOwner()->IsKinematic();

	AccumulateCorrection(a->GetOwner(), delta1);
	AccumulateCorrection(b->GetOwner(), delta2);

	//gestion de la velocity

	if (!a->GetOwner()->IsKinematic() || !b->GetOwner()->IsKinematic()) {
		a->GetOwner()->GetRigidBody().RemoveVelocityAlongNormal(normal);
		b->GetOwner()->GetRigidBody().RemoveVelocityAlongNormal(-normal);
	}
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

		AccumulateCorrection(b->GetOwner(), translation * b->GetOwner()->IsKinematic());
		AccumulateCorrection(a->GetOwner(), -translation * a->GetOwner()->IsKinematic());

		a->GetOwner()->GetRigidBody().RemoveVelocityAlongNormal(-normal);
		b->GetOwner()->GetRigidBody().RemoveVelocityAlongNormal(normal);

		return;
	}

	float32 length = std::sqrt(dist);
	Vector2f normal = delta / length;
	float32  overlap = (pCircle->GetRadius() - length) * correctionMultiplyer;

	Vector2f translation = normal * overlap;

	AccumulateCorrection(b->GetOwner(), translation * b->GetOwner()->IsKinematic());
	AccumulateCorrection(a->GetOwner(), -translation * a->GetOwner()->IsKinematic());

	if (!a->GetOwner()->IsKinematic() || !b->GetOwner()->IsKinematic()) {
		a->GetOwner()->GetRigidBody().RemoveVelocityAlongNormal(normal);
		b->GetOwner()->GetRigidBody().RemoveVelocityAlongNormal(-normal);
	}

	m_pCurrentColliderA->CollidingOn(normal);
	m_pCurrentColliderB->CollidingOn(normal);
}

void PhysicsManager::RepulseCircleRect(Collider* colA, Collider* colB)
{
	std::swap(m_pCurrentColliderA, m_pCurrentColliderB);
	RepulseRectCircle(colA, colB);
}

void PhysicsManager::RepulseAABBOBB(Collider* colA, Collider* colB)
{
	RepulseOBBAABB(colA, colB);
}

void PhysicsManager::RepulseOBBAABB(Collider* colA, Collider* colB)
{
	gcle::Shape* a = colA->GetShape();
	gcle::Shape* b = colB->GetShape();

	if (a->GetOwner()->IsKinematic() == false) {

		Vector2f delta = (colDatas.orientation * colDatas.penetration) * GetRepulseCorrectionMultiplyer(colA, colB);
		AccumulateCorrection(a->GetOwner(), delta);
	}
	if (b->GetOwner()->IsKinematic() == false) {

		Vector2f delta = (colDatas.orientation * colDatas.penetration) * GetRepulseCorrectionMultiplyer(colA, colB);
		AccumulateCorrection(b->GetOwner(), delta);
	}

}

void PhysicsManager::RepulseOBBCircle(Collider* colA, Collider* colB) {

}

void PhysicsManager::RepulseCircleOBB(Collider* colA, Collider* colB) {
	RepulseOBBCircle(colA, colB);
}

void PhysicsManager::RepulseOBBOBB(Collider* colA, Collider* colB) {

}



float32 PhysicsManager::GetRepulseCorrectionMultiplyer(Collider* colA, Collider* colB)
{
	gcle::Shape* a = colA->GetShape();
	gcle::Shape* b = colB->GetShape();

	if (a->GetOwner()->IsKinematic() && b->GetOwner()->IsKinematic())
		return 0.5;
	else
		return 1.0;
}