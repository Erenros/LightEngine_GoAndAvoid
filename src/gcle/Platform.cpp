#include "Platform.h"

void Platform::OnInitialize()
{
    SetTag(1);
    SetRigidBody(true);
    GetRigidBody()->SetGravity(false);
    SetStatic(true);
}

void Platform::SetType(PlatformType type)
{
    m_type = type;
}

void Platform::SetStatic(bool isStatic)
{
    Entity::SetStatic(isStatic);
}