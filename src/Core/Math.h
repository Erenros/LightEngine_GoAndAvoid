#pragma once

#include "include.h"


namespace Value
{
	constexpr float32 PI = 3.14159265358979323846;
	constexpr float RAD_TO_DEG = 180.0f / PI;
	constexpr float DEG_TO_RAD = PI / 180.0f;
}

class Math
{
	using Radians = float;
	using Degrees = float;

	float RadToDeg(Radians rad);
	float DegToRad(Degrees deg);
	Vector2f AngleToVec(Radians rad);
	Radians VecToAngle(Vector2f vec);

	template <typename T>
	T Lerp(T minValue, T maxValue, float t) {
		return (t - 1) * minValue + t * maxValue;
	};
};
