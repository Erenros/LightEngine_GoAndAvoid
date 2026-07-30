//#pragma once
//#include "LevelEntry.h"
//#include "Decor.h"
//
//class DecorEntry : public LevelEntry
//{
//public:
//    DecorEntry(
//        Vector2f position,
//        Vector2f scale,
//        int16 srcX,
//        int16 srcY,
//        int16 gridSize,
//        int16 textureWidth,
//        int16 textureHeight,
//        bool isWater);
//
//    void Spawn(Scene* pScene) override;
//    void Despawn() override;
//
//private:
//    int16 m_SrcX;
//    int16 m_SrcY;
//    int16 m_GridSize;
//    int16 m_TextureWidth;
//    int16 m_TextureHeight;
//    bool m_IsWater;
//
//#ifdef _DEBUG
//    Entity* mp_DebugEntity = nullptr;
//#endif
//};