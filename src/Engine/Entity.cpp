#include "Entity.h"

void Entity::Initialize()
{
}

void Entity::Repulse(Entity* other)
{
}

void Entity::Update()
{
}

void Entity::Destroy()
{
}

bool Entity::IsInside(float x, float y) const
{
    return false;
}

bool Entity::IsColliding(Entity* other) const
{
    return false;
}

bool Entity::GoToPosition(int x, int y, float speed)
{
    return false;
}

bool Entity::GoToDirection(int x, int y, float speed)
{
    return false;
}

Vector2f Entity::GetPosition(float ratioX, float ratioY) const
{
    return Vector2f();
}

void Entity::SetDirection(float x, float y, float speed)
{
}

void Entity::SetPosition(float x, float y, float ratioX, float ratioY)
{
}
