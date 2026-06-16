#pragma once

#include "include.h"

using Radians = float32;
using Degrees = float32;

class MathGC
{
public:
	static float32 RadToDeg(Radians rad);
	static float32 DegToRad(Degrees deg);
	static Vector2f AngleToVec(Radians rad);
	static Radians VecToAngle(Vector2f& vec);

	template<typename T>
	static T Lerp(T minValue, T maxValue, float t);
};

template<typename T>
T MathGC::Lerp(T minValue, T maxValue, float t)
{
	return (t - 1) * minValue + t * maxValue;
}



