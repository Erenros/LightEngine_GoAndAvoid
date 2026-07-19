#include "CharacterInput.h"

void CharacterInput::SetMapping(InputMapping mapping, int8 padIndex)
{
    m_mapping = mapping;
    m_padIndex = padIndex;
    m_padPrev = 0;

    if (mapping == InputMapping::Controller || mapping == InputMapping::Joystick)
    {
        InputManager::GetInstance().AddController(new Controller());
    }
}

FrameInputs CharacterInput::Read()
{
    switch (m_mapping)
    {
    case InputMapping::Controller: return ReadController();
    case InputMapping::Joystick:   return ReadJoystick();
    default:                       return ReadKeyboard();
    }
}

int16 CharacterInput::RisingEdges(int16 now)
{
    int16 pressed = now & ~m_padPrev;
    m_padPrev = now;
    return pressed;
}

FrameInputs CharacterInput::ReadKeyboard()
{
    InputManager& im = InputManager::GetInstance();
    FrameInputs in;

    in.left = im.IsHeld(static_cast<char>(m_keyLeft));
    in.right = im.IsHeld(static_cast<char>(m_keyRight));
    in.crouch = im.IsHeld(static_cast<char>(m_keyCrouch));
    in.jump = im.IsDown(m_keyJump);
    in.skill0 = im.IsDown(m_keySkill0);
    in.skill1 = im.IsDown(m_keySkill1);
    return in;
}

FrameInputs CharacterInput::ReadController()
{
    InputManager& im = InputManager::GetInstance();
    FrameInputs in;

    const bool stickH = !im.IsLeftStickInHorizontalDeadzone(m_padIndex);
    const bool stickV = !im.IsLeftStickInVerticalDeadzone(m_padIndex);
    const float32 stickX = im.GetLeftStickX(m_padIndex);
    const float32 stickY = im.GetLeftStickY(m_padIndex);

    in.left = im.IsControllerDown(m_padIndex, XBOX_PAD_LEFT) || (stickH && stickX < 0.0f);
    in.right = im.IsControllerDown(m_padIndex, XBOX_PAD_RIGHT) || (stickH && stickX > 0.0f);
    in.crouch = im.IsControllerDown(m_padIndex, XBOX_PAD_DOWN) || (stickV && stickY < 0.0f);

    int16 now = 0;
    if (im.IsControllerDown(m_padIndex, XBOX_A)) now |= 1;
    if (im.IsControllerDown(m_padIndex, XBOX_X)) now |= 2;
    if (im.IsControllerDown(m_padIndex, XBOX_B) || im.IsControllerDown(m_padIndex, XBOX_RB)) now |= 4;

    int16 pressed = RisingEdges(now);
    in.jump = (pressed & 1) != 0;
    in.skill0 = (pressed & 2) != 0;
    in.skill1 = (pressed & 4) != 0;
    return in;
}

FrameInputs CharacterInput::ReadJoystick()
{
    InputManager& im = InputManager::GetInstance();
    FrameInputs in;

    bool stickH = !im.IsLeftStickInHorizontalDeadzone(m_padIndex);
    bool stickV = !im.IsLeftStickInVerticalDeadzone(m_padIndex);
    const float32 stickX = im.GetLeftStickX(m_padIndex);
    const float32 stickY = im.GetLeftStickY(m_padIndex);

    in.left = (stickH && stickX < 0.0f) || im.IsControllerDown(m_padIndex, XBOX_PAD_LEFT);
    in.right = (stickH && stickX > 0.0f) || im.IsControllerDown(m_padIndex, XBOX_PAD_RIGHT);
    in.crouch = (stickV && stickY < 0.0f) || im.IsControllerDown(m_padIndex, XBOX_PAD_DOWN);

    int16 now = 0;
    if (im.IsControllerDown(m_padIndex, XBOX_A)) now |= 1;
    if (im.IsControllerDown(m_padIndex, XBOX_X)) now |= 2;
    if (im.IsControllerDown(m_padIndex, XBOX_Y)) now |= 4;

    int16 pressed = RisingEdges(now);
    in.jump = (pressed & 1) != 0;
    in.skill0 = (pressed & 2) != 0;
    in.skill1 = (pressed & 4) != 0;
    return in;
}