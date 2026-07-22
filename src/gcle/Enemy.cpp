#include "Enemy.h"

void Enemy::OnCollisionEnter(Entity* other)
{
    KillableEntity* victim = dynamic_cast<KillableEntity*>(other);
    if (victim != nullptr)
    {
        victim->TakeDamage(1);
    }
}