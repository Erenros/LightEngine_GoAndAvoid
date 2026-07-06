#include "InputManager.h"



void InputManager::Update()
{
	std::unordered_map<short, bool>::iterator it;

	for (it = m_keysDownReset.begin(); it != m_keysDownReset.end(); it++)
	{
		if (it->second == false)
		{
			if ((GetAsyncKeyState(it->first) & 0x8000) == 0)
			{
				it->second = true;
			}
		}
	}

	for (int8 i = 0; i < m_Controllers.size(); i++)
	{
		m_Controllers[i]->m_State = GetGamepadState(i);
	}
}




Vector2<long> InputManager::GetMouseRelativePosition()
{
	POINT pos;

	GetCursorPos(&pos);
	return { pos.x, pos.y };
}

//TO DO
Vector2<long> InputManager::GetMouseWorldPosition()
{
	return Vector2<long>();
}






bool InputManager::IsDown(const short key)
{
	if ((GetAsyncKeyState(key) & 0x8000) != 0)
	{
		if (m_keysDownReset.find(key) == m_keysDownReset.end())
		{
			m_keysDownReset[key] = false;
			return true;
		}

		else
		{
			if (m_keysDownReset[key] == true)
			{
				m_keysDownReset[key] = false;
				return true;
			}

			else
			{
				return false;
			}
		}
	}

	return false;
}

bool InputManager::IsHeld(const char key){
	return ((GetAsyncKeyState(key) & 0x8000) != 0);
}

bool InputManager::IsUp(const char key){
	return ((GetAsyncKeyState(key) & 0x8000) == 0);
}

void InputManager::AddController(Controller* c)
{
	m_Controllers.push_back(c);
}



bool InputManager::IsControllerDown(int8 controller, int16 key)
{ 
	XINPUT_STATE state;
	if (!XInputGetState(controller, &state) == ERROR_SUCCESS)
		return false;
	 
	if (state.Gamepad.wButtons & key)
	{
		return true;
	}

	return false;
}

void InputManager::SetVibration(int8 controller, float32 powerLeft, float32 powerRight)
{
	XINPUT_VIBRATION VibrationState;

	ZeroMemory(&VibrationState, sizeof(XINPUT_VIBRATION));

	int iLeftMotor = int(powerLeft * 65535.0f);
	int iRightMotor = int(powerRight * 65535.0f);

	VibrationState.wLeftMotorSpeed = iLeftMotor;
	VibrationState.wRightMotorSpeed = iRightMotor;

	XInputSetState(controller, &VibrationState);
}





XINPUT_STATE InputManager::GetGamepadState(int8 controller)
{
	XINPUT_STATE GamepadState;

	ZeroMemory(&GamepadState, sizeof(XINPUT_STATE));

	XInputGetState(controller, &GamepadState);

	return GamepadState;
}



void InputManager::SetLeftStickDeadZone(int8 controller, float Xpos, float Xneg, float Ypos, float Yneg)
{
	m_Controllers[controller]->m_LeftStickDeadzone = { static_cast<short>(Xpos * 32767.0f), static_cast<short>(Xneg * 32768.0f), static_cast<short>(Ypos * 32767.0f), static_cast<short>(Yneg * 32768.0f) };
}

void InputManager::SetRightStickDeadZone(int8 controller, float Xpos, float Xneg, float Ypos, float Yneg)
{
	m_Controllers[controller]->m_RightStickDeadzone = { static_cast<short>(Xpos * 32767.0f), static_cast<short>(Xneg * 32768.0f), static_cast<short>(Ypos * 32767.0f), static_cast<short>(Yneg * 32768.0f) };
}

std::vector<short> InputManager::GetLeftStickDeadZone(int8 controller)
{
	return m_Controllers[controller]->m_LeftStickDeadzone;
}

std::vector<short> InputManager::GetRightStickDeadZone(int8 controller)
{
	return m_Controllers[controller]->m_RightStickDeadzone;
}



bool InputManager::IsLeftStickInHorizontalDeadzone(int8 controller)
{
	short sX = m_Controllers[controller]->m_State.Gamepad.sThumbLX;

	if ((sX <= m_Controllers[controller]->m_LeftStickDeadzone[0]) and (sX >= m_Controllers[controller]->m_LeftStickDeadzone[1]))
	{
		return true;
	}

	return false;
}

bool InputManager::IsLeftStickInVerticalDeadzone(int8 controller)
{
	short sY = m_Controllers[controller]->m_State.Gamepad.sThumbLY;

	if ((sY <= m_Controllers[controller]->m_LeftStickDeadzone[2]) and (sY >= m_Controllers[controller]->m_LeftStickDeadzone[3]))
	{
		return true;
	}

	return false;
}

