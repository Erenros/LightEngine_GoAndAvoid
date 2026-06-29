#include "Timer.h"
#include <iostream>
#include <Windows.h>
#include <thread>
#include <chrono>

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

float64 Clock::GetTimeScale() const {
	return m_TimeScale;
}

void Clock::SetTimeScale(float64 scale) {
	m_TimeScale = scale;
}

std::chrono::steady_clock::time_point Clock::GetLastUpdateTime() const{
	return m_LastUpdateTime;
}

Seconds Clock::GetRawDT() const{
	auto now = clock::now();
	return std::chrono::duration<Seconds>(now - GetLastUpdateTime()).count();
}

void Clock::ClockSleep(Seconds time){
	Sleep(time * 1000);
}

void Clock::SmartSleep(Seconds time) {
	auto start = std::chrono::high_resolution_clock::now();
	auto target = start + std::chrono::duration<float64>(time);

	while (std::chrono::high_resolution_clock::now() < target) {
		if (std::chrono::high_resolution_clock::now() + std::chrono::milliseconds(1) < target)
			std::this_thread::yield();
	}
	while (std::chrono::high_resolution_clock::now() < target) {
#if defined(_MSC_VER)
		_mm_pause();
#endif
	}
}

void Clock::PreciseSleep(Seconds time) {
	auto start = std::chrono::high_resolution_clock::now();
	auto target = start + std::chrono::duration<float64>(time);

	while (std::chrono::high_resolution_clock::now() < target) {
		// Boucle active (consomme du CPU mais est ultra précise)
#if defined(_MSC_VER)
		_mm_pause(); // Optimise la consommation CPU dans la boucle
#endif
	}
}
