#pragma once
#include "Entity.h"

class Collider;

class Hitbox : public Entity
{
public:
    void SetOwner(Entity* owner) { m_owner = owner; }

    void Activate(Vector2f offset, Vector2f size, int32 damage, float32 duration);
    void Deactivate();

protected:
    void OnInitialize() override;
    void OnUpdate() override;
    void OnCollisionEnter(Entity* other) override;
    void OnCollision(Entity* other) override;

    virtual void OnHit(Entity* victim);

private:
    void TryHit(Entity* other);

    Entity* m_owner = nullptr;
    Collider* mp_col = nullptr;
    Vector2f m_offset = { 0.0f, 0.0f };
    int32 m_damage = 0;
    float32 m_life = 0.0f;
    bool m_hasHit = false;
};