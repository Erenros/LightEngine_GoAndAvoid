#pragma once
#include "include.h"

class Scene;
class Entity;

class LevelEntry
{
public:
    LevelEntry(Vector2f position, Vector2f scale);
    virtual ~LevelEntry() = default;

    virtual void Spawn(Scene* pScene) = 0;
    virtual void Despawn() = 0;

    bool IsSpawned() const;
    float32 GetX() const;

protected:
    Vector2f m_Position;
    Vector2f m_Scale;
    Entity* mp_Entity = nullptr;
};