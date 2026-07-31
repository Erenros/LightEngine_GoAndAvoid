#include "KillableEntity.h"
#include "GameManager.h"
#include "Collider.h"
#include <cmath>

namespace
{
    constexpr int32 KILLABLE_LAYER = 2;

    bool ShapesOverlapAsFloor(gcle::Shape* selfShape, gcle::Shape* otherShape)
    {
        if (selfShape == nullptr || otherShape == nullptr)
        {
            return false;
        }

        Vector2f selfPos = selfShape->GetPosition(0.5f, 0.5f);
        Vector2f otherPos = otherShape->GetPosition(0.5f, 0.5f);

        float32 selfHalfW = selfShape->GetWidth() * 0.5f;
        float32 selfHalfH = selfShape->GetHeight() * 0.5f;
        float32 otherHalfW = otherShape->GetWidth() * 0.5f;
        float32 otherHalfH = otherShape->GetHeight() * 0.5f;

        if (selfHalfW <= 0.0f || selfHalfH <= 0.0f || otherHalfW <= 0.0f || otherHalfH <= 0.0f)
        {
            return false;
        }

        float32 overlapX = (selfHalfW + otherHalfW) - std::abs(selfPos.x - otherPos.x);
        float32 overlapY = (selfHalfH + otherHalfH) - std::abs(selfPos.y - otherPos.y);

        if (overlapX <= 0.0f || overlapY <= 0.0f)
        {
            return false;
        }

        return overlapY <= overlapX && selfPos.y <= otherPos.y;
    }

    bool HasFloorContact(Entity* self, Entity* other)
    {
        for (Collider* selfCollider : self->GetColliders())
        {
            if (selfCollider == nullptr || !selfCollider->IsActive())
            {
                continue;
            }

            for (Collider* otherCollider : other->GetColliders())
            {
                if (otherCollider == nullptr || !otherCollider->IsActive())
                {
                    continue;
                }

                if (ShapesOverlapAsFloor(selfCollider->GetShape(), otherCollider->GetShape()))
                {
                    return true;
                }
            }
        }

        return false;
    }
}

void KillableEntity::TakeDamage(int dmg)
{
    m_hp -= dmg;
    if (m_hp <= 0) OnDeath();
}

void KillableEntity::OnInitialize()
{
    SetStatic(true);
    SetRigidBody(true);
    SetLayer(KILLABLE_LAYER);

    RigidBody2D* rb = GetRigidBody();
    if (rb != nullptr)
    {
        rb->SetActive(true);
        rb->SetGravity(false);
        rb->SetCollisionOnContinuous();
    }
}

bool KillableEntity::IsGrounded()
{
    for (auto it = m_groundContacts.begin(); it != m_groundContacts.end(); )
    {
        if (*it == nullptr || (*it)->ToDestroy())
        {
            it = m_groundContacts.erase(it);
        }
        else
        {
            ++it;
        }
    }

    return !m_groundContacts.empty();
}

void KillableEntity::OnUpdate()
{
    float32 dt = static_cast<float32>(GameManager::GetInstance().GetTime()->GetDeltaTime());
    RigidBody2D* rb = GetRigidBody();

    if (rb == nullptr)
    {
        return;
    }

    m_grounded = IsGrounded();

    if (m_grounded && m_velY >= 0.0f)
    {
        m_velY = 0.0f;
    }
    else
    {
        m_velY += m_gravity * dt;
    }

    rb->SetVelocity({ m_velX, m_velY });
}

void KillableEntity::OnCollision(Entity* other)
{
    if (other == nullptr || !other->IsTag(1))
    {
        return;
    }

    if (!HasFloorContact(this, other))
    {
        return;
    }

    m_groundContacts.insert(other);
    if (m_velY > 0.0f)
    {
        m_velY = 0.0f;
    }
}

void KillableEntity::OnCollisionEnter(Entity* other)
{
    OnCollision(other);
}

void KillableEntity::OnCollisionExit(Entity* other)
{
    if (other != nullptr && other->IsTag(1))
    {
        m_groundContacts.erase(other);
    }
}