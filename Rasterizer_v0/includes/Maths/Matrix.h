#pragma once

#include <iostream>
#include <vector>
#include <cmath>
#include "Vector3.h"
#include "Vector4.h"

namespace Maths
{

	template <typename T, unsigned int R, unsigned int C>
	class Matrix
	{
	public:
		T m_matrix[R][C];

	public:
		Matrix()
		{
			for (int i = 0; i < R; ++i)
				for (int j = 0; j < C; ++j)
					this->m_matrix[i][j] = 0;
		}

		Matrix(std::initializer_list<T> p_initList)
		{
			if (p_initList.size() < R * C)
				std::cout << "Insufficient elements" << std::endl;
			else
			{
				auto it = p_initList.begin();
				for (int i = 0; i < R; ++i)
					for (int j = 0; j < C; ++j)
						this->m_matrix[i][j] = *it++;
			}
		}

		Matrix(const Matrix<T, R, C> &p_other)
		{
			for (int i = 0; i < R; ++i)
				for (int j = 0; j < C; ++j)
					this->m_matrix[i][j] = p_other.m_matrix[i][j];
		}

		~Matrix() {}

		// TRANFORMATION MATRICES

		static Matrix<T, 4, 4> CreateTranslationMatrix(const Vector3<T>& p_translation)
		{
			return  { 1,0,0,p_translation.X(),
					 0,1,0,p_translation.Y(),
					 0,0,1,p_translation.Z(),
					 0,0,0,1 };
		}
		static Matrix<T, 4, 4> CreateScaleMatrix(const Vector3<T>& p_scale)
		{
			return  { p_scale.X(),0,0,0,
					 0,p_scale.Y(),0,0,
					 0,0,p_scale.Z(),0,
					 0,0,0,1 };
		}

		static Matrix<T, 2, 2> Create2DRotationMatrix(const float p_angle)
		{
			return  { cos(p_angle), -sin(p_angle),
					 sin(p_angle),cos(p_angle) };
		}
		static Matrix<T, 3, 3> CreateXRotationMatrix(const float p_angle)
		{
			return  { 1,	0,			0,
					 0,	cos(p_angle),	-sin(p_angle),
					 0,	sin(p_angle),	cos(p_angle) };
		}
		static Matrix<T, 3, 3> CreateYRotationMatrix(const float p_angle)
		{
			return  { cos(p_angle),	0,	sin(p_angle),
					 0,				1,	0,
					 -sin(p_angle),	0,	cos(p_angle) };
		}
		static Matrix<T, 3, 3> CreateZRotationMatrix(const float p_angle)
		{
			return  { cos(p_angle),	-sin(p_angle),	0,
					 sin(p_angle),	cos(p_angle),	0,
					 0,				0,				1 };
		}

		static Matrix<T, 3, 3> CreateRotationMatrix(const Vec3& p_rotation)
		{
			return  Matrix<T, 3, 3>::CreateXRotationMatrix(Vec3::ToRad(p_rotation.x)) *
				Matrix<T, 3, 3>::CreateYRotationMatrix(Vec3::ToRad(p_rotation.y)) *
				Matrix<T, 3, 3>::CreateZRotationMatrix(Vec3::ToRad(p_rotation.z));
		}

		static Matrix<T, 4, 4> CreateTransformMatrix(const Vec3& p_rotation, const  Vector3<T>& p_position, const  Vector3<T>& p_scale)
		{
			return  Matrix<T, 3, 3>::CreateTranslationMatrix(p_position) *
				Matrix<T, 3, 3>::CreateRotationMatrix(p_rotation) *
				Matrix<T, 3, 3>::CreateScaleMatrix(p_scale);
		}


		// SCALAR OPERATIONS

		Matrix<T, R, C> Add(const T &p_scalar) const
		{
			Matrix<T, R, C> add;
			for (int i = 0; i < R; ++i)
				for (int j = 0; j < C; ++j)
					add.m_matrix[i][j] = this->m_matrix[i][j] + p_scalar;
			return add;
		};
		void Add(const T &p_scalar)
		{
			for (int i = 0; i < R; ++i)
				for (int j = 0; j < C; ++j)
					this->m_matrix[i][j] += p_scalar;
		};

		Matrix<T, R, C> Sub(const T &p_scalar) const
		{
			Matrix<T, R, C> sub;
			for (int i = 0; i < R; ++i)
				for (int j = 0; j < C; ++j)
					sub.m_matrix[i][j] = this->m_matrix[i][j] - p_scalar;
			return sub;
		};
		void Sub(const T &p_scalar)
		{
			for (int i = 0; i < R; ++i)
				for (int j = 0; j < C; ++j)
					this->m_matrix[i][j] -= p_scalar;
		};

