#include "DustParticle.h"
#include "GameManager.h"
#include <algorithm>

#undef min
#undef max

namespace
{
    constexpr float32 DUST_GRAVITY = 35.0f;
    constexpr float32 DUST_DRAG = 5.5f;
    constexpr float32 DUST_GROWTH = 0.3f;
}

void DustParticle::OnInitialize()
{
    SetStatic(true);
    GetRigidBody()->SetActive(false);
}

void DustParticle::Init(Vector2f position, Vector2f velocity, float32 lifetime, float32 startScale, Color color)
{
    SetPosition(position.x, position.y);
    SetScale({ startScale, startScale });
    SetColor(color);

    m_Velocity = velocity;
    m_MaxLife = lifetime > 0.0f ? lifetime : 0.01f;
    m_Life = m_MaxLife;
    m_StartScale = startScale;
    m_BaseColor = color;
}

void DustParticle::OnUpdate()
{
    float32 dt = static_cast<float32>(GameManager::GetInstance().GetTime()->GetDeltaTime());

    m_Life -= dt;
    if (m_Life <= 0.0f)
    {
        Destroy();
        return;
    }

    m_Velocity.y += DUST_GRAVITY * dt;
    m_Velocity *= (1.0f - std::min(1.0f, DUST_DRAG * dt));

    Vector2f pos = GetPosition();
    SetPosition(pos.x + m_Velocity.x * dt, pos.y + m_Velocity.y * dt);

    float32 t = 1.0f - (m_Life / m_MaxLife);
    float32 scale = m_StartScale * (1.0f + t * DUST_GROWTH);
    SetScale({ scale, scale });

    byte alpha = static_cast<byte>(static_cast<float32>(m_BaseColor.a) * (1.0f - t));
    SetColor({ m_BaseColor.r, m_BaseColor.g, m_BaseColor.b, alpha });
}