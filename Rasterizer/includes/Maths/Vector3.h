#pragma once

#include <string>
#include <SDL_stdinc.h>

namespace Maths
{
	template<typename T>
	class Vector3
	{
	public:
		T x;
		T y;
		T z;

	public:
		static float ToDegree(const double &angle)
		{
			return angle * 180.0 / M_PI;
		}
		static float ToRad(const double &angle)
		{
			return static_cast<float>(angle * M_PI / 180.0f);
		}

		Vector3(T p_x, T p_y, T p_z = 0.0f) : x{ p_x }, y{ p_y }, z{ p_z } {}
		Vector3(const Vector3 &p_other) : x{p_other.x} , y{p_other.y}, z{p_other.z}
		{
		}

		Vector3& operator=(const Vector3& p_other)
		{
			this->x = p_other.x;
			this->y = p_other.y;
			this->z = p_other.z;
			return *this;
		}

		~Vector3() = default;

		Vector3 Normalize() const
		{
			float mag = this->GetMagnitude();
			if (mag != 0 && mag != 1)
			{
				return this->Div(mag);
			}
			return *this;
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

		T GetMagnitude() const { return sqrt(MagSq()); }
		T DotProduct(const Vector3 &other) const { return (this->x * other.x) + (this->y * other.y) + (this->z * other.z); }
		Vector3 CrossProduct(const Vector3 &other) const
		{
			float crossX = (this->y * other.z) - (other.y * this->z);
			float crossY = (this->z * other.x) - (other.z * this->x);
			float crossZ = (this->x * other.y) - (other.x * this->y);

			return Vector3(crossX, crossY, crossZ);
		}		
		float CrossProduct2D(const Vector3 &other) const
		{
			return ((this->x * other.y) - (this->y - other.x));
		}

		float AngleBetween(const Vector3 &other) const
		{
			if (this->x == 0 && this->y == 0 && this->z == 0) return 0.0f;
			if (other.x == 0 && other.y == 0 && other.z == 0) return 0.0f;


			const double amt = this->DotProduct(other) / (this->GetMagnitude() * other.GetMagnitude());

			if (amt <= -1) {
				return M_PI;
			}
			else if (amt >= 1) {
				return 0;
			}
			return static_cast<float>(acos(amt));
		}

		T operator,(const Vector3 &other) const { return this->DotProduct(other); }


		// SCALAR OPERATIONS

		Vector3 operator+(const Vector3 &other) const { return Vector3(this->x + other.x, this->y + other.y, this->z + other.z); }
		Vector3 &operator+=(const Vector3 &other)
		{
			this->x += other.x;
			this->y += other.y;
			this->z += other.z;
			return *this;
		}

		Vector3 Add(const Vector3 &other) const { return Vector3(this->x + other.x, this->y + other.y, this->z + other.z); }
		Vector3 &Add(const Vector3 &other)
		{
			this->x += other.x;
			this->y += other.y;
			this->z += other.z;
			return *this;
		}

		Vector3 operator-(const Vector3 &other) const { return Vector3(this->x - other.x, this->y - other.y, this->z - other.z); }
		Vector3 &operator-=(const Vector3 &other)
		{
			this->x -= other.x;
			this->y -= other.y;
			this->z -= other.z;
			return *this;
		}
		Vector3 Sub(const Vector3 &other) const { return Vector3(this->x - other.x, this->y - other.y, this->z - other.z); }
		Vector3 &Sub(const Vector3 &other)
		{
			this->x -= other.x;
			this->y -= other.y;
			this->z -= other.z;
			return *this;
		}

		Vector3 operator*(const T n) const { return Vector3(this->x*n, this->y*n, this->z*n); }
		Vector3 &operator*=(const T n)
		{
			this->x *= n;
			this->y *= n;
			this->z *= n;
			return *this;
		}
		Vector3 Scale(const T n) const { return Vector3(this->x*n, this->y*n, this->z*n); }
		Vector3 &Scale(const T n)
		{
			this->x *= n;
			this->y *= n;
			this->z *= n;
			return *this;
		}

		Vector3 operator/(const T n) const { return Vector3(this->x / n, this->y / n, this->z / n); }
		Vector3 &operator/=(const T n)
		{
			this->x /= n;
			this->y /= n;
			this->z /= n;
			return *this;
		}
		Vector3 Div(T n) const { return Vector3(this->x / n, this->y / n, this->z / n); }
		Vector3 &Div(T n)
		{
			this->x /= n;
			this->y /= n;
			this->z /= n;
			return *this;
		}

		Vector3 &operator++(int)
		{
			T length = this->GetMagnitude();
			T mult = (length + 1) / length;
			this->x *= mult;
			this->y *= mult;
			this->z *= mult;
			return *this;
		}
		Vector3 &operator--(int)
		{
			T length = this->GetMagnitude();
			T mult = (length - 1) / length;
			this->x *= mult;
			this->y *= mult;
			this->z *= mult;
			return *this;
		}

		// Tests

		bool IsParallel(const Vector3 &other) const
		{
			return (this->x / other.x == this->y / other.y) && (this->x / other.x == this->z / other.z);
		}

		bool IsOpposed(const Vector3 &other) const
		{
			return (this->IsParallel(other) && *this + other < *this);
		}

		bool IsPerpendicular(const Vector3 &other) const
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

		T& operator[](const int coord)
		{
			if (coord == 0)
				return this->x;
			if (coord == 1)
				return this->y;
			if (coord == 2)
				return this->z;
			return this->x;
		}
		const T& operator[](const int coord) const
		{
			if (coord == 0)
				return this->x;
			if (coord == 1)
				return this->y;
			if (coord == 2)
				return this->z;
			return this->x;
		}
		// Comparisons

		bool operator==(const Vector3 &other) const
		{
			return this->GetMagnitude() == other.GetMagnitude();
		}

		bool operator!=(const Vector3 &other) const
		{
			return this->GetMagnitude() != other.GetMagnitude();
		}

		bool operator<(const Vector3 &other) const
		{
			return this->GetMagnitude() < other.GetMagnitude();
		}

		bool operator<=(const Vector3 &other) const
		{
			return this->GetMagnitude() <= other.GetMagnitude();
		}

		bool operator>(const Vector3 &other) const
		{
			return this->GetMagnitude() > other.GetMagnitude();
		}

		bool operator>=(const Vector3 &other) const
		{
			return this->GetMagnitude() >= other.GetMagnitude();
		}


	};

