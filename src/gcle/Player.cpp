#include "Player.h"
#include "GameManager.h"

void Player::OnInitialize()
{
    RigidBody2D* rb = GetRigidBody();
    if (rb != nullptr)
    {
        rb->SetFriction({ 0.0f, 0.15f });
    }
    m_input.SetMapping(InputMapping::Keyboard, 0);

    m_hp = 1;
    m_maxHp = 1;
}

void Player::SetupPlayer(InputMapping mapping, int8 padIndex)
{
    m_input.SetMapping(mapping, padIndex);
}

void Player::SetCustomKeys(int16 left, int16 right, int16 crouch, int16 jump, int16 skill0, int16 skill1)
{
    m_input.m_keyLeft = left;
    m_input.m_keyRight = right;
    m_input.m_keyCrouch = crouch;
    m_input.m_keyJump = jump;
    m_input.m_keySkill0 = skill0;
    m_input.m_keySkill1 = skill1;
}

void Player::OnUpdate()
{
    m_currentInputs = m_input.Read();
    RigidBody2D* rb = GetRigidBody();

    if (rb == nullptr)
    {
        return;
    }

    Vector2f currentVel = rb->GetVelocity();
    rb->SetVelocity({ 400.0f, currentVel.y });

    if (m_currentInputs.jump)
    {
        rb->SetVelocity({ 400.0f, 0.0f });
        rb->AddImpulse({ 0.0f, -1.0f }, 600.0f);
    }
}

void Player::OnDeath()
{
    Destroy();
}