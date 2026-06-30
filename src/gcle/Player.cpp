#include "Player.h"
#include "GameManager.h"

void Player::OnUpdate() {
    InputManager& im = InputManager::GetInstance();
    RigidBody2D rb = GetRigidBody();
    Seconds dt = GameManager::GetInstance().GetTime()->GetDeltaTime();
    

    if (im.IsHeld('Q')) {
        GetRigidBody().AddForce({ -1, 0 }, 600, dt);
    }
    if (im.IsHeld('D')) {
        GetRigidBody().AddForce({ 1, 0 }, 600, dt);
    }
    if (im.IsHeld('S')) {
        GetRigidBody().AddForce({ 0, 1 }, 600, dt);
    }
    if (im.IsHeld('Z')) {
        GetRigidBody().AddForce({ 0, -1 }, 600, dt);
    }
    if (im.IsHeld('B')) {
        GetRigidBody().AddImpulse({ 0, -1 }, 600);
    }
}

void Player::OnInitialize() {
    RigidBody2D rb = GetRigidBody();
    rb.SetDampingStrenght(1000);
}