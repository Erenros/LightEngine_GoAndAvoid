#pragma once
#include <iostream>
#include <unordered_map>
#include "SDL_events.h"

class InputManager
{
	public:

		struct Keystate
		{
			bool isPressed;
			bool isHeld;
			bool isReleased;
		};

		std::unordered_map<SDL_Keycode, Keystate> keyMap;

		void HandleInput();
};

