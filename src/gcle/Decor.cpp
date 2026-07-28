#include "Decor.h"

namespace
{
    constexpr int32 DECOR_LAYER = 0;
}

void Decor::OnInitialize()
{
    SetRigidBody(false);
    SetStatic(true);
    SetLayer(DECOR_LAYER);
}

void Decor::SetWater(bool isWater, byte alpha)
{
    m_IsWater = isWater;

    Color current = GetColor();
    current.a = isWater ? alpha : 255;
    SetColor(current);
}