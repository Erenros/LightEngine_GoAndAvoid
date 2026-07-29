#include "Enemy.h"
#include "ScoreManager.h"
#include "GameManager.h"
#include "SceneManager.h"
#include "Scene.h"
#include "Camera.h"
#include "Collider.h"

namespace
{
    constexpr float32 DEATH_HOP_SPEED = -450.0f;
    constexpr float32 DEATH_ROTATION_SPEED = 240.0f;
    constexpr float32 DEATH_KNOCKBACK_FACTOR = 0.35f;
    constexpr float32 DEATH_MAX_SPEED = 900.0f;
    constexpr float32 DEATH_MAX_DURATION = 3.0f;
    constexpr float32 DEATH_SCREEN_MARGIN = 150.0f;
    constexpr float32 DEATH_ABSOLUTE_Y_LIMIT = 5000.0f;
    constexpr float32 SCREEN_HALF_HEIGHT = 540.0f;
}

void Enemy::OnUpdate()
{
    if (m_isDying)
    {
        float32 dt = static_cast<float32>(GameManager::GetInstance().GetTime()->GetDeltaTime());
        UpdateDeathAnimation(dt);
        return;
    }

    KillableEntity::OnUpdate();
    OnAliveUpdate();
}

void Enemy::OnCollisionEnter(Entity* other)
{
    if (m_isDying || other == nullptr || dynamic_cast<Enemy*>(other) != nullptr)
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
    if (m_isDying)
    {
        return;
    }

    ScoreManager::GetInstance().AddScore(m_scoreReward);
    StartDeathSequence();
}

void Enemy::OnDestroy()
{
    if (m_onDestroyedCallback)
    {
        m_onDestroyedCallback();
    }

    KillableEntity::OnDestroy();
}

void Enemy::StartDeathSequence()
{
    m_isDying = true;
    m_deathElapsedTime = 0.0f;
    m_deathFallSpeed = DEATH_HOP_SPEED;

    for (Collider* collider : GetColliders())
    {
        if (collider != nullptr)
        {
            collider->SetActive(false);
        }
    }

    RigidBody2D* rb = GetRigidBody();
    if (rb != nullptr)
    {
        rb->SetMaxSpeed(DEATH_MAX_SPEED);
        rb->SetVelocity({ m_velX * DEATH_KNOCKBACK_FACTOR, m_deathFallSpeed });
    }

    StopAnimation();
}

void Enemy::UpdateDeathAnimation(float32 dt)
{
    m_deathElapsedTime += dt;
    m_deathFallSpeed += m_gravity * dt;

    RigidBody2D* rb = GetRigidBody();
    if (rb != nullptr)
    {
        rb->SetVelocity({ m_velX * DEATH_KNOCKBACK_FACTOR, m_deathFallSpeed });
    }

    Rotate(DEATH_ROTATION_SPEED * dt);

    if (m_deathElapsedTime >= DEATH_MAX_DURATION || IsOffScreen() || GetPosition().y > DEATH_ABSOLUTE_Y_LIMIT)
    {
        Destroy();
    }
}

bool Enemy::IsOffScreen()
{
    Scene* scene = SceneManager::GetInstance().GetCurrentScene();
    if (scene == nullptr)
    {
        return false;
    }

    Camera* camera = scene->GetCurrentCamera();
    if (camera == nullptr)
    {
        return false;
    }

    float32 screenBottom = camera->GetPosition().y + SCREEN_HALF_HEIGHT;
    return GetPosition().y - screenBottom > DEATH_SCREEN_MARGIN;
}