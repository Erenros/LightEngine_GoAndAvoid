#pragma once
#include "LevelEntry.h"
#include "Platform.h"
#include <string>

class PlatformEntry : public LevelEntry
{
public:
    PlatformEntry(
        Vector2f position,
        Vector2f scale,
        int16 srcX,
        int16 srcY,
        int16 gridSize,
        int16 textureWidth,
        int16 textureHeight,
        float32 engineScale,
        const std::string& slopeType);

    void Spawn(Scene* pScene) override;
    void Despawn() override;

private:
#ifdef _DEBUG
    void SpawnDebugVisuals(Scene* pScene, Platform* pPlatform);
#endif

    int16 m_SrcX;
    int16 m_SrcY;
    int16 m_GridSize;
    int16 m_TextureWidth;
    int16 m_TextureHeight;
    float32 m_EngineScale;
    std::string m_SlopeType;
    PlatformType m_Type;

#ifdef _DEBUG
    Entity* mp_DebugPrimary = nullptr;
    Entity* mp_DebugSecondary = nullptr;
#endif
};