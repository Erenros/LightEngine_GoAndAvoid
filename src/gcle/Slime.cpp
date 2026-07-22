#include "Slime.h"
#include <cstdlib>
#include <cmath>

Slime::Slime() : m_StateFrames(0), m_Speed(30.0f)
{
}

Slime::~Slime()
{
}

void Slime::OnInitialize()
{
    Enemy::OnInitialize();

    SetTag(1);
    SetRigidBody(true);
    SetStatic(false);

    GetRigidBody()->SetMass(1.0f);
    GetRigidBody()->SetMaxSpeed(50.0f);
    GetRigidBody()->SetGravity(false);
    GetRigidBody()->SetFriction({ 0.1f, 0.1f });
    GetRigidBody()->ActivateFriction(true);

    SetTexture("Slime");

    AddAnimation("Idle", 0, 3, 0, 47, 47, 0.15f);

    PlayAnimation("Idle", AnimationMode::Loop, AnimationInterrupt::Force);

    CreateCollider(gcle::Shapes::Rectangle, true, { 0.0f, 0.0f }, 0.0f, { 1.0f, 1.0f }, false);
}

void Slime::OnUpdate()
{
    Enemy::OnUpdate();

    m_StateFrames--;

    if (m_StateFrames <= 0)
    {
        m_StateFrames = 120 + (std::rand() % 120);

        float32 dirX = (static_cast<float32>(std::rand()) / static_cast<float32>(RAND_MAX)) * 2.0f - 1.0f;

        if (std::abs(dirX) < 0.2f)
        {
            SetDirection(0.0f, 0.0f, 0.0f);
            PlayAnimation("Idle", AnimationMode::Loop, AnimationInterrupt::Force);
        }
        else
        {
            SetDirection(dirX, 0.0f, m_Speed);
            PlayAnimation("Move", AnimationMode::Loop, AnimationInterrupt::Force);
        }
    }
}

void Slime::OnCollisionEnter(Entity* other)
{
    Enemy::OnCollisionEnter(other);

    m_StateFrames = 0;
}