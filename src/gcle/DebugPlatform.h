#pragma once
#include "Engine/Entity.h"

#ifdef _DEBUG

class DebugPlatform : public Entity
{
private:
    Entity* m_target = nullptr;

public:
    void SetTarget(Entity* target)
    {
        m_target = target;
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
            SetPosition(m_target->GetPosition().x, m_target->GetPosition().y);
            SetScale(m_target->GetScale());
        }
    }
};

#endif