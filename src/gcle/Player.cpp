#include "Player.h"
#include "GameManager.h"


void Player::OnUpdate() {
    InputManager& im = InputManager::GetInstance();
    RigidBody2D rb = GetRigidBody();


    if (im.IsHeld('Q')) {
        GetRigidBody().AddForce({ -1, 0 }, 600, 0.006);
    }
    if (im.IsHeld('D')) {
        GetRigidBody().AddForce({ 1, 0 }, 600, 0.006);
    }
    if (im.IsHeld('S')) {
        GetRigidBody().AddForce({ 0, 1 }, 600, 0.006);
    }
    if (im.IsHeld('Z')) {
        GetRigidBody().AddForce({ 0, -1 }, 600, 0.006);
    }
}