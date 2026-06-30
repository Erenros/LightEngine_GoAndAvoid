#include "Timer.h"

void Clock::Restart(float64 t) {
    m_DeltaTime = 0.0;
    m_UnscaleDeltaTime = 0.0;
    m_Time = 0.0;
    m_UnscaleTime = 0.0;
    m_LastUpdateTime = clock::now();
}

void Clock::Update() {
    auto t = clock::now();
    auto time = t - m_LastUpdateTime;
    uint64 time_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(time).count();
    constexpr float64 NANOSECONDS_TO_SECOND = 0.000000001;
    Seconds time_s = static_cast<float64>(time_ns) * NANOSECONDS_TO_SECOND;

    m_UnscaleTime += time_s;
    m_Time += time_s * m_TimeScale;

    m_UnscaleDeltaTime = time_s;
    m_DeltaTime = time_s * m_TimeScale;

    m_LastUpdateTime = t;

    // FPS Count
    m_fpsTimer += GetDeltaTime();
    if (m_fpsTimer >= 1.f) {
        m_fpsTimer -= 1.f;
        m_fpsCount = static_cast<int16>(1.f / GetDeltaTime());
    }

}

float64 Clock::GetDeltaTime() const {
    return m_DeltaTime;
}

float64 Clock::GetDeltaTimeUnscaled() const {
    return m_UnscaleDeltaTime;
}

float64 Clock::GetTime() const {
    return m_Time;
}

float64 Clock::GetTimeUnscaled() const {
    return m_UnscaleTime;
}

int16 Clock::GetFramePerSecond() const
{
    return m_fpsCount;
}

float64 Clock::GetTimeScale() const {
    return m_TimeScale;
}

void Clock::SetTimeScale(float64 scale) {
    m_TimeScale = scale;
}