#pragma once

#include "include.h"
#include <iostream>

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


class Matrix3x3
{
	float32 m[3][3];

	void CutMatrixRowLine(float32 output[2][2], uint32 column, uint32 row) {
		for (uint32 x = 0; x < 2; ++x) {
			for (uint32 y = 0; y < 2; ++y) {
				output[x][y] = m[x + (x >= column ? 1 : 0)][y + (y >= row ? 1 : 0)];
			}
		}
	}

public:
	const unsigned WIDTH = 3;
	const unsigned HEIGHT = 3;

	constexpr Matrix3x3(float32(*func)(uint32, uint32)) {
		for (uint32 x = 0; x < WIDTH; ++x) {
			for (uint32 y = 0; y < HEIGHT; ++y) {
				m[x][y] = func(x, y);
			}
		}
	}

	constexpr Matrix3x3() : Matrix3x3(+[](uint32 x, uint32 y) { if (x == y) { return 1.0f; } else { return 0.0f; }}) {}

	float32* operator[](uint32 x) { return m[x]; }
	const float32* operator[](uint32 x) const { return m[x]; }
	
	Matrix3x3 operator* (const Matrix3x3& other) const {
		Matrix3x3 r(+[](uint32, uint32) {return 0.0f; });

		for (uint32 i = 0; i < WIDTH; i++) {
			for (uint32 j = 0; j < HEIGHT; j++) {

				for (uint32 k = 0; k < WIDTH; k++) {
					r[i][j] += m[k][j] * other[i][k];
				}

			}
		}

		return r;
	}

	Matrix3x3 operator+ (const Matrix3x3& other) const {
		Matrix3x3 r(+[](uint32, uint32) {return 0.0f; });

		for (uint32 i = 0; i < WIDTH; i++) {
			for (uint32 j = 0; j < HEIGHT; j++) {
				r[i][j] = m[i][j] + other[i][j];
			}
		}

		return r;
	}

	Matrix3x3 operator* (float32 other) const {
		Matrix3x3 r(+[](uint32, uint32) {return 0.0f; });

		for (uint32 i = 0; i < WIDTH; i++) {
			for (uint32 j = 0; j < HEIGHT; j++) {
				r[i][j] = m[i][j] * other;
			}
		}

		return r;
	}

	float32 determinant() const {

		return m[0][0] * m[1][1] * m[2][2]
			+  m[0][1] * m[1][2] * m[2][0]
			+  m[0][2] * m[1][0] * m[2][1]
			-  m[0][2] * m[1][1] * m[2][0]
			-  m[0][0] * m[1][2] * m[2][1]
			-  m[0][1] * m[1][0] * m[2][2];
	}

	Matrix3x3 Inverse() const {
		float32 det = determinant();
		if (abs(det) < FLT_EPSILON) {
			return *this;
		}

		Matrix3x3 r(+[](uint32, uint32) {return 0.0f; });
		float32 inv_det = 1 / det;

		r[0][0] = (m[1][1] * m[2][2] - m[2][1] * m[1][2]) * inv_det;		r[1][0] = (m[2][0] * m[1][2] - m[1][0] * m[2][2]) * inv_det;	r[2][0] = (m[1][0] * m[2][1] - m[2][0] * m[1][1]) * inv_det;
		r[0][1] = (m[2][1] * m[0][2] - m[0][1] * m[2][2]) * inv_det;		r[1][1] = (m[0][0] * m[2][2] - m[2][0] * m[0][2]) * inv_det;	r[2][1] = (m[2][0] * m[0][1] - m[0][0] * m[2][1]) * inv_det;
		r[0][2] = (m[0][1] * m[1][2] - m[1][1] * m[0][2]) * inv_det;		r[1][2] = (m[1][0] * m[0][2] - m[0][0] * m[1][2]) * inv_det;	r[2][2] = (m[0][0] * m[1][1] - m[1][0] * m[0][1]) * inv_det;

		return r;
	}

	static Matrix3x3 Scale(Vector2f scale) {
		Matrix3x3 m;
		m[0][0] = scale.x;
		m[1][1] = scale.y;
		return m;
	}

	static Matrix3x3 Rotation(Radians angle) {
		float c = cos(angle);
		float s = sin(angle);
		Matrix3x3 m;
		m[0][0] = c; m[1][0] = -s;
		m[0][1] = s; m[1][1] = c;
		return m;
	}

	static Matrix3x3 Translation(Vector2f position) {
		Matrix3x3 m;
		m[2][0] = position.x;
		m[2][1] = position.y; 
		return m;
	}

};

std::ostream& operator<< (std::ostream& stream, const Matrix3x3& mat);
