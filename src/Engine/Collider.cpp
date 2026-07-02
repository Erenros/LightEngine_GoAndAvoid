#include "Collider.h"
#include "Entity.h"

void Collider::Initialize(Shape* shape, Vector2f position, float32 rotation, Entity* owner)
{
    m_shape = shape;
    mp_Owner = owner;
    m_shape->SetPosition(position.x, position.y);
	m_shape->SetRotation(rotation);
    m_shape->GetTransform()->SetParent(&mp_Owner->GetTransform2D());
}

void Collider::CollidingOn(Vector2f direction)
{
    if (direction.x < 0.0f)
        m_CollisionDirection.IsCollidingOnRight = true;
    else if (direction.x > 0.0f)
        m_CollisionDirection.IsCollidingOnLeft = true;

    if (direction.y > 0.0f)
        m_CollisionDirection.IsCollidingOnTop = true;
    else if (direction.y < 0.0f)
        m_CollisionDirection.IsCollidingOnBottom = true;
}

void Collider::CollidingOnX(float32 direction)
{
    if (direction < 0.0f)
        m_CollisionDirection.IsCollidingOnRight = true;
    else if (direction > 0.0f)
        m_CollisionDirection.IsCollidingOnLeft = true;
}

void Collider::CollidingOnY(float32 direction)
{
    if (direction > 0.0f)
        m_CollisionDirection.IsCollidingOnTop = true;
    else if (direction < 0.0f)
        m_CollisionDirection.IsCollidingOnBottom = true;
}

void Collider::StoppedColliding()
{
    m_CollisionDirection.IsCollidingOnRight = false;
    m_CollisionDirection.IsCollidingOnLeft = false;
    m_CollisionDirection.IsCollidingOnTop = false;
    m_CollisionDirection.IsCollidingOnBottom = false;
}

bool Collider::GetInQuerySeen() {
    return m_inQuerySeen;
}

void Collider::SetInQuerySeen(bool seen) {
    m_inQuerySeen = seen;
}

AABB Collider::GetAABB(){
    return m_aabb;
}

void Collider::SetAABB(AABB aabb){
    m_aabb = aabb;
}
