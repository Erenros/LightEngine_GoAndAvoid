#include "ScoreManager.h"

ScoreManager& ScoreManager::GetInstance()
{
    static ScoreManager instance;
    return instance;
}

void ScoreManager::Reset()
{
    m_score = 0;
    m_distanceAccumulator = 0.0f;
    m_distance = 0;
    m_hasLastPosition = false;
}

void ScoreManager::AddScore(int32 amount)
{
    m_score += amount;
}

void ScoreManager::UpdateDistanceFromPosition(Vector2f currentPosition)
{
    if (!m_hasLastPosition)
    {
        m_lastPosition = currentPosition;
        m_hasLastPosition = true;
        return;
    }

    float32 travelled = m_lastPosition.GetDistance(currentPosition);
    m_distanceAccumulator += travelled;

    int32 newDistance = static_cast<int32>(m_distanceAccumulator / 10.0f);
    int32 distanceDelta = newDistance - m_distance;

    if (distanceDelta > 0)
        AddScore(distanceDelta);

    m_distance = newDistance;

    m_lastPosition = currentPosition;
}

void ScoreManager::RegisterRunEnd()
{
    m_runHistory.push_back({ m_score, m_distance });

    if (m_score > m_bestScore)
        m_bestScore = m_score;

    if (m_distance > m_bestDistance)
        m_bestDistance = m_distance;
}

int32 ScoreManager::GetScore() const
{
    return m_score;
}

int32 ScoreManager::GetDistance() const
{
    return m_distance;
}

int32 ScoreManager::GetBestScore() const
{
    return m_bestScore;
}

int32 ScoreManager::GetBestDistance() const
{
    return m_bestDistance;
}

const std::vector<RunResult>& ScoreManager::GetRunHistory() const
{
    return m_runHistory;
}