		Matrix<T, R, C> Scale(const T &p_scalar) const
		{
			Matrix<T, R, C> scale;
			for (int i = 0; i < R; ++i)
				for (int j = 0; j < C; ++j)
					scale.m_matrix[i][j] = this->m_matrix[i][j] * p_scalar;
			return scale;
		}
		void Scale(const T &p_scalar)
		{
			for (int i = 0; i < R; ++i)
				for (int j = 0; j < C; ++j)
					this->m_matrix[i][j] *= p_scalar;
		}

		Matrix<T, R, C> Div(const T &p_scalar) const
		{
			Matrix<T, R, C> div;
			for (int i = 0; i < R; ++i)
				for (int j = 0; j < C; ++j)
					div.m_matrix[i][j] = this->m_matrix[i][j] / p_scalar;
			return div;
		}
		void Div(const T &p_scalar)
		{
			for (int i = 0; i < R; ++i)
				for (int j = 0; j < C; ++j)
					this->m_matrix[i][j] /= p_scalar;
		}



		// MATRICES ARTIHMETICS OPERATIONS

		Matrix<T, R, C> Add(const Matrix<T, R, C> &p_other) const
		{
			Matrix<T, R, C> add;
			for (int i = 0; i < R; ++i)
				for (int j = 0; j < C; ++j)
					add.m_matrix[i][j] = this->m_matrix[i][j] + p_other.m_matrix[i][j];
			return add;
		}
		void Add(const Matrix<T, R, C> &p_other)
		{
			for (int i = 0; i < R; ++i)
				for (int j = 0; j < C; ++j)
					this->m_matrix[i][j] += p_other.m_matrix[i][j];
		}

		Matrix<T, R, C> Sub(const Matrix<T, R, C> &p_other) const
		{
			Matrix<T, R, C> sub;
			for (int i = 0; i < R; ++i)
				for (int j = 0; j < C; ++j)
					sub.m_matrix[i][j] = this->m_matrix[i][j] - p_other.m_matrix[i][j];
			return sub;
		}
		void Sub(const Matrix<T, R, C> &p_other)
		{
			for (int i = 0; i < R; ++i)
				for (int j = 0; j < C; ++j)
					this->m_matrix[i][j] -= p_other.m_matrix[i][j];
		}

		Matrix<T, R, C> Multiply(const Matrix<T, R, C> &p_other) const
		{
			Matrix<T, R, C> multiply;
			for (unsigned int i = 0; i < R; ++i)
				for (unsigned int j = 0; j < C; ++j)
					multiply.m_matrix[i][j] = VectorsMul(this->GetRow(i), p_other.GetColumn(j));
			return multiply;
		}
		void Multiply(const Matrix<T, R, C> &p_other)
		{
			for (unsigned int i = 0; i < R; ++i)
				for (unsigned int j = 0; j < C; ++j)
					this->m_matrix[i][j] = VectorsMul(this->GetRow(i), p_other.GetColumn(j));
		}

		Matrix<T, R, C> Multiply(const Vector4<T> &p_other) const
		{
			Matrix<T, R, C> multiply;
			if (C != 4)
				std::cout << "Can't be multiply" << std::endl;
			else
			{
				for (unsigned int i = 0; i < R; ++i)
					for (unsigned int j = 0; j < C; ++j)
						multiply.m_matrix[i][j] = VectorsMul(this->GetRow(i), p_other);
			}
			return multiply;
		}
		void Multiply(const Vector4<T> &p_other)
		{
			if (p_other.size() != C)
				std::cout << "Can't be multiply" << std::endl;
			else
			{
				for (unsigned int i = 0; i < R; ++i)
					for (unsigned int j = 0; j < C; ++j)
						this->m_matrix[i][j] = VectorsMul(GetRow(i), p_other);
			}
		}

		// MATRICES TRANSFORMATIONS

		Matrix<T, R, C> Transpose() const
		{
			Matrix<T, C, R> transpose;
			for (unsigned int i = 0; i < R; ++i)
				for (unsigned int j = 0; j < C; ++j)
					transpose.m_matrix[i][j] = m_matrix[j][i];
			return transpose;
		}

