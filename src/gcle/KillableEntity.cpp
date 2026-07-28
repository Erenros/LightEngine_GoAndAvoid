#include "KillableEntity.h"
#include "GameManager.h"

namespace
{
    constexpr int32 KILLABLE_LAYER = 2;
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
    if (other != nullptr && other->IsTag(1))
    {
        m_groundContacts.insert(other);
        if (m_velY > 0.0f)
        {
            m_velY = 0.0f;
        }
    }
}

void KillableEntity::OnCollisionExit(Entity* other)
{
    if (other != nullptr && other->IsTag(1))
    {
        m_groundContacts.erase(other);
    }
}