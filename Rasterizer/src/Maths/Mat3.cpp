#include <Maths/Mat3.h>
#include <iostream>

using namespace Maths;

Mat3::Mat3()
{
	for (int i = 0; i < 3; ++i)
		for (int j = 0; j < 3; ++j)
			this->m_matrix[i][j] = 0;
}

Mat3::Mat3(std::initializer_list<float> p_initList)
{
	if (p_initList.size() < 9)
		std::cout << "Insufficient elements" << std::endl;
	else
	{
		auto it = p_initList.begin();
		for (int i = 0; i < 3; ++i)
			for (int j = 0; j < 3; ++j)
				this->m_matrix[i][j] = *it++;
	}
}

Mat3::Mat3(const Mat3& p_other)
{
	for (int i = 0; i < 3; ++i)
		for (int j = 0; j < 3; ++j)
			this->m_matrix[i][j] = p_other.m_matrix[i][j];
}


Mat3::~Mat3()
{
}

float Mat3::Determinant()
{
	return m_matrix[0][0] * (m_matrix[1][1] * m_matrix[2][2] - m_matrix[1][2] * m_matrix[2][1]) -
		m_matrix[0][1] * (m_matrix[1][0] * m_matrix[2][2] - m_matrix[1][2] * m_matrix[2][0]) +
		m_matrix[0][2] * (m_matrix[1][0] * m_matrix[2][1] - m_matrix[1][1] * m_matrix[2][0]);
}
