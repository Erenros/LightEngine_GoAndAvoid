#include "PhysicsManager.h"

PhysicsManager::CollisionFn PhysicsManager::collisionTable[static_cast<int32>(gcle::Shapes::Count) - 1][static_cast<int32>(gcle::Shapes::Count) - 1]
{
	{
		&PhysicsManager::CheckRectRect,
		&PhysicsManager::CheckRectCircle,
	},
	{
		&PhysicsManager::CheckCircleCircle,
		&PhysicsManager::CheckCircleRect,
	}
};

PhysicsManager& PhysicsManager::GetInstance() 
{
	static PhysicsManager instance;
	return instance;
}

void PhysicsManager::Update(float64 deltaTime)
{
}

bool PhysicsManager::IsColliding(Entity* pEntity1, Entity* pEntity2)
{
	auto* shapeA = pEntity1->GetShape();
	auto* shapeB = pEntity2->GetShape();

	int32 typeA = static_cast<int32>(shapeA->GetShape()) - 1;
	int32 typeB = static_cast<int32>(shapeB->GetShape()) - 1;

	return (this->*collisionTable[typeA][typeB])(shapeA, shapeB);
}

bool PhysicsManager::IsInside(Entity* pEntity, Vector2f positionToCheck)
{
	return false;
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
	float32 rx = pRect->GetPosition().x;
	float32 ry = pRect->GetPosition().y;
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

	return dist <= pCircle->GetRadius();
}

bool PhysicsManager::CheckCircleCircleCollision(gcle::Circle* pCircle1, gcle::Circle* pCircle2)
{
	float32 distance = pCircle1->GetPosition(0.5f, 0.5f).GetDistance(pCircle2->GetPosition(0.5f, 0.5f));
	return distance <= pCircle1->GetRadius() + pCircle2->GetRadius();
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

