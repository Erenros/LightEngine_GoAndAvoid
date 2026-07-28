#include "LevelStreamer.h"
#include <algorithm>

void LevelStreamer::Initialize(Scene* pScene, float32 loadDistance, float32 unloadDistance)
{
    mp_Scene = pScene;
    m_LoadDistance = loadDistance;
    m_UnloadDistance = unloadDistance;
}

void LevelStreamer::AddEntry(std::unique_ptr<LevelEntry> entry)
{
    m_Entries.push_back(std::move(entry));
}

void LevelStreamer::FinalizeLoading()
{
    std::stable_sort(m_Entries.begin(), m_Entries.end(), [](const std::unique_ptr<LevelEntry>& a, const std::unique_ptr<LevelEntry>& b)
        {
            return a->GetX() < b->GetX();
        });

    m_LowIndex = 0;
    m_HighIndex = 0;
}

void LevelStreamer::Update(float32 centerX)
{
    if (mp_Scene == nullptr)
    {
        return;
    }

    const float32 spawnLow = centerX - m_LoadDistance;
    const float32 spawnHigh = centerX + m_LoadDistance;
    const float32 despawnLow = centerX - m_UnloadDistance;
    const float32 despawnHigh = centerX + m_UnloadDistance;
    const size_t entryCount = m_Entries.size();

    while (m_HighIndex < entryCount && m_Entries[m_HighIndex]->GetX() <= spawnHigh)
    {
        m_Entries[m_HighIndex]->Spawn(mp_Scene);
        ++m_HighIndex;
    }

    size_t newLowIndex = m_LowIndex;
    while (newLowIndex > 0 && m_Entries[newLowIndex - 1]->GetX() >= spawnLow)
    {
        --newLowIndex;
    }

    for (size_t i = newLowIndex; i < m_LowIndex; ++i)
    {
        m_Entries[i]->Spawn(mp_Scene);
    }

    m_LowIndex = newLowIndex;

    while (m_LowIndex < m_HighIndex && m_Entries[m_LowIndex]->GetX() < despawnLow)
    {
        m_Entries[m_LowIndex]->Despawn();
        ++m_LowIndex;
    }

    while (m_HighIndex > m_LowIndex && m_Entries[m_HighIndex - 1]->GetX() > despawnHigh)
    {
        --m_HighIndex;
        m_Entries[m_HighIndex]->Despawn();
    }
}

void LevelStreamer::Shutdown()
{
    for (size_t i = m_LowIndex; i < m_HighIndex; ++i)
    {
        m_Entries[i]->Despawn();
    }

    m_LowIndex = 0;
    m_HighIndex = 0;
}