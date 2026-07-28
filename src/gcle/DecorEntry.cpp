#include "DecorEntry.h"
#include "Scene.h"

DecorEntry::DecorEntry(
    Vector2f position,
    Vector2f scale,
    int16 srcX,
    int16 srcY,
    int16 gridSize,
    int16 textureWidth,
    int16 textureHeight,
    bool isWater) :
    LevelEntry(position, scale),
    m_SrcX(srcX),
    m_SrcY(srcY),
    m_GridSize(gridSize),
    m_TextureWidth(textureWidth),
    m_TextureHeight(textureHeight),
    m_IsWater(isWater)
{
}

void DecorEntry::Spawn(Scene* pScene)
{
    if (pScene == nullptr || IsSpawned())
    {
        return;
    }

    Decor* pDecor = pScene->CreateEntity<Decor>(gcle::Shapes::Rectangle);
    pDecor->SetPosition(m_Position.x, m_Position.y);
    pDecor->SetScale(m_Scale);
    pDecor->SetTexture("Assets");

    if (pDecor->GetRenderShape() != nullptr)
    {
        pDecor->GetRenderShape()->SetTextureRect(m_SrcX, m_SrcY, m_GridSize, m_GridSize, m_TextureWidth, m_TextureHeight);
    }

    pDecor->SetWater(m_IsWater);

    mp_Entity = pDecor;
}

void DecorEntry::Despawn()
{
    if (!IsSpawned())
    {
        return;
    }

    mp_Entity->Destroy();
    mp_Entity = nullptr;
}