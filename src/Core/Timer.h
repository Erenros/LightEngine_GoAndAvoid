#pragma once
#include <chrono>

using namespace std::chrono;
using second = double;


class Timer
{
private:
	system_clock::time_point m_beginning_time = system_clock::now();
	system_clock::time_point m_ending_time;

	float scale = 1;

public:
	void ResetChrono();
	second GetChronoTime();
	second GetChronoTimeScale();

	void SetScale(float f);
	float GetScale();
};

