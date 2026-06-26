#include "Collider.h"
#include "Entity.h"

void Collider::Initialize(Shape* shape, Vector2f position, Entity* owner)
{
    m_shape = shape;
    mp_Owner = owner;
    m_shape->SetPosition(position.x, position.y);
    m_shape->GetTransform()->SetParent(mp_Owner->GetShape()->GetTransform());
}

void Collider::CollidingOn(Vector2f direction)
{
    m_CollisionDirection.IsCollidingOnRight = direction.x > 0;
    m_CollisionDirection.IsCollidingOnLeft = direction.x < 0;
    m_CollisionDirection.IsCollidingOnTop = direction.y > 0;
    m_CollisionDirection.IsCollidingOnBottom = direction.y < 0;
}

void Collider::CollidingOnX(float32 direction)
{
    m_CollisionDirection.IsCollidingOnRight = direction > 0;
    m_CollisionDirection.IsCollidingOnLeft = direction < 0;
}

void Collider::CollidingOnY(float32 direction)
{
    m_CollisionDirection.IsCollidingOnTop = direction > 0;
    m_CollisionDirection.IsCollidingOnBottom = direction < 0;
}

void Collider::StoppedColliding()
{
    m_CollisionDirection.IsCollidingOnRight = false;
    m_CollisionDirection.IsCollidingOnLeft = false;
    m_CollisionDirection.IsCollidingOnTop = false;
    m_CollisionDirection.IsCollidingOnBottom = false;
}