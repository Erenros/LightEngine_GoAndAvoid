#include "KillableEntity.h"

void KillableEntity::TakeDamage(int dmg)
{
    m_hp -= dmg;
    if (m_hp <= 0) OnDeath();
}
