#include "Timer.h"

void Timer::ResetChrono()
{
	m_beginning_time = system_clock::now();
}

second Timer::GetChronoTime()
{
	m_ending_time = system_clock::now();

	float f = (m_ending_time - m_beginning_time).count() / 10000000.f;

	return f;
}

second Timer::GetChronoTimeScale()
{
	m_ending_time = system_clock::now();

	float f = ((m_ending_time - m_beginning_time).count() / 10000000.f) * scale;

	return f;
}

void Timer::SetScale(float f)
{
	scale = f;
}

float Timer::GetScale()
{
	return scale;
}
