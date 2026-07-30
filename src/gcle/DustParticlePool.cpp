#include "DustParticlePool.h"
#include "DustParticle.h"
#include "Scene.h"

DustParticlePool::DustParticlePool(size_t capacity) :
    m_Capacity(capacity)
{
}

void DustParticlePool::Grow(Scene* pScene)
{
    m_Particles.reserve(m_Capacity);
    while (m_Particles.size() < m_Capacity)
    {
        m_Particles.push_back(pScene->CreateEntity<DustParticle>(gcle::Shapes::Rectangle));
    }
}

DustParticle* DustParticlePool::Acquire(Scene* pScene)
{
    if (m_Particles.empty())
    {
        if (pScene == nullptr)
        {
            return nullptr;
        }
        Grow(pScene);
    }

    DustParticle* pParticle = m_Particles[m_NextIndex];
    m_NextIndex = (m_NextIndex + 1) % m_Particles.size();
    return pParticle;
}

void DustParticlePool::Shutdown()
{
    for (DustParticle* pParticle : m_Particles)
    {
        if (pParticle != nullptr)
        {
            pParticle->Destroy();
        }
    }
    m_Particles.clear();
    m_NextIndex = 0;
}