		T VectorsMul(const Vector4<T> &p_v1, const Vector4<T> &p_v2) const
		{
			T VectorsMul = 0;
			int i = 0;
			for (int j = 0; i < 4;++j)
			{
				VectorsMul += p_v1[i] * p_v2[j];
				++i;
			}
			return VectorsMul;
		}

		Matrix<T, 3, 3> Rotate2D(const T &p_angle) const
		{
			Matrix<T, 3, 3> rotationMatrix = { cos(p_angle), -sin(p_angle),	0,
											  sin(p_angle), cos(p_angle),	0,
											  0,			0,				1 };
			Matrix<T, 3, 3> rotate = this->Multiply(rotationMatrix);
			return rotate;
		};

		T Determinant(unsigned int p_size)
		{
			T determinant = 0;
			if (!IsSquare())
				std::cout << "Can't find the determinant" << std::endl;
			else
			{
				if (R == 1)
					return m_matrix[0][0];
				else if (R == 2)
				{
					determinant = (m_matrix[0][0] * m_matrix[1][1] - m_matrix[0][1] * m_matrix[1][0]);
					return determinant;
				}
				// TODO Implement generic determinant for all square matrices
	//			else if( R > 2)
	//			{
	//				int sign = 1;
	//				for (unsigned int i = 0; i < p_size; ++i )
	//				{
	//					auto temp = this->MinorMatrix(0,i);
	//					determinant += sign * this->m_matrix[0][i] * temp.Determinant(R - 1);
	//					sign = -sign;
	//				}
	//			}
				else if (p_size == 3)
					return m_matrix[0][0] * (m_matrix[1][1] * m_matrix[2][2] - m_matrix[1][2] * m_matrix[2][1]) -
					m_matrix[0][1] * (m_matrix[1][0] * m_matrix[2][2] - m_matrix[1][2] * m_matrix[2][0]) +
					m_matrix[0][2] * (m_matrix[1][0] * m_matrix[2][1] - m_matrix[1][1] * m_matrix[2][0]);
			}
			return determinant;
		}

		Matrix<T, R - 1, C - 1> MinorMatrix(unsigned int p_i, unsigned int p_j) const
		{
			if (R > 1 && C > 1)
			{
				Matrix<T, R - 1, C - 1> cofactor;
				if (!IsSquare())
					std::cout << "Can't find the cofactor" << std::endl;
				else
				{
					unsigned int i = 0, j = 0;
					for (unsigned int row = 0; row < R; ++row)
						for (unsigned int col = 0; col < C; ++col)
						{
							if (row == p_i || col == p_j)
								continue;

							cofactor.GetRow(i)[j++] = m_matrix[row][col];
							if (j == R - 1)
							{
								j = 0;
								i++;
							}
						}
				}
				return cofactor;
			}
			return Matrix<T,R,C> {};
		}

		Matrix<T, R, C> Inverse() const
		{
			Matrix<T, R, C> inverse;
			if (!IsSquare() && Determinant() != 0)
				std::cout << "Can't find the inverse" << std::endl;
			else
				inverse = Adjoint() / Determinant();
			return inverse;
		}
		void Inverse()
		{
			if (!IsSquare() && Determinant() != 0)
				std::cout << "Can't find the inverse" << std::endl;
			else
				this *= Adjoint() / Determinant();
		}

		Matrix<T, R, C> Adjoint() const
		{
			Matrix<T, R, C> adjoint;
			if (!IsSquare())
				std::cout << "Can't find the adjoint" << std::endl;
			else
			{
				for (unsigned int row = 0; row < R; ++row)
				{
					for (unsigned int col = 0; col < C; ++col)
					{
						Matrix<T, R-1, C-1> minor = this->MinorMatrix(row, col);
						adjoint.m_matrix[row][col] = minor.Determinant(R-1);
					}
				}
			}
			return adjoint.Transpose();
		}

		const T& operator()(unsigned int p_row, unsigned int p_col) const
		{
			if (p_row > R || p_col > C)
				std::cout << "Out of Matrix" << std::endl;
			else
				return this->m_matrix[p_row][p_col];
			return {};
		}
		T& operator()(unsigned int p_row, unsigned int p_col)
		{
			if (p_row >= R || p_col >= C)
				std::cout << "Out of Matrix" << std::endl;
			else
				return this->m_matrix[p_row][p_col];
			return {};
		}

		// GETTERS

		T** GetData()
		{
			return reinterpret_cast<T**>(this->m_matrix);
		}

