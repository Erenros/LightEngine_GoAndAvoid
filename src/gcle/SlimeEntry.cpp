#include "SlimeEntry.h"
#include "Scene.h"

#ifdef _DEBUG
#include "DebugKillableEntity.h"
#endif

SlimeEntry::SlimeEntry(Vector2f position, Vector2f scale) :
    LevelEntry(position, scale)
{
}

void SlimeEntry::Spawn(Scene* pScene)
{
    if (pScene == nullptr || IsSpawned())
    {
        return;
    }

    Slime* pSlime = pScene->CreateEntity<Slime>(gcle::Shapes::Rectangle);
    pSlime->SetPosition(m_Position.x, m_Position.y);
    pSlime->SetScale(m_Scale);

    mp_Entity = pSlime;

#ifdef _DEBUG
    DebugKillableEntity* pDebug = pScene->CreateEntity<DebugKillableEntity>(gcle::Shapes::Rectangle);
    pDebug->SetTarget(pSlime);
    mp_DebugEntity = pDebug;
#endif
}

void SlimeEntry::Despawn()
{
    if (!IsSpawned())
    {
        return;
    }

    mp_Entity->Destroy();
    mp_Entity = nullptr;

#ifdef _DEBUG
    if (mp_DebugEntity != nullptr)
    {
        mp_DebugEntity->Destroy();
        mp_DebugEntity = nullptr;
    }
#endif
}