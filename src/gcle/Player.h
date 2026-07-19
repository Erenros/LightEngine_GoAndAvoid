#pragma once
#include "KillableEntity.h"
#include "CharacterInput.h"

class Player : public KillableEntity
{
public:
    void SetupPlayer(InputMapping mapping, int8 padIndex = 0);
    void SetCustomKeys(int16 left, int16 right, int16 crouch, int16 jump, int16 skill0, int16 skill1);

protected:
    void OnUpdate() override;
    void OnInitialize() override;
    void OnDeath() override;

private:
    CharacterInput m_input;
    FrameInputs m_currentInputs;
};