#include "PhysicsManager.h"
#include "SceneManager.h"

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

PhysicsManager::RepulseFn PhysicsManager::repulseTable[static_cast<int32>(gcle::Shapes::Count) - 1][static_cast<int32>(gcle::Shapes::Count) - 1]
{
	{
		&PhysicsManager::RepulseRectRect,
		&PhysicsManager::RepulseRectCircle,
	},
	{
		&PhysicsManager::RepulseCircleRect,
		&PhysicsManager::RepulseCircleCircle,
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

	//Collision
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

					if (entity->m_OnCollisionEnter)
					{
						entity->OnCollisionEnter(otherEntity);
						entity->m_OnCollisionEnter = false;
					}
					if (otherEntity->m_OnCollisionEnter)
					{
						otherEntity->OnCollisionEnter(otherEntity);
						otherEntity->m_OnCollisionEnter = false;
					}

					entity->OnCollision(otherEntity);
					entity->m_WasOnCollision = true;
					otherEntity->OnCollision(entity);
					otherEntity->m_WasOnCollision = true;
				}
			}
		}
	}


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

	if (shapeA->GetShape() == gcle::Shapes::Triangle || shapeB->GetShape() == gcle::Shapes::Triangle)
	{
		return;
	}

	int32 typeA = static_cast<int32>(shapeA->GetShape()) - 1;
	int32 typeB = static_cast<int32>(shapeB->GetShape()) - 1;

	(this->*repulseTable[typeA][typeB])(shapeA, shapeB);
}

bool PhysicsManager::CheckAABBAABBCollision(gcle::Rectangle* pRect1, gcle::Rectangle* pRect2)
{
	float32 x1 = pRect1->GetPosition().x;
	float32 y1 = pRect1->GetPosition().y;
	float32 w1 = pRect1->GetWidth();
	float32 h1 = pRect1->GetHeight();

	float32 x2 = pRect2->GetPosition().x;
	float32 y2 = pRect2->GetPosition().y;
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

bool PhysicsManager::CheckRectRect(gcle::Shape* a, gcle::Shape* b)
{
	return CheckAABBAABBCollision(static_cast<gcle::Rectangle*>(a), static_cast<gcle::Rectangle*>(b));
}

bool PhysicsManager::CheckCircleCircle(gcle::Shape* a, gcle::Shape* b)
{
	return CheckCircleCircleCollision(static_cast<gcle::Circle*>(a), static_cast<gcle::Circle*>(b));
}

bool PhysicsManager::CheckRectCircle(gcle::Shape* a, gcle::Shape* b)
{
	return CheckAABBCircleCollision(static_cast<gcle::Rectangle*>(a), static_cast<gcle::Circle*>(b));
}

bool PhysicsManager::CheckCircleRect(gcle::Shape* a, gcle::Shape* b)
{
	return CheckAABBCircleCollision(static_cast<gcle::Rectangle*>(b), static_cast<gcle::Circle*>(a));
}

#undef min
#undef max

void PhysicsManager::RepulseRectRect(gcle::Shape* a, gcle::Shape* b)
{
	gcle::Rectangle* pRect1 = static_cast<gcle::Rectangle*>(a);
	gcle::Rectangle* pRect2 = static_cast<gcle::Rectangle*>(b);

	float x1 = pRect1->GetPosition(0.0f, 0.0f).x;
	float y1 = pRect1->GetPosition(0.0f, 0.0f).y;
	float w1 = pRect1->GetWidth();
	float h1 = pRect1->GetHeight();

	float x2 = pRect2->GetPosition(0.0f, 0.0f).x;
	float y2 = pRect2->GetPosition(0.0f, 0.0f).y;
	float w2 = pRect2->GetWidth();
	float h2 = pRect2->GetHeight();

	float overlapX = std::min(x1 + w1, x2 + w2) - std::max(x1, x2);
	float overlapY = std::min(y1 + h1, y2 + h2) - std::max(y1, y2);

	if (overlapX <= 0.0f || overlapY <= 0.0f)
		return;

	Vector2f pos1 = a->GetPosition(0.5f, 0.5f);
	Vector2f pos2 = b->GetPosition(0.5f, 0.5f);

	if (overlapX < overlapY)
	{
		float correction = overlapX * 0.5f;

		if (x1 < x2)
		{
			pos1.x -= correction;
			pos2.x += correction;
		}
		else
		{
			pos1.x += correction;
			pos2.x -= correction;
		}
	}
	else
	{
		float correction = overlapY * 0.5f;

		if (y1 < y2)
		{
			pos1.y -= correction;
			pos2.y += correction;
		}
		else
		{
			pos1.y += correction;
			pos2.y -= correction;
		}
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

	float32 overlap = (length - (radius1 + radius2)) * 0.5f;

	Vector2f normal = distance / length;

	Vector2f translation = normal * overlap;

	Vector2f position1 = a->GetPosition(0.5f, 0.5f) - translation;
	Vector2f position2 = b->GetPosition(0.5f, 0.5f) + translation;

	a->SetPosition(position1.x, position1.y, 0.5f, 0.5f);
	b->SetPosition(position2.x, position2.y, 0.5f, 0.5f);
}

void PhysicsManager::RepulseRectCircle(gcle::Shape* a, gcle::Shape* b)
{
	gcle::Rectangle* pRect = static_cast<gcle::Rectangle*>(a);
	gcle::Circle* pCircle = static_cast<gcle::Circle*>(b);

	float32 rx = pRect->GetPosition(0.0f, 0.0f).x;
	float32 ry = pRect->GetPosition(0.0f, 0.0f).y;
	float32 rw = pRect->GetWidth();
	float32 rh = pRect->GetHeight();

	Vector2f circlePos = pCircle->GetCenter();

	float32 nearestX = std::max(rx, std::min(circlePos.x, rx + rw));
	float32 nearestY = std::max(ry, std::min(circlePos.y, ry + rh));
	Vector2f nearest({ nearestX, nearestY });

	Vector2f delta = circlePos - nearest;
	float32 dist = delta.x * delta.x + delta.y * delta.y;

	if (dist == 0.0f)
	{
		float32 overlapL = circlePos.x - rx;
		float32 overlapR = (rx + rw) - circlePos.x;
		float32 overlapT = circlePos.y - ry;
		float32 overlapB = (ry + rh) - circlePos.y;

		float32 minOverlap = std::min({ overlapL, overlapR, overlapT, overlapB });

		Vector2f newPos = circlePos;
		if (minOverlap == overlapL)
		{
			newPos.x = rx - pCircle->GetRadius();
		}
		else if (minOverlap == overlapR)
		{
			newPos.x = rx + rw + pCircle->GetRadius();
		}
		else if (minOverlap == overlapT)
		{
			newPos.y = ry - pCircle->GetRadius();
		}
		else
		{
			newPos.y = ry + rh + pCircle->GetRadius();
		}

		pCircle->SetPosition(newPos.x, newPos.y, 0.5f, 0.5f);
		return;
	}

	float32 length = std::sqrt(dist);
	Vector2f normal = delta / length;
	float32  overlap = pCircle->GetRadius() - length;

	Vector2f newPos = circlePos + normal * overlap;
	pCircle->SetPosition(newPos.x, newPos.y, 0.5f, 0.5f);
}

void PhysicsManager::RepulseCircleRect(gcle::Shape* a, gcle::Shape* b)
{
	RepulseRectCircle(b, a);
}