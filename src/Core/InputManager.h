#pragma once
#include <iostream>
#include <unordered_map>
#include "include.h"
#include <Windows.h>
#include <Xinput.h>

#pragma comment(lib, "Xinput.lib")

namespace Mouse
{
	#define LeftButton VK_LBUTTON
	#define RightButton VK_RBUTTON
	#define MiddleButton VK_MBUTTON
	#define Button1 VK_XBUTTON1
	#define Button2 VK_XBUTTON2
}

namespace Keyboard
{
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



#define XBOX_START XINPUT_GAMEPAD_START
#define XBOX_BACK XINPUT_GAMEPAD_BACK

#define XBOX_PAD_UP XINPUT_GAMEPAD_DPAD_UP
#define XBOX_PAD_LEFT XINPUT_GAMEPAD_DPAD_LEFT
#define XBOX_PAD_RIGHT XINPUT_GAMEPAD_DPAD_RIGHT
#define XBOX_PAD_DOWN XINPUT_GAMEPAD_DPAD_DOWN

#define XBOX_A XINPUT_GAMEPAD_A
#define XBOX_B XINPUT_GAMEPAD_B
#define XBOX_X XINPUT_GAMEPAD_X
#define XBOX_Y XINPUT_GAMEPAD_Y

#define XBOX_LJS XINPUT_GAMEPAD_LEFT_THUMB
#define XBOX_RJS XINPUT_GAMEPAD_RIGHT_THUMB
#define XBOX_LB XINPUT_GAMEPAD_LEFT_SHOULDER
#define XBOX_RB XINPUT_GAMEPAD_RIGHT_SHOULDER



class InputManager
{
private:

	std::unordered_map<short, bool> m_keysDownReset;

	std::vector<short> m_LeftStickDeadzone = { 4000, -4000, 4000, -4000 };
	std::vector<short> m_RightStickDeadzone = { 4000, -4000, 4000, -4000 };

	BYTE m_LeftTriggerDeadzone = 30;
	BYTE m_RightTriggerDeadzone = 30;

	XINPUT_STATE m_State;

	BYTE m_GamepadIndex = 0; // this is supposed to be referring to which controller you're using (from 0 to 3) but it's not used for now

public:

	static InputManager& GetInstance()
	{
		static InputManager instance;
		return instance;
	}

	void Update();

	//Constructor

	InputManager() = default;

	//Destructor

	~InputManager() = default;


	//Input tests

	Vector2<long> GetMouseRelativePosition();
	Vector2<long> GetMouseWorldPosition();

	bool IsDown(const short key);
	bool IsHeld(const char key);
	bool IsUp(const char key);

	bool IsControllerDown(int16 key);

	void SetVibration(float32 powerLeft, float32 powerRight);


	XINPUT_STATE GetGamepadState();


	void SetLeftStickDeadZone(float Xpos, float Xneg, float Ypos, float Yneg);
	void SetRightStickDeadZone(float Xpos, float Xneg, float Ypos, float Yneg);
	std::vector<short> GetLeftStickDeadZone();
	std::vector<short> GetRightStickDeadZone();

	bool IsLeftStickInHorizontalDeadzone();
	bool IsLeftStickInVerticalDeadzone();
	bool IsRightStickInHorizontalDeadzone();
	bool IsRightStickInVerticalDeadzone();

	float GetLeftStickX();
	float GetLeftStickY();
	float GetRightStickX();
	float GetRightStickY();

	Vector2f LeftStickPressed();
	Vector2f RightStickPressed();


	float LeftTriggerPressed();
	float RightTriggerPressed();
};

