#include <Maths/Mat4.h>
#include <iostream>

using namespace Maths;

Mat4::Mat4()
{
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
			this->m_matrix[i][j] = 0;
}

Mat4::Mat4(std::initializer_list<float> p_initList)
{
	if (p_initList.size() < 16)
		std::cout << "Insufficient elements" << std::endl;
	else
	{
		auto it = p_initList.begin();
		for (int i = 0; i < 4; ++i)
			for (int j = 0; j < 4; ++j)
				this->m_matrix[i][j] = *it++;
	}
}

Mat4::Mat4(const Mat4& p_other)
{
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
			this->m_matrix[i][j] = p_other.m_matrix[i][j];
}


int Mat4::Sign(const unsigned& p_i, const unsigned& p_j)
{
	return static_cast<int>(powf(-1, (p_i + 1.f) + (p_j + 1.f)));
}

// TRANFORMATION MATRICES
Mat4 Mat4::CreateTranslationMatrix(const Vector3<float>& p_translation)
{
	return  {	1,0,0,p_translation.x,
				0,1,0,p_translation.y,
				0,0,1,p_translation.z,
				0,0,0,1 };
}

Mat4 Mat4::CreateScaleMatrix(const Vector3<float>& p_scale)
{
	return  {	p_scale.x,0,0,0,
				0,p_scale.y,0,0,
				0,0,p_scale.z,0,
				0,0,0,1 };
}

Mat4 Mat4::CreateXRotationMatrix(const float p_angle)
{
	return  {	1,		0,				0,				0,
				0,		cos(p_angle),	-sin(p_angle),	0,
				0,		sin(p_angle),	cos(p_angle),	0,
				0,		0,				0,				1 };
}

Mat4 Mat4::CreateYRotationMatrix(const float p_angle)
{
	return  {	cos(p_angle),	0,	sin(p_angle),	0,
				0,				1,	0,				0,
				-sin(p_angle),	0,	cos(p_angle),	0,
				0,				0,	0,				1 };
}

Mat4 Mat4::CreateZRotationMatrix(const float p_angle)
{
	return  {	cos(p_angle),	-sin(p_angle),	0,	0,
				sin(p_angle),	cos(p_angle),	0,	0,
				0,				0,				1,	0,
				0,				0,				0,	1 };
}

Mat4 Mat4::CreateRotationMatrix(const Vec3& p_rotation)
{
	return  CreateYRotationMatrix(Vec3::ToRad(p_rotation.y)) *
			CreateXRotationMatrix(Vec3::ToRad(p_rotation.x)) *
			CreateZRotationMatrix(Vec3::ToRad(p_rotation.z));
}

Mat4 Mat4::CreateTransformMatrix(const Vec3& p_position, const Vec3& p_rotation, const Vec3& p_scale)
{
	return  CreateTranslationMatrix(p_position) *
			CreateRotationMatrix(p_rotation) * 
			CreateScaleMatrix(p_scale);
}


// SCALAR OPERATIONS
Mat4 Mat4::Add(const float& p_scalar) const
{
	Mat4 add;
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
			add.m_matrix[i][j] = this->m_matrix[i][j] + p_scalar;
	return add;
}

void Mat4::Add(const float& p_scalar)
{
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
			this->m_matrix[i][j] += p_scalar;
}

Mat4 Mat4::Sub(const float& p_scalar) const
{
	Mat4 sub;
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
			sub.m_matrix[i][j] = this->m_matrix[i][j] - p_scalar;
	return sub;
}

void Mat4::Sub(const float& p_scalar)
{
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
			this->m_matrix[i][j] -= p_scalar;
}

Mat4 Mat4::Scale(const float& p_scalar) const
{
	Mat4 scale;
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
			scale.m_matrix[i][j] = this->m_matrix[i][j] * p_scalar;
	return scale;
}

void Mat4::Scale(const float& p_scalar)
{
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
			this->m_matrix[i][j] *= p_scalar;
}

Mat4 Mat4::Div(const float& p_scalar) const
{
	Mat4 div;
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
			div.m_matrix[i][j] = this->m_matrix[i][j] / p_scalar;
	return div;
}

void Mat4::Div(const float& p_scalar)
{
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
			this->m_matrix[i][j] /= p_scalar;
}


// MATRICES ARTIHMETICS OPERATIONS
Mat4 Mat4::Add(const Mat4& p_other) const
{
	Mat4 add;
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
			add.m_matrix[i][j] = this->m_matrix[i][j] + p_other.m_matrix[i][j];
	return add;
}

void Mat4::Add(const Mat4& p_other)
{
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
			this->m_matrix[i][j] += p_other.m_matrix[i][j];
}

