#pragma once
#include <cstddef>
#include <vector>

class Scene;
class DustParticle;

class DustParticlePool
{
public:
    explicit DustParticlePool(size_t capacity = 16);

    DustParticle* Acquire(Scene* pScene);
    void Shutdown();

private:
    void Grow(Scene* pScene);

    std::vector<DustParticle*> m_Particles;
    size_t m_Capacity;
    size_t m_NextIndex = 0;
};