		Vector4<T> GetRow(unsigned int p_row) const
		{
			if (p_row < R)
			{
				return Vector4<T>(this->m_matrix[p_row][0],
					this->m_matrix[p_row][1],
					this->m_matrix[p_row][2],
					this->m_matrix[p_row][3]);
			}
			std::cout << "Out of Matrix" << std::endl;
			return;

		}

		Vector4<T> GetColumn(unsigned int p_col) const
		{
			if (p_col < C)
			{
				return Vector4<T>(this->m_matrix[0][p_col],
					this->m_matrix[1][p_col],
					this->m_matrix[2][p_col],
					this->m_matrix[3][p_col]);
			}
			std::cout << "Out of Matrix" << std::endl;
			return;

		}

		const T& GetValue(unsigned int p_row, unsigned int p_col) const
		{
			if (p_row < R && p_col < C)
			{
				return this->m_matrix[p_row][p_col];
			}
			std::cout << "Out of Matrix" << std::endl;
			return nullptr;
		}
		T& GetValue(unsigned int p_row, unsigned int p_col)
		{
			if (p_row < R && p_col < C)
			{
				return this->m_matrix[p_row][p_col];
			}
			std::cout << "Out of Matrix" << std::endl;
			return nullptr;
		}

		unsigned int GetSize1() const { return R; }
		unsigned int GetSize2() const { return C; }

		// TESTS

		bool IsEqual(const Matrix<T, R, C> &p_other)
		{
			for (unsigned int i = 0; i < R; ++i)
				for (unsigned int j = 0; j < C; ++j)
					if (m_matrix[i][j] != p_other.m_matrix[i][j])
						return false;
			return true;
		}
		bool IsOrthogonal() const { return Multiply(Transpose()).IsIdentity(); }
		bool IsIdentity() const
		{
			if (!IsSquare())
				return false;

			for (unsigned int i = 0; i < R; ++i)
				for (unsigned int j = 0; j < C; ++j)
				{
					if (i == j && m_matrix[i][j] != 1)
						return false;
					if (i != j && m_matrix[i][j] != 0)
						return false;

				}
			return true;
		}
		bool IsSquare() const { return R == C; }
		bool IsVector() const { return R == 1 || C == 1; }

		// OPERATORS

		Matrix<T, R, C> operator+(const Matrix<T, R, C> &p_other) const { return Add(p_other); }
		Matrix<T, R, C> operator+=(const Matrix<T, R, C> &p_other) { return Add(p_other); }
		Matrix<T, R, C> operator-(const Matrix<T, R, C> &p_other) const { return Sub(p_other); }
		Matrix<T, R, C> operator-=(const Matrix<T, R, C> &p_other) { return Sub(p_other); }
		Matrix<T, R, C> operator*(const Matrix<T, R, C> &p_other) const { return Multiply(p_other); }
		Matrix<T, R, C> operator*=(const Matrix<T, R, C> &p_other) { return Multiply(p_other); }

		Matrix<T, R, C> operator*(const Vector4<T>& p_other) const { return Multiply(p_other); }
		Matrix<T, R, C> operator*=(const Vector4<T> &p_other) { return Multiply(p_other); }

		Matrix<T, R, C> operator*(const T &p_scalar) const { return Scale(p_scalar); }
		Matrix<T, R, C> operator*=(const T &p_scalar) { return Scale(p_scalar); }
		Matrix<T, R, C> operator/(const T &p_scalar) const { return Div(p_scalar); }
		Matrix<T, R, C> operator/=(const T &p_scalar) { return Div(p_scalar); }
		Matrix<T, R, C> operator+(const T &p_scalar) const { return Add(p_scalar); }
		Matrix<T, R, C> operator+=(const T &p_scalar) { return Add(p_scalar); }
		Matrix<T, R, C> operator-(const T &p_scalar) const { return Sub(p_scalar); }
		Matrix<T, R, C> operator-=(const T &p_scalar) { return Sub(p_scalar); }


		bool operator==(const Matrix<T, R, C> &p_other) { return IsEqual(p_other); }
		bool operator!=(const Matrix<T, R, C> &p_other) { return !IsEqual(p_other); }
	};

	template <typename T, unsigned int R, unsigned int C>
	std::ostream &operator<<(std::ostream &p_os, const Matrix<T, R, C> &p_other)
	{
		for (int i = 0; i < R; ++i)
		{
			for (int j = 0; j < C; ++j)
				p_os << p_other(i, j) << " ";
			p_os << std::endl;
		}
		return p_os;
	}

	typedef Matrix<float, 4, 4> Mat4;
};