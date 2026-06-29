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

void PhysicsManager::Update(float64 deltaTime)
{
	//PROFILER_START("RemoveEntity", "RemoveEntities");
	for (EntityInfo entity : m_EntitiesToRemove) {
		for (int i = (int)m_EntitiesToUpdate.size() - 1; i >= 0; --i)
		{
			if (m_EntitiesToUpdate[i].pEntity == entity.pEntity) {
				m_EntitiesToUpdate.erase(m_EntitiesToUpdate.begin() + i);
			}

		}
	}
	//PROFILER_END("RemoveEntity");

	//PROFILER_START("AddEntity", "AddEntity");
	for (EntityInfo entity : m_EntitiesToAdd) {
		m_EntitiesToUpdate.push_back(entity);
	}
	//PROFILER_END("AddEntity");

	m_EntitiesToAdd.clear();
	m_EntitiesToRemove.clear();


	int32 nbrTest = 0;
	if (m_activateQuadTree == true) {
		std::vector<Entity*> activeEntities; 
		//PROFILER_START("GetActiveEntities", "GetActiveEntities");
		for(auto& e : m_EntitiesToUpdate){
			if(e.pEntity->IsActiveIn(SceneManager::GetInstance().GetCurrentSceneTag()))
			activeEntities.push_back(e.pEntity);
		}
		//PROFILER_END("GetActiveEntities");

		//PROFILER_START("QuadTreeRegeneration", "QuadTreeRegeneration");
		m_timeBetweenRegeneration += 1;
		if (m_timeBetweenRegeneration >= m_frameBetweenQuadTreeRegenerations) {
			m_timeBetweenRegeneration = 0;
			m_quadTree->Clear();
			for (auto& entity : activeEntities) {
				m_quadTree->Insert(entity);
			}
		}
		//PROFILER_END("QuadTreeRegeneration");

		//PROFILER_START("Query", "Query");
		for (auto& entity : activeEntities) {
			AABB aabb;
			Vector2f pos1 = entity->GetPosition(0.f, 0.f);
			Vector2f pos2 = entity->GetPosition(1.f, 1.f);
			aabb = { pos1.x, pos1.y, pos2.x , pos2.y };

			bool present = false;
			ColliderEntry e{ aabb, entity };
			
			////PROFILER_START("QueryQuad", "QueryQuad");
			const auto& candidates = m_quadTree->Query(e);
			////PROFILER_END("QueryQuad");

			for (auto& c : candidates) {
				if (entity < c.entity) {
					m_pairs.push_back({ entity, c.entity });
				}
			}
		}
		//PROFILER_END("Query");

		//PROFILER_START("collisionLoop", "collisionLoop");
		for (auto& entities : m_pairs) {
			nbrTest += 1;

			//PROFILER_START("collisionTest", "Collision test");
			bool coliding = entities.first->IsColliding(entities.second);
			//PROFILER_END("collisionTest");
			
			
			if (coliding) {
				if (entities.first->IsRigidBody() && entities.second->IsRigidBody())
				{
					//PROFILER_START("Repulse", "Repulse");

					Repulse(entities.first, entities.second);
					//PROFILER_END("Repulse");
				}
				//PROFILER_START("Callbacks", "Callbacks");
				if (!entities.first->CollidingEntity.contains(entities.second->GetId()))
				{
					entities.first->OnCollisionEnter(entities.second);
					entities.first->CollidingEntity.insert(entities.second->GetId());

					entities.second->OnCollisionEnter(entities.first);
					entities.second->CollidingEntity.insert(entities.first->GetId());
				}

				else
				{
					entities.first->OnCollision(entities.second);
					entities.second->OnCollision(entities.first);
				}
				//PROFILER_END("Callbacks");
			}

			else
			{
			//PROFILER_START("Callbacks", "Callbacks");
				if (entities.first->CollidingEntity.contains(entities.second->GetId()))
				{
					entities.first->OnCollisionExit(entities.second);
					entities.first->CollidingEntity.erase(entities.second->GetId());
					entities.second->OnCollisionExit(entities.first);
					entities.second->CollidingEntity.erase(entities.first->GetId());
				}
			}
			//PROFILER_END("Callbacks");


		}
		m_pairs.clear();

		//PROFILER_END("collisionLoop");
		//GCLE_INFO << "Loop end" << ENDL;
	}
	
	else {
		for (auto it1 = m_EntitiesToUpdate.begin(); it1 != m_EntitiesToUpdate.end(); ++it1)
		{
			auto it2 = it1;
			++it2;
			for (; it2 != m_EntitiesToUpdate.end(); ++it2)
			{
				Entity* entity = (*it1).pEntity;
				Entity* otherEntity = (*it2).pEntity;

				if (entity->IsActiveIn(SceneManager::GetInstance().GetCurrentSceneTag()) && otherEntity->IsActiveIn(SceneManager::GetInstance().GetCurrentSceneTag()))
				{
					if (entity->IsColliding(otherEntity))
					{
						if (entity->IsRigidBody() && otherEntity->IsRigidBody())
						{
							Repulse(entity, otherEntity);
						}
						if (!entity->CollidingEntity.contains(otherEntity->GetId()))
						{
							entity->OnCollisionEnter(otherEntity);
							entity->CollidingEntity.insert(otherEntity->GetId());

							otherEntity->OnCollisionEnter(entity);
							otherEntity->CollidingEntity.insert(entity->GetId());
						}

						else
						{
							entity->OnCollision(otherEntity);
							otherEntity->OnCollision(entity);
						}
					}
					else 
					{
						if (entity->CollidingEntity.contains(otherEntity->GetId())) 
						{
							entity->OnCollisionExit(otherEntity);
							entity->CollidingEntity.erase(otherEntity->GetId());
							otherEntity->OnCollisionExit(entity);
							otherEntity->CollidingEntity.erase(entity->GetId());
						}
					}
				}
			}
		}
	}

	//GCLE_INFO << nbrTest << ENDL;

	


	//Erase
	for (int32 i = static_cast<int32>(m_EntitiesToUpdate.size()) - 1; i >= 0; i--)
	{
		if (m_EntitiesToUpdate[i].toRemove == false)
		{
			continue;
		}

		m_EntitiesToUpdate.erase(m_EntitiesToUpdate.begin() + i);
	}
}

