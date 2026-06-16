#pragma once

#include "include.h"

namespace Math
{
	using Radians = float;
	using Degrees = float;


	constexpr float32 PI = 3.14159265358979323846;
	constexpr float RAD_TO_DEG = 180.0f / PI;
	constexpr float DEG_TO_RAD = PI / 180.0f;

	template <typename T>
	T Lerp(T minValue, T maxValue, float t) {
		return (t - 1) * minValue + t * maxValue;
	}

	float RadToDeg(Radians rad) {
		return rad* RAD_TO_DEG;
	}

	float DegToRad(Degrees deg) {
		return deg* DEG_TO_RAD;
	}

	Vector2f AngleToVec(Radians rad) {

		float x = std::cos(rad);
		float y = std::sin(rad);

		Vector2f vec({ x, y });

		return vec;
	}

	Radians VecToAngle(Vector2f vec) {

		Radians rad = std::atan2f(vec.y, vec.x);
		return rad;
	}
}
