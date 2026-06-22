#pragma once
#include <chrono> 

#include "include.h"

using Seconds = float64;

class Clock
{
    using clock = std::chrono::steady_clock;

    clock::time_point m_LastUpdateTime;
    double m_TimeScale;

    Seconds m_UnscaleTime;
    Seconds m_Time;

    Seconds m_DeltaTime;
    Seconds m_UnscaleDeltaTime;
public:
    Clock() :
        m_TimeScale(1.0),
        m_UnscaleTime(0.0),
        m_Time(0.0),
        m_DeltaTime(0.0),
        m_UnscaleDeltaTime(0.0)
    {
        m_LastUpdateTime = clock::now();
    }

    ~Clock() = default;

    void Restart(float64 t = 0.0);
    void Update();

    [[nodiscard]] Seconds GetDeltaTime() const;
    [[nodiscard]] Seconds GetDeltaTimeUnscaled() const;

    [[nodiscard]] Seconds GetTime() const;
    [[nodiscard]] Seconds GetTimeUnscaled() const;

    [[nodiscard]] float64 GetTimeScale() const;
    void SetTimeScale(float64 scale);
};