#pragma once
#pragma once

#define _USE_MATH_DEFINES

#include <math.h>
#include <string>
#include "Vector3.h"

namespace Maths
{
	template<typename T>
	class Vector4
	{
	public:
		T x;
		T y;
		T z;
		T w;

	public:
		static float ToDegree(const double &angle)
		{
			return angle * 180.0 / M_PI;
		}
		static float ToRad(const double &angle)
		{
			return angle * M_PI / 180.0f;
		}

		Vector4(T p_x, T p_y, T p_z = 0.0f, T p_w = 1.0f) : x{ p_x }, y{ p_y }, z{ p_z }, w{ p_w } {}
		Vector4(const Vec3& vec3, T p_w = 1.0f) : x{ vec3.x }, y{ vec3.y }, z{ vec3.z }, w{ p_w } {}
		Vector4(const Vector4 &other)
		{
			this->x = other.x;
			this->y = other.y;
			this->z = other.z;
			this->w = other.w;
		}

		Vector4 &operator=(const Vector4 &other)
		{
			this->x = other.x;
			this->y = other.y;
			this->z = other.z;
			this->w = other.w;
			return *this;
		}

		~Vector4() = default;

		void Homogenize()
		{
			if (this.w != 0)
			{
				this->Div(this.w);
				this->w /= this->w;
			}
		}		
		Vector4 Homogenize() const
		{
			if (this.w != 0)
			{
				Vector4 homogenized = this->Div(this.w);
				homogenized->w /= this->w;
				return homogenized;
			}
			return Vector4();
		}

		Vector4 Normalize() const
		{
			float mag = this->GetMagnitude();
			if (mag != 0 && mag != 1)
			{
				return this->Div(mag);
			}
			return this;
		}

		void Normalize()
		{
			float mag = this->GetMagnitude();
			if (mag != 0 && mag != 1)
			{
				this->Div(mag);
			}
		}

		T MagSq() const { return (this->x * this->x) + (this->y * this->y) + (this->z * this->z); }

		T GetMagnitude() const
		{
			if (this.w != 1.0f)
			{
				this->Homogenize();
			}
			return sqrt(MagSq());
			
		}
		T DotProduct(const Vector4 &other) const { return (this->x * other.x) + (this->y * other.y) + (this->z * other.z); }
		Vector4 CrossProduct(const Vector4 &other) const
		{
			float crossX = (this->y * other.z) - (other.y * this->z);
			float crossY = (this->z * other.x) - (other.z * this->x);
			float crossZ = (this->x * other.y) - (other.x * this->y);

			return Vector4(crossX, crossY, crossZ);
		}

		float AngleBetween(const Vector4 &other) const
		{
			if (this->x == 0 && this->y == 0 && this->z == 0) return 0.0f;
			if (other.x == 0 && other.y == 0 && other.z == 0) return 0.0f;


			double amt = this->DotProduct(other) / (this->GetMagnitude() * other.GetMagnitude());

			if (amt <= -1) {
				return M_PI;
			}
			else if (amt >= 1) {
				return 0;
			}
			return static_cast<float>(acos(amt));
		}

		T operator,(const Vector4 &other) const { return this->DotProduct(other); }


		// SCALAR OPERATIONS

		Vector4 operator+(const Vector4 &other) const { return Vector4(this->x + other.x, this->y + other.y, this->z + other.z); }
		Vector4 &operator+=(const Vector4 &other)
		{
			this->x += other.x;
			this->y += other.y;
			this->z += other.z;
			return *this;
		}

		Vector4 Add(const Vector4 &other) const { return Vector4(this->x + other.x, this->y + other.y, this->z + other.z); }
		Vector4 &Add(const Vector4 &other)
		{
			this->x += other.x;
			this->y += other.y;
			this->z += other.z;
			return *this;
		}

		Vector4 operator-(const Vector4 &other) const { return Vector4(this->x - other.x, this->y - other.y, this->z - other.z); }
		Vector4 &operator-=(const Vector4 &other)
		{
			this->x -= other.x;
			this->y -= other.y;
			this->z -= other.z;
			return *this;
		}
		Vector4 Sub(const Vector4 &other) const { return Vector4(this->x - other.x, this->y - other.y, this->z - other.z); }
		Vector4 &Sub(const Vector4 &other)
		{
			this->x -= other.x;
			this->y -= other.y;
			this->z -= other.z;
			return *this;
		}

		Vector4 operator*(const T n) const { return Vector4(this->x*n, this->y*n, this->z*n); }
		Vector4 &operator*=(const T n)
		{
			this->x *= n;
			this->y *= n;
			this->z *= n;
			return *this;
		}
		Vector4 Scale(const T n) const { return Vector4(this->x*n, this->y*n, this->z*n); }
		Vector4 &Scale(const T n)
		{
			this->x *= n;
			this->y *= n;
			this->z *= n;
			return *this;
		}

