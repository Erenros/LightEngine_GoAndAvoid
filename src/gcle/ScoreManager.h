#pragma once
#include "include.h"
#include <vector>

struct RunResult
{
    int32 score = 0;
    int32 distance = 0;
};

class ScoreManager
{
public:
    static ScoreManager& GetInstance();

    void Reset();

    void AddScore(int32 amount);
    void UpdateDistanceFromPosition(Vector2f currentPosition);

    void RegisterRunEnd();

    int32 GetScore() const;
    int32 GetDistance() const;

    int32 GetBestScore() const;
    int32 GetBestDistance() const;

    const std::vector<RunResult>& GetRunHistory() const;

private:
    ScoreManager() = default;
    ~ScoreManager() = default;

    int32 m_score = 0;

    float32 m_distanceAccumulator = 0.0f;
    int32 m_distance = 0;

    bool m_hasLastPosition = false;
    Vector2f m_lastPosition = { 0.0f, 0.0f };

    int32 m_bestScore = 0;
    int32 m_bestDistance = 0;

    std::vector<RunResult> m_runHistory;
};