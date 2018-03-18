#pragma once
#include <initializer_list>

namespace Maths
{
	class Mat3
	{
	public:
		float m_matrix[3][3];

	public:
		Mat3();
		Mat3(std::initializer_list<float> p_initList);
		Mat3(const Mat3& p_other);
		~Mat3();

		float Determinant();
	};
}