		Vector4 operator/(const T n) const { return Vector4(this->x / n, this->y / n, this->z / n); }
		Vector4 &operator/=(const T n)
		{
			this->x /= n;
			this->y /= n;
			this->z /= n;
			return *this;
		}
		Vector4 Div(T n) const { return Vector4(this->x / n, this->y / n, this->z / n); }
		Vector4 &Div(T n)
		{
			this->x /= n;
			this->y /= n;
			this->z /= n;
			return *this;
		}

		Vector4 &operator++(int)
		{
			T length = this->GetMagnitude();
			T mult = (length + 1) / length;
			this->x *= mult;
			this->y *= mult;
			this->z *= mult;
			return *this;
		}
		Vector4 &operator--(int)
		{
			T length = this->GetMagnitude();
			T mult = (length - 1) / length;
			this->x *= mult;
			this->y *= mult;
			this->z *= mult;
			return *this;
		}

		// Tests

		bool IsParallel(const Vector4 &other) const
		{
			return (this->x / other.x == this->y / other.y) && (this->x / other.x == this->z / other.z);
		}

		bool IsOpposed(const Vector4 &other) const
		{
			return (this->IsParallel(other) && *this + other < *this);
		}

		bool IsPerpendicular(const Vector4 &other) const
		{
			return (this->DotProduct(other) == 0);
		}

		bool IsNormalized() const
		{
			return this->GetMagnitude() == 1;
		}

		bool IsNull() const
		{
			return (x == 0 && y == 0 && z == 0);
		}

		T &operator[](const int coord)
		{
			if (coord == 0)
				return this->x;
			else if (coord == 1)
				return this->y;
			else if (coord == 2)
				return this->z;
			else if (coord == 3)
				return this->w;
			return 0;
		}

		T &operator[](const char coord)
		{
			if (coord == 'x')
				return this->x;
			else if (coord == 'y')
				return this->y;
			else if (coord == 'z')
				return this->z;
			else if (coord == 'w')
				return this->w;
			return 0;
		}

		T &operator[](const char coord[2])
		{
			if (coord[0] == 'x')
				return this->x;
			else if (coord[0] == 'y')
				return this->y;
			else if (coord[0] == 'z')
				return this->z;
			else if (coord[0] == 'w')
				return this->w;
			return 0;
		}

		// Comparisons

		bool operator==(const Vector4 &other) const
		{
			return this->GetMagnitude() == other.GetMagnitude();
		}

		bool operator!=(const Vector4 &other) const
		{
			return this->GetMagnitude() != other.GetMagnitude();
		}

		bool operator<(const Vector4 &other) const
		{
			return this->GetMagnitude() < other.GetMagnitude();
		}

		bool operator<=(const Vector4 &other) const
		{
			return this->GetMagnitude() <= other.GetMagnitude();
		}

		bool operator>(const Vector4 &other) const
		{
			return this->GetMagnitude() > other.GetMagnitude();
		}

		bool operator>=(const Vector4 &other) const
		{
			return this->GetMagnitude() >= other.GetMagnitude();
		}


	};

	// Strings

	template<typename T>
	std::ostream &operator<<(std::ostream &os, const Vector4<T> &v)
	{
		os << v.x << "," << v.y << "," << v.z;
		return os;
	}

	template<typename T>
	std::string &operator+(std::string &str, const Vector4<T> &v)
	{
		str += "x:" + std::to_string(v.x) + " y:" + std::to_string(v.y) + " z:" + std::to_string(v.z);
		return str;
	}

	template<typename T>
	std::string &operator+=(std::string &str, const Vector4<T> &v)
	{
		str += "x:" + std::to_string(v.x) + " y:" + std::to_string(v.y) + " z:" + std::to_string(v.z);
		return str;
	}

	template<typename T>
	Vector4<T> operator-(const Vector4<T> &v) { return Vector4<T>(-v.x, -v.y, -v.z); }

	template<typename T>
	Vector4<T> operator*(const T n, const Vector4<T> &v) { return Vector4<T>(v.x*n, v.y*n, v.z*n); }

	template<typename T>
	Vector4<T> operator/(const T n, const Vector4<T> &v) { return Vector4<T>(v.x / n, v.y / n, v.z / n); }

	typedef Vector4<float> Vec4;
	typedef Vector4<float> Vector4f;
	typedef Vector4<double> Vector4d;
	typedef Vector4<int> Vector4i;

}
