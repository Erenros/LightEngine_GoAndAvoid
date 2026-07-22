#include "KillableEntity.h"
#include "GameManager.h"

void KillableEntity::TakeDamage(int dmg)
{
    m_hp -= dmg;
    if (m_hp <= 0) OnDeath();
}

void KillableEntity::OnInitialize()
{
    SetStatic(true);
    SetRigidBody(true);

    RigidBody2D* rb = GetRigidBody();
    if (rb != nullptr)
    {
        rb->SetActive(true);
        rb->SetGravity(false);
        rb->SetCollisionOnContinuous();
    }
}

void KillableEntity::OnUpdate()
{
    float32 dt = static_cast<float32>(GameManager::GetInstance().GetTime()->GetDeltaTime());
    RigidBody2D* rb = GetRigidBody();

    if (rb == nullptr)
    {
        return;
    }

    m_grounded = (!m_groundContacts.empty());

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