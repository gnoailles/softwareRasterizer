#pragma once

#include <Maths/Vector3.h>
#include <Maths/Vector4.h>
#include "Mat3.h"

namespace Maths
{
	class Mat4
	{
	public:
		float m_matrix[4][4];

	public:
		Mat4();
		Mat4(std::initializer_list<float> p_initList);
		Mat4(const Mat4& p_other);

		~Mat4() {}

		//UTILS
		static int Sign(const unsigned int &p_i, const unsigned int &p_j);

		// TRANFORMATION MATRICES
		static Mat4 CreateTranslationMatrix(const Vector3<float>& p_translation);
		static Mat4 CreateScaleMatrix(const Vector3<float>& p_scale);

		static Mat4 CreateXRotationMatrix(const float p_angle);
		static Mat4 CreateYRotationMatrix(const float p_angle);
		static Mat4 CreateZRotationMatrix(const float p_angle);

		static Mat4 CreateRotationMatrix(const Vec3& p_rotation);

		static Mat4 CreateTransformMatrix(const Vec3& p_position, const Vec3& p_rotation, const Vec3& p_scale);


		// SCALAR OPERATIONS
		Mat4 Add(const float &p_scalar) const;
		void Add(const float &p_scalar);

		Mat4 Sub(const float &p_scalar) const;
		void Sub(const float &p_scalar);

		Mat4 Scale(const float &p_scalar) const;
		void Scale(const float &p_scalar);

		Mat4 Div(const float &p_scalar) const;
		void Div(const float &p_scalar);


		// MATRICES ARTIHMETICS OPERATIONS
		Mat4 Add(const Mat4 &p_other) const;
		void Add(const Mat4 &p_other);

		Mat4 Sub(const Mat4 &p_other) const;
		void Sub(const Mat4 &p_other);

		Mat4 Multiply(const Mat4 &p_other) const;
		void Multiply(const Mat4 &p_other);

		Vector4<float> Multiply(const Vector4<float> &p_other) const;
		void Multiply(const Vector4<float> &p_other);

		static float VectorsMul(const Vector4<float> &p_v1, const Vector4<float> &p_v2);

		// MATRICES TRANSFORMATIONS
		Mat4 Transpose() const;
		float Determinant() const;
		Mat4 Adjoint() const;
		Mat3 Minor(int i, int j) const;
		Mat4 Inverse() const;

	
		// GETTERS
		float** GetData();
		Vector4<float> GetRow(unsigned int p_row) const;
		Vector4<float> GetColumn(unsigned int p_col) const;

		const float& GetValue(unsigned int p_row, unsigned int p_col) const;
		float& GetValue(unsigned int p_row, unsigned int p_col);

		const float& operator()(unsigned int p_row, unsigned int p_col) const;
		float& operator()(unsigned int p_row, unsigned int p_col);


		// TESTS
		bool IsEqual(const Mat4 &p_other);
		bool IsOrthogonal() const;
		bool IsIdentity() const;

		// OPERATORS

		Mat4 operator+(const Mat4 &p_other) const;
		void operator+=(const Mat4 &p_other);

		Mat4 operator-(const Mat4 &p_other) const;
		void operator-=(const Mat4 &p_other);

		Mat4 operator*(const Mat4 &p_other) const;
		void operator*=(const Mat4 &p_other);


		Vector4<float> operator*(const Vector4<float>& p_other) const;
		void operator*=(const Vector4<float> &p_other);


		Mat4 operator+(const float &p_scalar) const;
		void operator+=(const float &p_scalar);

		Mat4 operator-(const float &p_scalar) const;
		void operator-=(const float &p_scalar);

		Mat4 operator*(const float &p_scalar) const;
		void operator*=(const float &p_scalar);

		Mat4 operator/(const float &p_scalar) const;
		void operator/=(const float &p_scalar);


		bool operator==(const Mat4 &p_other);
		bool operator!=(const Mat4 &p_other);
	};

	std::ostream &operator<<(std::ostream &p_os, const Mat4 &p_other);
};