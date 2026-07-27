#include "Decor.h"

void Decor::OnInitialize()
{
    SetRigidBody(false);
    SetStatic(true);
}

void Decor::SetWater(bool isWater, byte alpha)
{
    m_IsWater = isWater;

    Color current = GetColor();
    current.a = isWater ? alpha : 255;
    SetColor(current);
}