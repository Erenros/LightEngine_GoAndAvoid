#include "MathGC.h" 

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

std::ostream& operator<<(std::ostream& stream, const Matrix3x3& mat)
{
	stream << std::endl;
	for (uint32 j = 0; j < mat.HEIGHT; j++) {
		stream << " | ";
		for (uint32 i = 0; i < mat.WIDTH; i++) {
			stream << mat[i][j] << " | ";
		}
		stream << std::endl;
	}
	return stream;
}


Matrix::Matrix(int width, int height)
{
	InitMatrix(width, height);
}


void Matrix::InitSize()
{
	m_height = m_matrix.size();


	if (m_height == 0)
	{
		m_width = 0;
	}

	else
	{
		m_width = m_matrix[0].size();
	}


}

void Matrix::InitMatrix(int width, int height)
{
	for (int i = 0; i < height; i++)
	{
		std::vector<float32> vect;

		for (int j = 0; j < width; j++)
		{
			float32 number = 0;

			vect.push_back(number);
		}

		m_matrix.push_back(vect);
	}

	InitSize();
}

void Matrix::PrintMatrix()
{
	InitSize();

	if (m_matrix.size() == 0)
	{
		std::cout << "The matrix is empty !" << std::endl;
		return;
	}


	for (int i = 0; i < m_height; i++)
	{
		std::cout << "[";

		for (int j = 0; j < m_width - 1; j++)
		{
			std::cout << m_matrix[i][j] << ", ";
		}

		std::cout << m_matrix[i][m_width - 1] << "]" << std::endl;
	}
}


Matrix Matrix::operator*(Matrix& matrix2)
{
	InitSize();
	matrix2.InitSize();

	assert(m_width == matrix2.m_height);

	Matrix result;
	result.m_matrix.resize(m_height);

	for (auto& m : result.m_matrix)
		m.resize(matrix2.m_width);

	result.InitSize();

	for (int i = 0; i < m_height; i++)
	{
		for (int j = 0; j < matrix2.m_width; j++)
		{
			for (int k = 0; k < m_width; k++)
			{
				result.m_matrix[i][j] += (m_matrix[i][k] * matrix2.m_matrix[k][j]);
			}
		}
	}

	return result;
}
