#include "Player.h"

void Player::OnUpdate(){
	InputManager& im = InputManager::GetInstance();

	
	if (im.IsHeld('Q')) {
		GetRigidBody().AddForce({ -1, 0 }, 100, 0.016f);
	}
	if (im.IsHeld('D')) {
		GetRigidBody().AddForce({ 1, 0 }, 100, 0.016f);
	}
	if (im.IsHeld('S')) {
		GetRigidBody().AddForce({ 0, 1 }, 100, 0.016f);
	}
	if (im.IsHeld('Z')) {
		GetRigidBody().AddForce({ 0, -1 }, 100, 0.016f);		 
	}
}