bool PhysicsManager::IsColliding(Entity* pEntity1, Entity* pEntity2)
{
	auto* shapeA = pEntity1->GetShape();
	auto* shapeB = pEntity2->GetShape();

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

void PhysicsManager::Repulse(Entity* pEntity1, Entity* pEntity2)
{
	auto* shapeA = pEntity1->GetShape();
	auto* shapeB = pEntity2->GetShape();

	RepulseTypes typeA;
	RepulseTypes typeB;

	switch (shapeA->GetShape()) {
	case gcle::Shapes::Circle:
		typeA = RepulseTypes::Circle;
		break;
	case gcle::Shapes::Rectangle:
		if (static_cast<int32>(shapeA->GetTransform().GetDegAngle()) % 180 != 0) 
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
		if (static_cast<int32>(shapeB->GetTransform().GetDegAngle()) % 180 != 0) 
			typeB = RepulseTypes::OOB;
		else
			typeB = RepulseTypes::AABB;
		break;
	case gcle::Shapes::Triangle:
		return;
		break;
	}


	(this->*repulseTable[static_cast<int32>(typeA)][static_cast<int32>(typeB)])(shapeA, shapeB);
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


bool PhysicsManager::CheckOBBAABBCollision(gcle::Rectangle* pRect1, gcle::Rectangle* pRect2){

	Vector2f axes[2];

	float32 radRotation = pRect1->GetTransform().GetRadAngle();
	axes[0] = {std::cos(radRotation), std::sin(radRotation) };
	axes[1] = {-std::sin(radRotation), std::cos(radRotation) };


	//OBB values
	Vector2f obbExtents{pRect1->GetWidth() / 2, pRect1->GetHeight()/2};

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

bool PhysicsManager::CheckOBBOBBCollision(gcle::Rectangle* pRect1, gcle::Rectangle* pRect2){
	Vector2f axes1[2];
	Vector2f axes2[2];

	float32 radRotation1 = pRect1->GetTransform().GetRadAngle();
	float32 radRotation2 = pRect2->GetTransform().GetRadAngle();
	
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
	float32 angle = pRect->GetTransform().GetRadAngle();

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

	float32 distance = (distanceCarree == 0.f) ? 0.f :std::sqrt(distanceCarree) ;
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


bool PhysicsManager::CheckRectRect(gcle::Shape* a, gcle::Shape* b)
{
	int16 angleA = static_cast<int16>(a->GetTransform().GetDegAngle()) % 180;
	int16 angleB = static_cast<int16>(b->GetTransform().GetDegAngle()) % 180;

	if (angleA != 0) {
		if (angleB != 0)
			return CheckOBBOBBCollision(static_cast<gcle::Rectangle*>(a), static_cast<gcle::Rectangle*>(b));
		else
			return CheckOBBAABBCollision(static_cast<gcle::Rectangle*>(a), static_cast<gcle::Rectangle*>(b));
	}
	else if (angleB != 0) {
		bool hit =  CheckOBBAABBCollision(static_cast<gcle::Rectangle*>(b), static_cast<gcle::Rectangle*>(a));
		if (hit) {
			m_colDatas.orientation = -m_colDatas.orientation;
		}
		return hit;
	}


	return CheckAABBAABBCollision(static_cast<gcle::Rectangle*>(a), static_cast<gcle::Rectangle*>(b));
}

bool PhysicsManager::CheckCircleCircle(gcle::Shape* a, gcle::Shape* b)
{
	return CheckCircleCircleCollision(static_cast<gcle::Circle*>(a), static_cast<gcle::Circle*>(b));
}

bool PhysicsManager::CheckRectCircle(gcle::Shape* a, gcle::Shape* b)
{
	int16 angle = static_cast<int16>(a->GetTransform().GetDegAngle()) % 180;
	if (angle != 0) {
		return CheckOBBCircleCollision(static_cast<gcle::Rectangle*>(a), static_cast<gcle::Circle*>(b));
	}

	return CheckAABBCircleCollision(static_cast<gcle::Rectangle*>(a), static_cast<gcle::Circle*>(b));
}

bool PhysicsManager::CheckCircleRect(gcle::Shape* a, gcle::Shape* b)
{
	int16 angle = static_cast<int16>(b->GetTransform().GetDegAngle()) % 180;
	if (angle != 0) {
		bool hit =CheckOBBCircleCollision(static_cast<gcle::Rectangle*>(b), static_cast<gcle::Circle*>(a));
		if (hit)
			m_colDatas.orientation = -m_colDatas.orientation;
		return hit;

	}
	return CheckAABBCircleCollision(static_cast<gcle::Rectangle*>(b), static_cast<gcle::Circle*>(a));
}


void PhysicsManager::RepulseRectRect(gcle::Shape* a, gcle::Shape* b)
{
	//DEBUG_INFO << "repulse rect " << ENDL
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

	Vector2f pos1 = a->GetPosition(0.5f, 0.5f);
	Vector2f pos2 = b->GetPosition(0.5f, 0.5f);

	float32 correctionMultiplyer = GetRepulseCorrectionMultiplyer(a, b);

	if (overlapX < overlapY)
	{
		float32 correction = overlapX * correctionMultiplyer;

		if (x1 < x2)
		{
			pos1.x -= correction * a->IsKinematic();
			pos2.x += correction * b->IsKinematic();

			a->GetOwner()->GetRigidBody().ZeroVelocityX(false);
			b->GetOwner()->GetRigidBody().ZeroVelocityX(false);
			
			a->GetCollider()->CollidingOnX(-correction);
			b->GetCollider()->CollidingOnX(correction);
		}
		else
		{
			pos1.x += correction * a->IsKinematic();
			pos2.x -= correction * b->IsKinematic();

			b->GetOwner()->GetRigidBody().ZeroVelocityX(true);
			a->GetOwner()->GetRigidBody().ZeroVelocityX(true);

			a->GetCollider()->CollidingOnX(correction);
			b->GetCollider()->CollidingOnX(-correction);
		}

		if (a->IsKinematic())
			a->GetOwner()->GetRigidBody().ZeroVelocityX();
		if (b->IsKinematic())
			b->GetOwner()->GetRigidBody().ZeroVelocityX();

	}
	else
	{
		float32 correction = overlapY * correctionMultiplyer;

		if (y1 < y2)
		{
			pos1.y -= correction * a->IsKinematic();
			pos2.y += correction * b->IsKinematic();

			a->GetOwner()->GetRigidBody().ZeroVelocityY(false);
			b->GetOwner()->GetRigidBody().ZeroVelocityY(false);

			a->GetCollider()->CollidingOnY(-correction);
			b->GetCollider()->CollidingOnY(correction);
		}
		else
		{
			pos1.y += correction * a->IsKinematic();
			pos2.y -= correction * b->IsKinematic();

			a->GetOwner()->GetRigidBody().ZeroVelocityY(true);
			b->GetOwner()->GetRigidBody().ZeroVelocityY(true);

			a->GetCollider()->CollidingOnY(correction);
			b->GetCollider()->CollidingOnY(-correction);
		}
		
		if(a->IsKinematic())
			a->GetOwner()->GetRigidBody().ZeroVelocityY();
		if(b->IsKinematic())
			b->GetOwner()->GetRigidBody().ZeroVelocityY();
	}

	a->SetPosition(pos1.x, pos1.y, 0.5f, 0.5f);
	b->SetPosition(pos2.x, pos2.y, 0.5f, 0.5f);

}

void PhysicsManager::RepulseCircleCircle(gcle::Shape* a, gcle::Shape* b)
{
	Vector2f distance = a->GetPosition(0.5f, 0.5f) - b->GetPosition(0.5f, 0.5f);

	float32 sqrLength = (distance.x * distance.x) + (distance.y * distance.y);
	float32 length = std::sqrt(sqrLength);

	float32 radius1 = a->GetRadius();
	float32 radius2 = b->GetRadius();

	float32 overlap = (length - (radius1 + radius2)) * GetRepulseCorrectionMultiplyer(a, b);

	Vector2f normal = distance / length;

	Vector2f translation = normal * overlap;

	Vector2f position1 = a->GetPosition(0.5f, 0.5f) - translation * a->IsKinematic();
	Vector2f position2 = b->GetPosition(0.5f, 0.5f) + translation * b->IsKinematic();

	a->SetPosition(position1.x, position1.y, 0.5f, 0.5f);
	b->SetPosition(position2.x, position2.y, 0.5f, 0.5f);

	//gestion de la velocity

	if (!a->IsKinematic() || !b->IsKinematic()){
		a->GetOwner()->GetRigidBody().RemoveVelocityAlongNormal(normal);
		b->GetOwner()->GetRigidBody().RemoveVelocityAlongNormal(-normal);
	}
}

void PhysicsManager::RepulseRectCircle(gcle::Shape* a, gcle::Shape* b)
{
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

	float32 correctionMultiplyer = GetRepulseCorrectionMultiplyer(a, b);

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

		Vector2f newCirclePos = circlePos + translation * b->IsKinematic();
		pCircle->SetPosition(newCirclePos.x, newCirclePos.y, 0.5f, 0.5f);

		Vector2f newRectPos = rectPos - translation * a->IsKinematic();
		pRect->SetPosition(newRectPos.x, newRectPos.y, 0.5f, 0.5f);

		a->GetOwner()->GetRigidBody().RemoveVelocityAlongNormal(-normal);
		b->GetOwner()->GetRigidBody().RemoveVelocityAlongNormal(normal);
		if (!a->IsKinematic() || !b->IsKinematic()) {
		}

		return;
	}

	float32 length = std::sqrt(dist);
	Vector2f normal = delta / length;
	float32  overlap = (pCircle->GetRadius() - length) * correctionMultiplyer;

	Vector2f translation = normal * overlap;

	Vector2f newCirclePos = circlePos + translation * b->IsKinematic();
	pCircle->SetPosition(newCirclePos.x, newCirclePos.y, 0.5f, 0.5f);

	Vector2f newRectPos = rectPos - translation * a->IsKinematic();
	pRect->SetPosition(newRectPos.x, newRectPos.y, 0.5f, 0.5f);

	if (!a->IsKinematic() || !b->IsKinematic()) {
		a->GetOwner()->GetRigidBody().RemoveVelocityAlongNormal(normal);
		b->GetOwner()->GetRigidBody().RemoveVelocityAlongNormal(-normal);
	}

	a->GetCollider()->CollidingOn(normal);
	b->GetCollider()->CollidingOn(normal);
}

void PhysicsManager::RepulseCircleRect(gcle::Shape* a, gcle::Shape* b)
{
	RepulseRectCircle(b, a);
}

void PhysicsManager::RepulseOBB(gcle::Shape* a, gcle::Shape* b) {
	if (a->IsKinematic() == true) {
		Vector2f actualPosition= a->GetPosition();
		Vector2f pos = actualPosition - ((m_colDatas.orientation * m_colDatas.penetration) * GetRepulseCorrectionMultiplyer(a, b));
		a->SetPosition(pos.x, pos.y);
	}
	if (b->IsKinematic() == true) {
		Vector2f actualPosition = b->GetPosition();
		Vector2f pos = actualPosition + ((m_colDatas.orientation * m_colDatas.penetration) * GetRepulseCorrectionMultiplyer(a, b));
		b->SetPosition(pos.x, pos.y);
	}

}

float32 PhysicsManager::GetRepulseCorrectionMultiplyer(gcle::Shape* a, gcle::Shape* b)
{
	if (a->IsKinematic() && b->IsKinematic())
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
