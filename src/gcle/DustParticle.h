#pragma once
#include "Entity.h"
#include <string>

class DustParticle : public Entity
{
public:
    void Init(Vector2f position, Vector2f velocity, float32 lifetime, float32 startScale, Color color);

protected:
    void OnInitialize() override;
    void OnUpdate() override;

private:
    void Deactivate();

    Vector2f m_Velocity{ 0.0f, 0.0f };
    float32 m_Life = 0.0f;
    float32 m_MaxLife = 1.0f;
    float32 m_StartScale = 1.0f;
    Color m_BaseColor{ 250, 250, 245, 210 };

    std::string m_SceneTag;
    bool m_Active = false;
};