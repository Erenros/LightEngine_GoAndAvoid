#pragma once
#include "Engine/Entity.h"

#ifdef _DEBUG

class DebugKillableEntity : public Entity
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
        SetColor(Color{ 255, 0, 0, 120 });
        SetLayer(3);
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