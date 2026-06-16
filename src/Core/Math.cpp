#include "Math.h" 

float32 MathGC::RadToDeg(Radians rad)
{
    return rad * RAD_TO_DEG;
}

float32 MathGC::DegToRad(Degrees deg)
{
    return deg * DEG_TO_RAD;
}

Vector2f MathGC::AngleToVec(Radians rad)
{
    float x = std::cos(rad);
    float y = std::sin(rad);

    Vector2f vec({ x, y });

    return vec;
}

Radians MathGC::VecToAngle(Vector2f& vec)
{
    Radians rad = std::atan2(vec.y, vec.x);
    return rad;
}