	// Strings

	template<typename T>
	std::ostream &operator<<(std::ostream &os, const Vector3<T> &v)
	{
		os << v.x << "," << v.y << "," << v.z;
		return os;
	}

	template<typename T>
	std::string &operator+(std::string &str, const Vector3<T> &v)
	{
		str += "x:" + std::to_string(v.x) + " y:" + std::to_string(v.y) + " z:" + std::to_string(v.z);
		return str;
	}

	template<typename T>
	std::string &operator+=(std::string &str, const Vector3<T> &v)
	{
		str += "x:" + std::to_string(v.x) + " y:" + std::to_string(v.y) + " z:" + std::to_string(v.z);
		return str;
	}

	template<typename T>
	Vector3<T> operator-(const Vector3<T> &v) { return Vector3<T>(-v.x, -v.y, -v.z); }

	template<typename T>
	Vector3<T> operator*(const T n, const Vector3<T> &v) { return Vector3<T>(v.x*n, v.y*n, v.z*n); }

	template<typename T>
	Vector3<T> operator/(const T n, const Vector3<T> &v) { return Vector3<T>(v.x / n, v.y / n, v.z / n); }

	typedef Vector3<float> Vec3;
	typedef Vector3<float> Vector3f;
	typedef Vector3<double> Vector3d;
	typedef Vector3<int> Vector3i;

}
