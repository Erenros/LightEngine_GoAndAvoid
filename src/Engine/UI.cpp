#include "UI.h"

void UI::Initialize(gcle::Shapes shape)
{
    GameObject::Initialize(shape);

    if (mp_RenderShape != nullptr)
    {
        Vector2f pos = m_Transform.GetPosition();

        mp_RenderShape->SetPosition(pos.x, pos.y);
        mp_RenderShape->SetScale(m_Transform.GetScale());
        mp_RenderShape->SetRotation(m_Transform.GetDegAngle());
    }

    OnInitialize();
}

void UI::Initialize()
{
    OnInitialize();
}

void UI::Update(float32 dt)
{
    GameObject::Update(dt);

    if (mp_RenderShape != nullptr)
    {
        Vector2f pos = m_Transform.GetPosition();

        // UI = position écran / render target, pas position monde
        mp_RenderShape->SetPosition(pos.x, pos.y);
        mp_RenderShape->SetScale(m_Transform.GetScale());
        mp_RenderShape->SetRotation(m_Transform.GetDegAngle());
    }

    OnUpdate();
}