#pragma once
#include "PrimitiveTypes.h"
#include "Vector2.hpp"
#include "Profiler.h"
#include "Utils.h"
#include "Timer.h"

using Vector2u = Vector2<int32>;
using Vector2f = Vector2<float32>;

constexpr float64 PI = 3.14159265358979323846;
constexpr float32 RAD_TO_DEG = float32(180.0f / PI);
constexpr float32 DEG_TO_RAD = float32(PI / 180.0f);