#pragma once
#include "../libary/sdkapi.hpp"
#include <xmmintrin.h>

constexpr static float M_PI = 3.14159265358979323846f;
constexpr static float M_RADPI = 180.f / M_PI;

#define DEG2RAD(a) (((a) * M_PI) / 180.f)
#define RAD2DEG(a) (((a) * M_RADPI))

class Vector
{
public:
	float x, y, z;

	Vector();
	Vector(float x, float y, float z);
	Vector(float* val);
	Vector(const float* val);
	Vector(int x, int y, int z);
	Vector(const Vector& other);
	~Vector();

	void Init(float ix = 0.0f, float iy = 0.0f, float iz = 0.0f);
	float* Base();
	const float* Base() const;

	float& operator[](const int i)
	{
		return reinterpret_cast<float*>(this)[i];
	}

	float operator[](const int i) const
	{
		return reinterpret_cast<const float*>(this)[i];
	}

	bool IsValid() const;
	bool IsZero() const;

	float Length() const;
	float Polar() const;
	Vector& Rotated(float angle);
	float LengthSqr() const;
	float DistanceTo(const Vector& other) const;
	float Dot(const Vector& other) const;
	float AngleBetween(const Vector& other) const;

	float NormalizeInPlace();
	Vector& Normalize();

	bool operator==(const Vector& v) const;
	bool operator!=(const Vector& v) const;

	Vector& SetZero();
	float Length2D() const;
	static Vector Zero();

	Vector operator-();
	Vector operator-() const;

	operator SDKVECTOR() const { return { this->x, this->y }; }

	void MulAdd(const Vector& a, const Vector& b, float scalar);

	// arithmetic operations
	Vector& operator+=(const Vector& v)
	{
		x += v.x; y += v.y; z += v.z;
		return *this;
	}

	Vector& operator-=(const Vector& v)
	{
		x -= v.x; y -= v.y; z -= v.z;
		return *this;
	}

	Vector& operator*=(float fl)
	{
		x *= fl;
		y *= fl;
		z *= fl;
		return *this;
	}

	Vector& operator*=(const Vector& v)
	{
		x *= v.x;
		y *= v.y;
		z *= v.z;
		return *this;
	}

	Vector& operator/=(const Vector& v)
	{
		x /= v.x;
		y /= v.y;
		z /= v.z;
		return *this;
	}

	// this ought to be an opcode.
	Vector&	operator+=(float fl)
	{
		x += fl;
		y += fl;
		z += fl;
		return *this;
	}

	// this ought to be an opcode.
	Vector&	operator/=(float fl)
	{
		x /= fl;
		y /= fl;
		z /= fl;
		return *this;
	}
	Vector&	operator-=(float fl)
	{
		x -= fl;
		y -= fl;
		z -= fl;
		return *this;
	}

	Vector& clamp();

	Vector operator+(const Vector& v) const;
	Vector operator-(const Vector& v) const;
	Vector operator*(const Vector& v) const;
	Vector operator/(const Vector& v) const;
	Vector operator*(float fl) const;
	Vector operator/(float fl) const;
};

class __declspec(align(16)) VectorAligned : public Vector
{
public:
	VectorAligned() : w(0)
	{
	}
	VectorAligned(float X, float Y, float Z)
	{
		Init(X, Y, Z);
	}

	explicit VectorAligned(const Vector& vOther)
	{
		Init(vOther.x, vOther.y, vOther.z);
	}

	VectorAligned& operator=(const Vector& vOther)
	{
		Init(vOther.x, vOther.y, vOther.z);
		return *this;
	}

	VectorAligned& operator=(const VectorAligned& vOther)
	{
		_mm_store_ps(Base(), _mm_load_ps(vOther.Base()));
		return *this;
	}

	float w;
};

void AngleVectors(const Vector& angles, Vector* forward);
void AngleVectors(const Vector& angles, Vector* forward, Vector* right, Vector* up);
void VectorAngles(const Vector& forward, Vector& out);
void VectorCopy(const Vector& src, Vector& dst);
float fast_sqrt(float x);