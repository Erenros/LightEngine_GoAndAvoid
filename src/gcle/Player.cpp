#include "Player.h"
#include "GameManager.h"


void Player::OnUpdate() {
    InputManager& im = InputManager::GetInstance();
    RigidBody2D rb = GetRigidBody();


    if (im.IsHeld('Q')) {
        GetRigidBody().AddForce({ -1.f, 0.f }, 600.f, 0.006f);
    }
    if (im.IsHeld('D')) {
        GetRigidBody().AddForce({ 1.f, 0.f }, 600.f, 0.006f);
    }
    if (im.IsHeld('S')) {
        GetRigidBody().AddForce({ 0.f, 1.f }, 600.f, 0.006f);
    }
    if (im.IsHeld('Z')) {
        GetRigidBody().AddForce({ 0.f, -1.f }, 600.f, 0.006f);
    }
}