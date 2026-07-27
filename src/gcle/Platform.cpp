#include "Platform.h"

void Platform::OnInitialize()
{
    SetTag(1);
    SetRigidBody(true);
    GetRigidBody()->SetGravity(false);
    SetStatic(false);
}

void Platform::SetType(PlatformType type)
{
    m_type = type;
}

void Platform::SetStatic(bool isStatic)
{
    Entity::SetStatic(isStatic);
}

void Platform::BuildColliders(const std::string& slopeType, float engineScale)
{
    float yOffset = 25.0f;

    if (slopeType == "SlopeRight")
    {
        CreateCollider(gcle::Shapes::Triangle, true, { 0.0f, 0.0f }, 0.0f, { -1.0f, 1.0f });
    }
    else if (slopeType == "SlopeLeft")
    {
        CreateCollider(gcle::Shapes::Triangle, true, { 0.0f, 0.0f }, 0.0f, { 1.0f, 1.0f });
    }
    else if (slopeType == "SlopeRight_Low")
    {
        CreateCollider(gcle::Shapes::Triangle, true, { 0.0f, yOffset }, 0.0f, { -1.0f, 0.5f });
    }
    else if (slopeType == "SlopeLeft_Low")
    {
        CreateCollider(gcle::Shapes::Triangle, true, { 0.0f, yOffset }, 0.0f, { 1.0f, 0.5f });
    }
    else if (slopeType == "SlopeRight_High")
    {
        CreateCollider(gcle::Shapes::Rectangle, true, { 0.0f, yOffset }, 0.0f, { 1.0f, 0.5f });
        CreateCollider(gcle::Shapes::Triangle, true, { 0.0f, -yOffset }, 0.0f, { -1.0f, 0.5f });
    }
    else if (slopeType == "SlopeLeft_High")
    {
        CreateCollider(gcle::Shapes::Rectangle, true, { 0.0f, yOffset }, 0.0f, { 1.0f, 0.5f });
        CreateCollider(gcle::Shapes::Triangle, true, { 0.0f, -yOffset }, 0.0f, { 1.0f, 0.5f });
    }
    else
    {
        CreateCollider(gcle::Shapes::Rectangle, true, { 0.0f, 0.0f }, 0.0f, { 1.0f, 1.0f });
    }
}