Mat4 Mat4::Sub(const Mat4& p_other) const
{
	Mat4 sub;
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
			sub.m_matrix[i][j] = this->m_matrix[i][j] - p_other.m_matrix[i][j];
	return sub;
}

void Mat4::Sub(const Mat4& p_other)
{
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
			this->m_matrix[i][j] -= p_other.m_matrix[i][j];
}

Mat4 Mat4::Multiply(const Mat4& p_other) const
{
	Mat4 multiply;
	for (unsigned int i = 0; i < 4; ++i)
		for (unsigned int j = 0; j < 4; ++j)
			multiply.m_matrix[i][j] = VectorsMul(this->GetRow(i), p_other.GetColumn(j));
	return multiply;
}

void Mat4::Multiply(const Mat4& p_other)
{
	for (unsigned int i = 0; i < 4; ++i)
		for (unsigned int j = 0; j < 4; ++j)
			this->m_matrix[i][j] = VectorsMul(this->GetRow(i), p_other.GetColumn(j));
}

Vector4<float> Mat4::Multiply(const Vector4<float>& p_other) const
{
	Vector4<float> multiply;
	for (unsigned int i = 0; i < 4; ++i)
		multiply[i] = VectorsMul(this->GetRow(i), p_other);
	return multiply;
}

void Mat4::Multiply(const Vector4<float>& p_other)
{
	for (unsigned int i = 0; i < 4; ++i)
		for (unsigned int j = 0; j < 4; ++j)
			this->m_matrix[i][j] = VectorsMul(this->GetRow(i), p_other);
}

float Mat4::VectorsMul(const Vector4<float>& p_v1, const Vector4<float>& p_v2)
{
	float VectorsMul = 0;
	int i = 0;
	for (int j = 0; i < 4; ++j)
	{
		VectorsMul += p_v1[i] * p_v2[j];
		++i;
	}
	return VectorsMul;
}

// MATRICES TRANSFORMATIONS
Mat4 Mat4::Transpose() const
{
	Mat4 transpose;
	for (unsigned int i = 0; i < 4; ++i)
		for (unsigned int j = 0; j < 4; ++j)
			transpose.m_matrix[i][j] = m_matrix[j][i];
	return transpose;
}

float Mat4::Determinant() const
{
	return m_matrix[0][0] * (m_matrix[1][1] * (m_matrix[2][2] * m_matrix[3][3] - m_matrix[2][3] * m_matrix[3][2]) -
			m_matrix[1][2] * (m_matrix[2][1] * m_matrix[3][3] - m_matrix[2][3] * m_matrix[3][1]) +
			m_matrix[1][3] * (m_matrix[2][1] * m_matrix[3][2] - m_matrix[2][2] * m_matrix[3][1])) -
		m_matrix[0][1] * (m_matrix[1][0] * (m_matrix[2][2] * m_matrix[3][3] - m_matrix[2][3] * m_matrix[3][2]) -
			m_matrix[1][2] * (m_matrix[2][0] * m_matrix[3][3] - m_matrix[2][3] * m_matrix[3][0]) +
			m_matrix[1][3] * (m_matrix[2][0] * m_matrix[3][2] - m_matrix[2][2] * m_matrix[3][0])) +
		m_matrix[0][2] * (m_matrix[1][0] * (m_matrix[2][1] * m_matrix[3][3] - m_matrix[2][3] * m_matrix[3][1]) -
			m_matrix[1][1] * (m_matrix[2][0] * m_matrix[3][3] - m_matrix[2][3] * m_matrix[3][0]) +
			m_matrix[1][3] * (m_matrix[2][0] * m_matrix[3][1] - m_matrix[2][1] * m_matrix[3][0])) -
		m_matrix[0][3] * (m_matrix[1][0] * (m_matrix[2][1] * m_matrix[3][2] - m_matrix[2][2] * m_matrix[3][1]) -
			m_matrix[1][1] * (m_matrix[2][0] * m_matrix[3][2] - m_matrix[2][2] * m_matrix[3][0]) +
			m_matrix[1][2] * (m_matrix[2][0] * m_matrix[3][1] - m_matrix[2][1] * m_matrix[3][0]));

}

Mat4 Mat4::Adjoint() const
{
	Mat4 adjoint;
	for (unsigned int i = 0; i < 4; ++i)
		for (unsigned int j = 0; j < 4; ++j)
			adjoint.m_matrix[i][j] = Sign(i, j) * Minor(i, j).Determinant();

	return adjoint;
}

Mat3 Mat4::Minor(const int x, const int y) const
{
	Mat3 minor;
	unsigned int i = 0, j = 0;
	for (unsigned int row = 0; row < 4; ++row)
		for (unsigned int col = 0; col < 4; ++col)
		{
			if (row != x && col != y)
			{
				minor.m_matrix[i][j++] = m_matrix[row][col];
				if (j == 3)
				{
					j = 0;
					++i;
				}
			}
		}
	return minor;
}

