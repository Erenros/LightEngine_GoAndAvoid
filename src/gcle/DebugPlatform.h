#pragma once
#include "Engine/Entity.h"

#ifdef _DEBUG

class DebugPlatform : public Entity
{
private:
    Entity* m_target = nullptr;
    Vector2f m_offset = { 0.0f, 0.0f };
    Vector2f m_scaleMod = { 1.0f, 1.0f };

public:
    void SetTarget(Entity* target, Vector2f offset = { 0.0f, 0.0f }, Vector2f scaleMod = { 1.0f, 1.0f })
    {
        m_target = target;
        m_offset = offset;
        m_scaleMod = scaleMod;
    }

    void OnInitialize() override
    {
        SetRigidBody(false);
        SetColor(Color{ 0, 255, 0, 120 });
    }

    void OnUpdate() override
    {
        if (m_target != nullptr)
        {
            Vector2f tScale = m_target->GetScale();
            SetScale({ tScale.x * m_scaleMod.x, tScale.y * m_scaleMod.y });

            Vector2f tPos = m_target->GetPosition();
            SetPosition(tPos.x + m_offset.x * tScale.x, tPos.y + m_offset.y * tScale.y);
        }
    }
};

#endif