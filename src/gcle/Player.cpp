#include "Player.h"
#include "GameManager.h"

void Player::OnUpdate() {
    InputManager& im = InputManager::GetInstance();
    RigidBody2D* rb = GetRigidBody();
    float32 dt = static_cast<float32>(GameManager::GetInstance().GetTime()->GetDeltaTime());
    
    constexpr float MOVE_FORCE = 600.0f;

    if (im.IsHeld('Q')) {
        rb->AddForce({ -1, 0 }, MOVE_FORCE, dt);
    }
    if (im.IsHeld('D')) {
        rb->AddForce({ 1, 0 }, MOVE_FORCE, dt);
    }
    if (im.IsHeld('S')) {
        rb->AddForce({ 0, 1 }, MOVE_FORCE, dt);
    }
    if (im.IsHeld('Z')) {
        rb->AddForce({ 0, -1 }, MOVE_FORCE, dt);
    }
    if (im.IsHeld('B')) {
        rb->AddImpulse({ 0, -1 }, MOVE_FORCE);
    }
}

void Player::OnInitialize() 
{
    RigidBody2D* rb = GetRigidBody();    
    rb->SetFriction({ 0.15f, 0.15f });
}