Mat4 Mat4::Inverse() const
{
  return Adjoint().Transpose() / Determinant();
}

float** Mat4::GetData()
{
	return reinterpret_cast<float**>(this->m_matrix);
}

Vector4<float> Mat4::GetRow(const unsigned int p_row) const
{
	if (p_row < 4)
	{
		return Vector4<float>(this->m_matrix[p_row][0],
			this->m_matrix[p_row][1],
			this->m_matrix[p_row][2],
			this->m_matrix[p_row][3]);
	}
	std::cout << "Out of Matrix" << std::endl;
	return Vector4<float>();

}

Vector4<float> Mat4::GetColumn(const unsigned int p_col) const
{
	if (p_col < 4)
	{
		return Vector4<float>(this->m_matrix[0][p_col],
			this->m_matrix[1][p_col],
			this->m_matrix[2][p_col],
			this->m_matrix[3][p_col]);
	}
	std::cout << "Out of Matrix" << std::endl;
	return Vector4<float>();

}

const float& Mat4::GetValue(const unsigned int p_row, const unsigned int p_col) const
{
	if (p_row < 4 && p_col < 4)
	{
		return this->m_matrix[p_row][p_col];
	}
	std::cout << "Out of Matrix" << std::endl;
	return this->m_matrix[0][0];
}

float& Mat4::GetValue(const unsigned int p_row, const unsigned int p_col)
{
	if (p_row < 4 && p_col < 4)
	{
		return this->m_matrix[p_row][p_col];
	}
	std::cout << "Out of Matrix" << std::endl;
	return this->m_matrix[0][0];
}

const float& Mat4::operator()(const unsigned int p_row, const unsigned int p_col) const
{
	if (p_row < 4 && p_col < 4)
		return this->m_matrix[p_row][p_col];
	std::cout << "Out of Matrix" << std::endl;
	return this->m_matrix[0][0];
}

float& Mat4::operator()(const unsigned int p_row, const unsigned int p_col)
{
	if (p_row < 4 && p_col < 4)
		return this->m_matrix[p_row][p_col];
	return m_matrix[0][0];
}


// TESTS
bool Mat4::IsEqual(const Mat4& p_other)
{
	for (unsigned int i = 0; i < 4; ++i)
		for (unsigned int j = 0; j < 4; ++j)
			if (m_matrix[i][j] != p_other.m_matrix[i][j])
				return false;
	return true;
}

bool Mat4::IsOrthogonal() const
{
	return Multiply(Transpose()).IsIdentity();
}

bool Mat4::IsIdentity() const
{
	for (unsigned int i = 0; i < 4; ++i)
		for (unsigned int j = 0; j < 4; ++j)
		{
			if (i == j && m_matrix[i][j] != 1)
				return false;
			if (i != j && m_matrix[i][j] != 0)
				return false;

		}
	return true;
}

Mat4 Mat4::operator+(const Mat4& p_other) const { return Add(p_other); }
void Mat4::operator+=(const Mat4& p_other) { Add(p_other); }

Mat4 Mat4::operator-(const Mat4& p_other) const { return Sub(p_other); }
void Mat4::operator-=(const Mat4& p_other) { Sub(p_other); }

Mat4 Mat4::operator*(const Mat4& p_other) const { return Multiply(p_other); }
void Mat4::operator*=(const Mat4& p_other) { Multiply(p_other); }


Vector4<float> Mat4::operator*(const Vector4<float>& p_other) const { return Multiply(p_other); }
void Mat4::operator*=(const Vector4<float>& p_other) { Multiply(p_other); }


Mat4 Mat4::operator+(const float& p_scalar) const { return Add(p_scalar); }
void Mat4::operator+=(const float& p_scalar) { Add(p_scalar); }

Mat4 Mat4::operator-(const float& p_scalar) const { return Sub(p_scalar); }
void Mat4::operator-=(const float& p_scalar) { Sub(p_scalar); }

Mat4 Mat4::operator*(const float& p_scalar) const { return Scale(p_scalar); }
void Mat4::operator*=(const float& p_scalar) { Scale(p_scalar); }

Mat4 Mat4::operator/(const float& p_scalar) const { return Div(p_scalar); }
void Mat4::operator/=(const float& p_scalar) { Div(p_scalar); }


bool Mat4::operator==(const Mat4& p_other) { return IsEqual(p_other); }
bool Mat4::operator!=(const Mat4& p_other) { return !IsEqual(p_other); }


std::ostream& Maths::operator<<(std::ostream& p_os, const Mat4& p_other)
{
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
			p_os << p_other(i, j) << " ";
		p_os << std::endl;
	}
	return p_os;
}
