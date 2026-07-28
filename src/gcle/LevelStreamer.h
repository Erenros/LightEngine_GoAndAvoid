#pragma once
#include "LevelEntry.h"
#include <vector>
#include <memory>

class Scene;

class LevelStreamer
{
public:
    void Initialize(Scene* pScene, float32 loadDistance, float32 unloadDistance);
    void AddEntry(std::unique_ptr<LevelEntry> entry);
    void FinalizeLoading();
    void Update(float32 centerX);
    void Shutdown();

private:
    Scene* mp_Scene = nullptr;
    float32 m_LoadDistance = 2200.0f;
    float32 m_UnloadDistance = 2600.0f;

    std::vector<std::unique_ptr<LevelEntry>> m_Entries;

    size_t m_LowIndex = 0;
    size_t m_HighIndex = 0;
};