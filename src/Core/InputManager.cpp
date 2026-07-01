#include "InputManager.h"



void InputManager::Update() {
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

	m_State = GetGamepadState();
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



bool InputManager::IsControllerDown(int16 key)
{ 
	XINPUT_STATE state;
	if (!XInputGetState(0, &state) == ERROR_SUCCESS)
		return false;
	 
	if (state.Gamepad.wButtons & key)
	{
		return true;
	}

	return false;
}

void InputManager::SetVibration(float32 powerLeft, float32 powerRight)
{
	XINPUT_VIBRATION VibrationState;

	ZeroMemory(&VibrationState, sizeof(XINPUT_VIBRATION));

	int iLeftMotor = int(powerLeft * 65535.0f);
	int iRightMotor = int(powerRight * 65535.0f);

	VibrationState.wLeftMotorSpeed = iLeftMotor;
	VibrationState.wRightMotorSpeed = iRightMotor;

	XInputSetState(m_GamepadIndex, &VibrationState);
}





XINPUT_STATE InputManager::GetGamepadState()
{
	XINPUT_STATE GamepadState;

	ZeroMemory(&GamepadState, sizeof(XINPUT_STATE));

	XInputGetState(m_GamepadIndex, &GamepadState);

	return GamepadState;
}



void InputManager::SetLeftStickDeadZone(float Xpos, float Xneg, float Ypos, float Yneg)
{
	m_LeftStickDeadzone = { static_cast<short>(Xpos * 32767.0f), static_cast<short>(Xneg * 32768.0f), static_cast<short>(Ypos * 32767.0f), static_cast<short>(Yneg * 32768.0f) };
}

void InputManager::SetRightStickDeadZone(float Xpos, float Xneg, float Ypos, float Yneg)
{
	m_RightStickDeadzone = { static_cast<short>(Xpos * 32767.0f), static_cast<short>(Xneg * 32768.0f), static_cast<short>(Ypos * 32767.0f), static_cast<short>(Yneg * 32768.0f) };
}

std::vector<short> InputManager::GetLeftStickDeadZone()
{
	return m_LeftStickDeadzone;
}

std::vector<short> InputManager::GetRightStickDeadZone()
{
	return m_RightStickDeadzone;
}



bool InputManager::IsLeftStickInHorizontalDeadzone()
{
	short sX = m_State.Gamepad.sThumbLX;

	if ((sX <= m_LeftStickDeadzone[0]) and (sX >= m_LeftStickDeadzone[1]))
	{
		return true;
	}

	return false;
}

bool InputManager::IsLeftStickInVerticalDeadzone()
{
	short sY = m_State.Gamepad.sThumbLY;

	if ((sY <= m_LeftStickDeadzone[2]) and (sY >= m_LeftStickDeadzone[3]))
	{
		return true;
	}

	return false;
}

bool InputManager::IsRightStickInHorizontalDeadzone()
{
	short sX = m_State.Gamepad.sThumbRX;

	if ((sX <= m_RightStickDeadzone[0]) and (sX >= m_RightStickDeadzone[1]))
	{
		return true;
	}

	return false;
}

bool InputManager::IsRightStickInVerticalDeadzone()
{
	short sY = m_State.Gamepad.sThumbRY;

	if ((sY <= m_RightStickDeadzone[2]) and (sY >= m_RightStickDeadzone[3]))
	{
		return true;
	}

	return false;
}



float InputManager::GetLeftStickX()
{
	if (m_State.Gamepad.sThumbLX < 0)
	{
		return static_cast<float>(m_State.Gamepad.sThumbLX / 32768.0f);
	}

	return (static_cast<float>(m_State.Gamepad.sThumbLX) / 32767.0f);
}

float InputManager::GetLeftStickY()
{
	if (m_State.Gamepad.sThumbLY < 0)
	{
		return static_cast<float>(m_State.Gamepad.sThumbLY / 32768.0f);
	}

	return (static_cast<float>(m_State.Gamepad.sThumbLY) / 32767.0f);
}

float InputManager::GetRightStickX()
{
	if (m_State.Gamepad.sThumbRX < 0)
	{
		return static_cast<float>(m_State.Gamepad.sThumbRX / 32768.0f);
	}

	return (static_cast<float>(m_State.Gamepad.sThumbRX) / 32767.0f);
}

float InputManager::GetRightStickY()
{
	if (m_State.Gamepad.sThumbRY < 0)
	{
		return static_cast<float>(m_State.Gamepad.sThumbRY / 32768.0f);
	}

	return (static_cast<float>(m_State.Gamepad.sThumbRY) / 32767.0f);
}



Vector2f InputManager::LeftStickPressed()
{
	float sX = static_cast<float>(m_State.Gamepad.sThumbLX);
	float sY = static_cast<float>(m_State.Gamepad.sThumbLY);

	if (IsLeftStickInHorizontalDeadzone() == true)
	{
		sX = 0;
	}

	if (IsLeftStickInVerticalDeadzone() == true)
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

Vector2f InputManager::RightStickPressed()
{
	float sX = static_cast<float>(m_State.Gamepad.sThumbRX);
	float sY = static_cast<float>(m_State.Gamepad.sThumbRY);

	if (IsRightStickInHorizontalDeadzone() == true)
	{
		sX = 0;
	}

	if (IsRightStickInVerticalDeadzone() == true)
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

float InputManager::LeftTriggerPressed()
{
	BYTE Trigger = m_State.Gamepad.bLeftTrigger;

	if (Trigger > m_LeftTriggerDeadzone)
	{
		return Trigger / 255.0f;
	}

	return 0.0f;
}

float InputManager::RightTriggerPressed()
{
	BYTE Trigger = m_State.Gamepad.bRightTrigger;

	if (Trigger > m_RightTriggerDeadzone)
	{
		return Trigger / 255.0f;
	}

	return 0.0f;
}
