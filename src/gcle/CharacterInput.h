#pragma once
#include "Core/InputManager.h"

enum class InputMapping
{
    Keyboard,
    Controller,
    Joystick
};

struct FrameInputs
{
    bool left = false;
    bool right = false;
    bool crouch = false;
    bool jump = false;
    bool skill0 = false;
    bool skill1 = false;
};

class CharacterInput
{
public:
    void SetMapping(InputMapping mapping, int8 padIndex = 0);
    InputMapping GetMapping() const { return m_mapping; }

    FrameInputs Read();

    int16 m_keyLeft = 'Q';
    int16 m_keyRight = 'D';
    int16 m_keyCrouch = 'S';
    int16 m_keyJump = VK_SPACE;
    int16 m_keySkill0 = VK_NUMPAD0;
    int16 m_keySkill1 = VK_NUMPAD1;

private:
    FrameInputs ReadKeyboard();
    FrameInputs ReadController();
    FrameInputs ReadJoystick();

    int16 RisingEdges(int16 now);

    InputMapping m_mapping = InputMapping::Keyboard;
    int8 m_padIndex = 0;
    int16 m_padPrev = 0;
};