#include "Math.h"

float Math::RadToDeg(Radians rad)
{
	return rad * RAD_TO_DEG;
}

float Math::DegToRad(Degrees deg) {
	return deg * DEG_TO_RAD;
}

Vector2f Math::AngleToVec(Radians rad) {

	float x = std::cos(rad);
	float y = std::sin(rad);

	Vector2f vec({ x, y });

	return vec;
}

Math::Radians Math::VecToAngle(Vector2f vec) {

	Radians rad = std::atan2f(vec.y, vec.x);
	return rad;
}
