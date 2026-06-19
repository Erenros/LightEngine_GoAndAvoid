#include "Player.h"

void Player::OnUpdate(){
	InputManager& im = InputManager::GetInstance();

	Vector2f pos = GetPosition();
	if (im.IsHeld('Q')) {
		SetPosition(pos.x - 5, pos.y);
		pos = GetPosition();
	}
	if (im.IsHeld('D')) {
		SetPosition(pos.x + 5, pos.y);
		pos = GetPosition();
	}
	if (im.IsHeld('S')) {
		SetPosition(pos.x, pos.y + 5);
		pos = GetPosition();
	}
	if (im.IsHeld('Z')) {
		SetPosition(pos.x, pos.y - 5);
		pos = GetPosition();
	}
}
