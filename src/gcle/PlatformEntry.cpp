#include "PlatformEntry.h"
#include "Scene.h"

#ifdef _DEBUG
#include "DebugPlatform.h"
#endif

PlatformEntry::PlatformEntry(
    Vector2f position,
    Vector2f scale,
    int16 srcX,
    int16 srcY,
    int16 gridSize,
    int16 textureWidth,
    int16 textureHeight,
    float32 engineScale,
    const std::string& slopeType) :
    LevelEntry(position, scale),
    m_SrcX(srcX),
    m_SrcY(srcY),
    m_GridSize(gridSize),
    m_TextureWidth(textureWidth),
    m_TextureHeight(textureHeight),
    m_EngineScale(engineScale),
    m_SlopeType(slopeType),
    m_Type(slopeType == "Spikes" ? PlatformType::Spikes : PlatformType::Grass)
{
}

void PlatformEntry::Spawn(Scene* pScene)
{
    if (pScene == nullptr || IsSpawned())
    {
        return;
    }

    Platform* pPlatform = pScene->CreateEntity<Platform>(gcle::Shapes::Rectangle);
    pPlatform->SetPosition(m_Position.x, m_Position.y);
    pPlatform->SetScale(m_Scale);
    pPlatform->SetTexture("Assets");
    pPlatform->SetTag(1);
    pPlatform->SetRigidBody(true);
    pPlatform->GetRigidBody()->SetGravity(false);
    pPlatform->SetStatic(false);
    pPlatform->SetType(m_Type);
    pPlatform->BuildColliders(m_SlopeType, m_EngineScale);

    if (pPlatform->GetRenderShape() != nullptr)
    {
        pPlatform->GetRenderShape()->SetTextureRect(m_SrcX, m_SrcY, m_GridSize, m_GridSize, m_TextureWidth, m_TextureHeight);
    }

    mp_Entity = pPlatform;

#ifdef _DEBUG
    SpawnDebugVisuals(pScene, pPlatform);
#endif
}

void PlatformEntry::Despawn()
{
    if (!IsSpawned())
    {
        return;
    }

    mp_Entity->Destroy();
    mp_Entity = nullptr;

#ifdef _DEBUG
    if (mp_DebugPrimary != nullptr)
    {
        mp_DebugPrimary->Destroy();
        mp_DebugPrimary = nullptr;
    }
    if (mp_DebugSecondary != nullptr)
    {
        mp_DebugSecondary->Destroy();
        mp_DebugSecondary = nullptr;
    }
#endif
}

#ifdef _DEBUG
void PlatformEntry::SpawnDebugVisuals(Scene* pScene, Platform* pPlatform)
{
    constexpr float32 DEBUG_Y_OFFSET = 25.0f;

    if (m_SlopeType == "SlopeRight")
    {
        DebugPlatform* dp = pScene->CreateEntity<DebugPlatform>(gcle::Shapes::Triangle);
        dp->SetTarget(pPlatform, { 0.0f, 0.0f }, { -1.0f, 1.0f });
        mp_DebugPrimary = dp;
    }
    else if (m_SlopeType == "SlopeLeft")
    {
        DebugPlatform* dp = pScene->CreateEntity<DebugPlatform>(gcle::Shapes::Triangle);
        dp->SetTarget(pPlatform, { 0.0f, 0.0f }, { 1.0f, 1.0f });
        mp_DebugPrimary = dp;
    }
    else if (m_SlopeType == "SlopeRight_Low")
    {
        DebugPlatform* dp = pScene->CreateEntity<DebugPlatform>(gcle::Shapes::Triangle);
        dp->SetTarget(pPlatform, { 0.0f, DEBUG_Y_OFFSET }, { -1.0f, 0.5f });
        mp_DebugPrimary = dp;
    }
    else if (m_SlopeType == "SlopeLeft_Low")
    {
        DebugPlatform* dp = pScene->CreateEntity<DebugPlatform>(gcle::Shapes::Triangle);
        dp->SetTarget(pPlatform, { 0.0f, DEBUG_Y_OFFSET }, { 1.0f, 0.5f });
        mp_DebugPrimary = dp;
    }
    else if (m_SlopeType == "SlopeRight_High")
    {
        DebugPlatform* dp1 = pScene->CreateEntity<DebugPlatform>(gcle::Shapes::Rectangle);
        dp1->SetTarget(pPlatform, { 0.0f, DEBUG_Y_OFFSET }, { 1.0f, 0.5f });
        mp_DebugPrimary = dp1;

        DebugPlatform* dp2 = pScene->CreateEntity<DebugPlatform>(gcle::Shapes::Triangle);
        dp2->SetTarget(pPlatform, { 0.0f, -DEBUG_Y_OFFSET }, { -1.0f, 0.5f });
        mp_DebugSecondary = dp2;
    }
    else if (m_SlopeType == "SlopeLeft_High")
    {
        DebugPlatform* dp1 = pScene->CreateEntity<DebugPlatform>(gcle::Shapes::Rectangle);
        dp1->SetTarget(pPlatform, { 0.0f, DEBUG_Y_OFFSET }, { 1.0f, 0.5f });
        mp_DebugPrimary = dp1;

        DebugPlatform* dp2 = pScene->CreateEntity<DebugPlatform>(gcle::Shapes::Triangle);
        dp2->SetTarget(pPlatform, { 0.0f, -DEBUG_Y_OFFSET }, { 1.0f, 0.5f });
        mp_DebugSecondary = dp2;
    }
    else
    {
        DebugPlatform* dp = pScene->CreateEntity<DebugPlatform>(gcle::Shapes::Rectangle);
        dp->SetTarget(pPlatform, { 0.0f, 0.0f }, { 1.0f, 1.0f });
        mp_DebugPrimary = dp;
    }
}
#endif