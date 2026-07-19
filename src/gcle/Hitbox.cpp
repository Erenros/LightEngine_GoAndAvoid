#include "Hitbox.h"
#include "KillableEntity.h"
#include "Collider.h"
#include "GameManager.h"

void Hitbox::OnInitialize()
{
    SetRigidBody(true);
    GetRigidBody()->SetActive(false);
    GetRigidBody()->SetGravity(false);
    SetStatic(false);

    SetRenderSize(0, { 0.0f, 0.0f });

    mp_col = CreateCollider(gcle::Shapes::Rectangle, false, { 0.0f, 0.0f }, 0, { 1.0f, 1.0f });
}

void Hitbox::Activate(Vector2f offset, Vector2f size, int32 damage, float32 duration)
{
    m_offset = offset;
    m_damage = damage;
    m_life = duration;
    m_hasHit = false;

    if (mp_col != nullptr)
    {
        mp_col->GetShape()->SetScale(size);
        mp_col->SetActive(true);
    }
}

void Hitbox::Deactivate()
{
    m_life = 0.0f;
    if (mp_col != nullptr)
    {
        mp_col->SetActive(false);
    }
}

void Hitbox::OnUpdate()
{
    if (m_owner == nullptr)
    {
        return;
    }

    float32 dir = (m_owner->GetTransform2D().GetScale().x >= 0.0f) ? 1.0f : -1.0f;
    Vector2f p = m_owner->GetTransform2D().GetPosition();
    SetPosition(p.x + dir * m_offset.x, p.y + m_offset.y);

    if (m_life > 0.0f)
    {
        float32 dt = static_cast<float32>(GameManager::GetInstance().GetTime()->GetDeltaTime());
        if (dt > 0.02f)
        {
            dt = 0.02f;
        }
        m_life -= dt;
        if (m_life <= 0.0f)
        {
            Deactivate();
        }
    }
}

void Hitbox::TryHit(Entity* other)
{
    if (m_owner == nullptr || m_hasHit || m_life <= 0.0f)
    {
        return;
    }

    KillableEntity* victim = dynamic_cast<KillableEntity*>(other);
    if (victim == nullptr || victim == m_owner)
    {
        return;
    }

    victim->TakeDamage(m_damage);
    m_hasHit = true;

    OnHit(victim);

    Deactivate();
}

void Hitbox::OnCollisionEnter(Entity* other)
{
    TryHit(other);
}

void Hitbox::OnCollision(Entity* other)
{
    TryHit(other);
}

void Hitbox::OnHit(Entity* victim)
{
}