#pragma once
#include <iostream>
#include <unordered_map>
#include "include.h"
#include <Windows.h>



namespace Mouse{
	#define LeftButton VK_LBUTTON
	#define RightButton VK_RBUTTON
	#define MiddleButton VK_MBUTTON
	#define Button1 VK_XBUTTON1
	#define Button2 VK_XBUTTON2
}

namespace Keyboard {
	#define Space VK_SPACE
	#define Shift VK_SHIFT
	#define Escape 0x1B
	#define Tab 0x09
	#define Backspace 0x08

	#define LeftArrow VK_LEFT
	#define RightArrow VK_RIGHT
	#define UpArrow VK_UP
	#define DownArrow VK_DOWN

    #define Delete VK_DELETE

	#define F1 VK_F1
	#define F2 VK_F2
	#define F3 VK_F3
	#define F4 VK_F4
	#define F5 VK_F5
	#define F6 VK_F6
	#define F7 VK_F7
	#define F8 VK_F8
	#define F9 VK_F9
	#define F10 VK_F10
	#define F11 VK_F11
	#define F12 VK_F12


}


class InputManager
{
private:

	std::unordered_map<short, bool> m_keysDownReset;

public:

	static InputManager& GetInstance() {
		static InputManager instance;
		return instance;
	}

	void Update();

	//Constructor

	InputManager() = default;

	//Destructor

	~InputManager() = default;


	//Input tests

	bool IsDown(const short key);
	bool IsHeld(const char key);
	bool IsUp(const char key);


	Vector2<long> GetMouseRelativePosition();
	Vector2<long> GetMouseWorldPosition();

	
};

