#include "Player.h"

void Player::OnUpdate() {
    InputManager& im = InputManager::GetInstance();
    RigidBody2D rb = GetRigidBody();


    if (im.IsHeld('Q')) {
        GetRigidBody().SetForce({ -1, 0 }, 100);
    }
    if (im.IsHeld('D')) {
        GetRigidBody().SetForce({ 1, 0 }, 100);
    }
    if (im.IsHeld('S')) {
        GetRigidBody().SetForce({ 0, 1 }, 100);
    }
    if (im.IsHeld('Z')) {
        GetRigidBody().SetForce({ 0, -1 }, 100);
    }
}