bool InputManager::IsRightStickInHorizontalDeadzone(int8 controller)
{
	short sX = m_Controllers[controller]->m_State.Gamepad.sThumbRX;

	if ((sX <= m_Controllers[controller]->m_RightStickDeadzone[0]) and (sX >= m_Controllers[controller]->m_RightStickDeadzone[1]))
	{
		return true;
	}

	return false;
}

bool InputManager::IsRightStickInVerticalDeadzone(int8 controller)
{
	short sY = m_Controllers[controller]->m_State.Gamepad.sThumbRY;

	if ((sY <= m_Controllers[controller]->m_RightStickDeadzone[2]) and (sY >= m_Controllers[controller]->m_RightStickDeadzone[3]))
	{
		return true;
	}

	return false;
}



float InputManager::GetLeftStickX(int8 controller)
{
	if (m_Controllers[controller]->m_State.Gamepad.sThumbLX < 0)
	{
		return static_cast<float>(m_Controllers[controller]->m_State.Gamepad.sThumbLX / 32768.0f);
	}

	return (static_cast<float>(m_Controllers[controller]->m_State.Gamepad.sThumbLX) / 32767.0f);
}

float InputManager::GetLeftStickY(int8 controller)
{
	if (m_Controllers[controller]->m_State.Gamepad.sThumbLY < 0)
	{
		return static_cast<float>(m_Controllers[controller]->m_State.Gamepad.sThumbLY / 32768.0f);
	}

	return (static_cast<float>(m_Controllers[controller]->m_State.Gamepad.sThumbLY) / 32767.0f);
}

float InputManager::GetRightStickX(int8 controller)
{
	if (m_Controllers[controller]->m_State.Gamepad.sThumbRX < 0)
	{
		return static_cast<float>(m_Controllers[controller]->m_State.Gamepad.sThumbRX / 32768.0f);
	}

	return (static_cast<float>(m_Controllers[controller]->m_State.Gamepad.sThumbRX) / 32767.0f);
}

float InputManager::GetRightStickY(int8 controller)
{
	if (m_Controllers[controller]->m_State.Gamepad.sThumbRY < 0)
	{
		return static_cast<float>(m_Controllers[controller]->m_State.Gamepad.sThumbRY / 32768.0f);
	}

	return (static_cast<float>(m_Controllers[controller]->m_State.Gamepad.sThumbRY) / 32767.0f);
}



Vector2f InputManager::LeftStickPressed(int8 controller)
{
	float sX = static_cast<float>(m_Controllers[controller]->m_State.Gamepad.sThumbLX);
	float sY = static_cast<float>(m_Controllers[controller]->m_State.Gamepad.sThumbLY);

	if (IsLeftStickInHorizontalDeadzone(controller) == true)
	{
		sX = 0;
	}

	if (IsLeftStickInVerticalDeadzone(controller) == true)
	{
		sY = 0;
	}

	if (sX < 0)
	{
		sX /= 32768.0f;
	}

	else
	{
		sX /= 32767.0f;
	}

	if (sY < 0)
	{
		sY /= 32768.0f;
	}

	else
	{
		sY /= 32767.0f;
	}

	return { sX, sY };
}

Vector2f InputManager::RightStickPressed(int8 controller)
{
	float sX = static_cast<float>(m_Controllers[controller]->m_State.Gamepad.sThumbRX);
	float sY = static_cast<float>(m_Controllers[controller]->m_State.Gamepad.sThumbRY);

	if (IsRightStickInHorizontalDeadzone(controller) == true)
	{
		sX = 0;
	}

	if (IsRightStickInVerticalDeadzone(controller) == true)
	{
		sY = 0;
	}

	if (sX < 0)
	{
		sX /= 32768.0f;
	}

	else
	{
		sX /= 32767.0f;
	}

	if (sY < 0)
	{
		sY /= 32768.0f;
	}

	else
	{
		sY /= 32767.0f;
	}

	return { sX, sY };
}

float InputManager::LeftTriggerPressed(int8 controller)
{
	BYTE Trigger = m_Controllers[controller]->m_State.Gamepad.bLeftTrigger;

	if (Trigger > m_Controllers[controller]->m_LeftTriggerDeadzone)
	{
		return Trigger / 255.0f;
	}

	return 0.0f;
}

float InputManager::RightTriggerPressed(int8 controller)
{
	

	BYTE Trigger = m_Controllers[controller]->m_State.Gamepad.bRightTrigger;

	if (Trigger > m_Controllers[controller]->m_RightTriggerDeadzone)
	{
		return Trigger / 255.0f;
	}

	return 0.0f;
}
