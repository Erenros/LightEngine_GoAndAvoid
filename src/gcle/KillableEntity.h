#pragma once
#include "Entity.h"
#include <unordered_set>

class KillableEntity : public Entity
{
protected:
    int m_maxHp = 0;
    int m_hp = 0;
    int m_dmg = 0;

    float32 m_velX = 0.0f;
    float32 m_velY = 0.0f;
    float32 m_gravity = 2600.0f;
    bool m_grounded = false;
    std::unordered_set<Entity*> m_groundContacts;

    virtual void OnDeath() { Destroy(); }
    void OnInitialize() override;
    void OnUpdate() override;
    void OnCollision(Entity* other) override;
    void OnCollisionExit(Entity* other) override;

public:
    int GetHp() const { return m_hp; }
    int GetMaxHp() const { return m_maxHp; }
    int GetDamage() const { return m_dmg; }

    void TakeDamage(int dmg);
};