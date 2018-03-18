#pragma once

#include <string>

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

		Vector4(const T& p_x = 0.0f, const T& p_y = 0.0f, const T& p_z = 0.0f, const T& p_w = 1.0f) : x{ p_x }, y{ p_y }, z{ p_z }, w{ p_w } {}
		Vector4(const Vector3<T>& vec3, const T& p_w = 1.0f) : x{ vec3.x }, y{ vec3.y }, z{ vec3.z }, w{ p_w } {}
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
	
		Vector4 Homogenize() const
		{
			if (this->w != 0 && this->w != 1)
			{
				Vector4 homogenized = this->Div(this->w);
				homogenized.w = 1;
				return homogenized;
			}
			return *this;
		}

		void Homogenize()
		{
			if (this->w != 0 && this->w != 1)
			{
				this->Div(this->w);
				this->w /= this->w;
			}
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
			if (this->w != 1.0f)
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


			const double amt = this->DotProduct(other) / (this->GetMagnitude() * other.GetMagnitude());

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

		Vector3<T> ToVec3() const
		{
			Vector4<T> homogenized = this->Homogenize();
			return Vector3<T>(homogenized.x, homogenized.y, homogenized.z);
		}

		T& operator[](const int coord)
		{
			if (coord == 0)
				return this->x;
			if (coord == 1)
				return this->y;
			if (coord == 2)
				return this->z;
			if (coord == 3)
				return this->w;
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
			if (coord == 3)
				return this->w;
			return this->x;
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
