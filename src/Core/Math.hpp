#pragma once

namespace Math 
{
	template <typename T>
	T Lerp(T minValue, T maxValue, float t ) {
		return (t-1) * minValue + t * maxValue
	}


}
