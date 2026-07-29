#pragma once
#include "include.h"

class FootstepDustEmitter
{
public:
    void Update(float32 dt, Vector2f feetPosition, bool canEmit, bool facingRight);

private:
    void Spawn(Vector2f feetPosition, bool facingRight);

    float32 m_Timer = 0.0f;
    float32 m_Interval = 0.09f;
    bool m_StepSideLeft = true;
};