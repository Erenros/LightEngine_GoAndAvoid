#include "Enemy.h"
#include "ScoreManager.h"

void Enemy::OnCollisionEnter(Entity* other)
{
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