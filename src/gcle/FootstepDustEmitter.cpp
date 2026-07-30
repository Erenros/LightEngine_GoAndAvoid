#include "FootstepDustEmitter.h"
#include "DustParticle.h"
#include "SceneManager.h"
#include "Scene.h"
#include "Utils.h"

namespace
{
    constexpr Color DUST_COLOR{ 250, 250, 245, 210 };
    constexpr float32 DUST_LIFETIME = 0.28f;
    constexpr float32 DUST_BASE_SCALE = 0.055f;
    constexpr float32 DUST_SCALE_VARIANCE = 0.025f;
    constexpr float32 DUST_STEP_OFFSET = 10.0f;
    constexpr int32 DUST_PARTICLES_PER_STEP = 2;
}

void FootstepDustEmitter::Update(float32 dt, Vector2f feetPosition, bool canEmit, bool facingRight)
{
    if (!canEmit)
    {
        m_Timer = 0.0f;
        return;
    }

    m_Timer -= dt;
    if (m_Timer > 0.0f)
    {
        return;
    }

    m_Timer = m_Interval;
    Spawn(feetPosition, facingRight);
    m_StepSideLeft = !m_StepSideLeft;
}

void FootstepDustEmitter::Spawn(Vector2f feetPosition, bool facingRight)
{
    Scene* pScene = SceneManager::GetInstance().GetCurrentScene();
    if (pScene == nullptr)
    {
        return;
    }

    float32 facing = facingRight ? 1.0f : -1.0f;
    float32 sideOffset = m_StepSideLeft ? -DUST_STEP_OFFSET : DUST_STEP_OFFSET;

    for (int32 i = 0; i < DUST_PARTICLES_PER_STEP; ++i)
    {
        Vector2f spawnPos
        {
            feetPosition.x + sideOffset + static_cast<float32>(GenerateRandomNumber(-4, 4)),
            feetPosition.y + static_cast<float32>(GenerateRandomNumber(-2, 2))
        };

        Vector2f velocity
        {
            -facing * static_cast<float32>(GenerateRandomNumber(15, 35)),
            -static_cast<float32>(GenerateRandomNumber(20, 45))
        };

        float32 scaleVariance = (static_cast<float32>(GenerateRandomNumber(0, 100)) / 100.0f) * DUST_SCALE_VARIANCE;
        float32 scale = DUST_BASE_SCALE + scaleVariance;

        DustParticle* pParticle = m_Pool.Acquire(pScene);
        if (pParticle != nullptr)
        {
            pParticle->Init(spawnPos, velocity, DUST_LIFETIME, scale, DUST_COLOR);
        }
    }
}

void FootstepDustEmitter::Shutdown()
{
    m_Pool.Shutdown();
}