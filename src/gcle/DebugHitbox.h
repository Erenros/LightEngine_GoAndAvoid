#pragma once
#include "Hitbox.h"

#ifdef _DEBUG

class DebugHitbox : public Hitbox
{
public:
    void OnInitialize() override
    {
        Hitbox::OnInitialize();
        SetColor(Color{ 255, 0, 0, 120 });
    }

    void Activate(Vector2f offset, Vector2f size, int32 damage, float32 duration)
    {
        Hitbox::Activate(offset, size, damage, duration);
        SetRenderSize(0, { 100.0f * size.x, 100.0f * size.y });
        SetColor(Color{ 255, 0, 0, 120 });
    }

    void Deactivate()
    {
        Hitbox::Deactivate();
        SetRenderSize(0, { 0.0f, 0.0f });
    }
};

#endif