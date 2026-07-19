#include "Platform.h"

void Platform::OnInitialize()
{
    SetRigidBody(true);
    SetStatic(true);
    CreateCollider(gcle::Shapes::Rectangle, true, { 0.0f, 0.0f }, 0.0f, { 1.0f, 1.0f });
}

void Platform::SetType(PlatformType type)
{
    m_type = type;
}