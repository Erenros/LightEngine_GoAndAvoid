#pragma once
#include "LevelEntry.h"
#include "Slime.h"

class SlimeEntry : public LevelEntry
{
public:
    SlimeEntry(Vector2f position, Vector2f scale);

    void Spawn(Scene* pScene) override;
    void Despawn() override;

private:
#ifdef _DEBUG
    Entity* mp_DebugEntity = nullptr;
#endif
};