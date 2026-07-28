#include "Enemy.h"
#include "ScoreManager.h"

void Enemy::OnCollisionEnter(Entity* other)
{
    if (other == nullptr || dynamic_cast<Enemy*>(other) != nullptr)
    {
        return;
    }

    KillableEntity* victim = dynamic_cast<KillableEntity*>(other);
    if (victim != nullptr)
    {
        victim->TakeDamage(1);
    }
}

void Enemy::OnDeath()
{
    ScoreManager::GetInstance().AddScore(m_scoreReward);
    KillableEntity::OnDeath();
}