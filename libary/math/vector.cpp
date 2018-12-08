#include "vector.hpp"

#include <algorithm>
#include <cmath>
#include <float.h>
#include "geometry.hpp"


Vector::Vector()
{
	this->x = 0.f;
	this->y = 0.f;
	this->z = 0.f;
}

Vector::Vector(float x, float y, float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

Vector::Vector(float* val)
{
	this->x = val[0];
	this->y = val[1];
	this->z = val[2];
}

Vector::Vector(const float* val)
{
	this->x = val[0];
	this->y = val[1];
	this->z = val[2];
}

Vector::Vector(int x, int y, int z)
{
	this->x = float(x);
	this->y = float(y);
	this->z = float(z);
}

Vector::Vector(const Vector& other)
{
	this->x = other.x;
	this->y = other.y;
	this->z = other.z;
}

Vector::~Vector()
{
	this->x = 0.f;
	this->y = 0.f;
	this->z = 0.f;
}

void Vector::Init(float ix, float iy, float iz)
{
	x = ix; y = iy; z = iz;
}

float* Vector::Base()
{
	return reinterpret_cast<float*>(this);
}

const float* Vector::Base() const
{
	return reinterpret_cast<const float*>(this);
}

bool Vector::IsValid() const
{
	if (std::isinf(this->x))
		return false;
	if (std::isinf(this->y))
		return false;
	if (std::isnan(this->x))
		return false;
	if (std::isnan(this->y))
		return false;

	return true;
}

bool Vector::IsZero() const
{
	return this->x == 0.f && this->y == 0.f && this->z == 0.f;
}

Vector& Vector::SetZero()
{
	this->x = 0.f;
	this->y = 0.f;
	this->z = 0.f;
	return *this;
}

float Vector::Length2D() const
{
	return fast_sqrt(x * x + y * y);
}

Vector Vector::Zero()
{
	return Vector(0, 0, 0);
}


Vector Vector::operator-()
{
	return Vector(-x, -y, -z);
}

Vector Vector::operator-(void) const
{
	return Vector(-x, -y, -z);
}

void Vector::MulAdd(const Vector& a, const Vector& b, float scalar)
{
	x = a.x + b.x * scalar;
	y = a.y + b.y * scalar;
	z = a.z + b.z * scalar;
}

Vector& Vector::clamp()
{
	x = std::clamp(std::remainderf(x, 180.f), -89.f, 89.f);
	y = std::clamp(std::remainderf(y, 360.f), -180.f, 180.f);
	z = 0.f;

	return *this;
}

Vector Vector::operator+(const Vector& v) const
{
	return Vector(x + v.x, y + v.y, z + v.z);
}

Vector Vector::operator-(const Vector& v) const
{
	return Vector(x - v.x, y - v.y, z - v.z);
}

Vector Vector::operator*(float fl) const
{
	return Vector(x * fl, y * fl, z * fl);
}

Vector Vector::operator*(const Vector& v) const
{
	return Vector(x * v.x, y * v.y, z * v.z);
}

Vector Vector::operator/(float fl) const
{
	return Vector(x / fl, y / fl, z / fl);
}

Vector Vector::operator/(const Vector& v) const
{
	return Vector(x / v.x, y / v.y, z / v.z);
}

Vector operator*(float fl, const Vector& v)
{
	return v * fl;
}

float Vector::Length() const
{
	return fast_sqrt(this->x*this->x + this->y*this->y + this->z*this->z);
}

float Vector::Polar() const
{
	if (geometry->close(this->x, 0, 0))
	{
		if (this->y > 0)
		{
			return 90.f;
		}
		return this->y < 0 ? 270.f : 0.f;
	}

	auto temp = atan(this->y / this->x);
	auto theta = (temp * (180 / M_PI));
	if (x < 0)
	{
		theta = theta + 180;
	}
	if(theta < 0)
	{
		theta = theta + 360;
	}
	return theta;
}

Vector& Vector::Rotated(float angle)
{
	auto c = cos(angle);
	auto s = sin(angle);

	this->x = x * c - y * s;
	this->y = y * c + x * s;

	return *this;
}

float Vector::LengthSqr() const
{
	return float(std::pow(this->x, 2) + std::pow(this->y, 2) + std::pow(this->z, 2));
}

float Vector::DistanceTo(const Vector& other) const
{
	return (*this - other).Length();
}

float Vector::Dot(const Vector& other) const
{
	return float(this->x * other.x + this->y * other.y + this->z * other.z);
}

float Vector::AngleBetween(const Vector& other) const
{
	auto theta = this->Polar() - other.Polar();
	if (theta < 0)
	{
		theta = theta + 360;
	}
	if (theta > 180)
	{
		theta = 360 - theta;
	}
	return theta;
}

float Vector::NormalizeInPlace()
{
	const auto l = this->Length();
	if (l != 0.0f)
	{
		*this /= l;
	}
	else
	{
		this->x = this->y = this->z = 0.0f;
	}
	return l;
}

Vector& Vector::Normalize()
{
	auto flLen = this->Length();
	if (flLen == 0)
	{
		this->SetZero();
		return *this;
	}
	flLen = 1 / flLen;
	this->x *= flLen;
	this->y *= flLen;
	this->z *= flLen;

	return *this;
}

bool Vector::operator==(const Vector& v) const
{
	return this->x == v.x && this->y == v.y && this->z == v.z;
}

bool Vector::operator!=(const Vector& v) const
{
	return this->x != v.x || this->y != v.y || this->z == v.z;
}

inline void VectorAdd(const Vector& a, const Vector& b, Vector& c)
{
	c.x = a.x + b.x;
	c.y = a.y + b.y;
	c.z = a.z + b.z;
}

inline void VectorSubtract(const Vector& a, const Vector& b, Vector& c)
{
	c.x = a.x - b.x;
	c.y = a.y - b.y;
	c.z = a.z - b.z;
}

inline void VectorMultiply(const Vector& a, float b, Vector& c)
{
	c.x = a.x * b;
	c.y = a.y * b;
	c.z = a.z * b;
}

inline void VectorMultiply(const Vector& a, const Vector& b, Vector& c)
{
	c.x = a.x * b.x;
	c.y = a.y * b.y;
	c.z = a.z * b.z;
}

inline void VectorDivide(const Vector& a, float b, Vector& c)
{
	auto oob = 1.0f / b;
	c.x = a.x * oob;
	c.y = a.y * oob;
	c.z = a.z * oob;
}

inline void VectorDivide(const Vector& a, const Vector& b, Vector& c)
{
	c.x = a.x / b.x;
	c.y = a.y / b.y;
	c.z = a.z / b.z;
}

void AngleVectors(const Vector& angles, Vector* forward)
{
	float sp, sy, cp, cy; /* sinus pitch, sinus yaw, cosinus pitch, cosinus yaw */

	auto SinCos = [](float radians, float* sine, float* cosine)
	{
		*sine = sin(radians);
		*cosine = cos(radians);
	};

	SinCos(DEG2RAD(angles.x), &sp, &cp);
	SinCos(DEG2RAD(angles.y), &sy, &cy);

	forward->x = cp * cy;
	forward->y = cp * sy;
	forward->z = -sp;
}

void AngleVectors(const Vector& angles, Vector* forward, Vector* right, Vector* up)
{
	float sr, sp, sy, cr, cp, cy;

	auto SinCos = [](float radians, float* sine, float* cosine)
	{
		*sine = sin(radians);
		*cosine = cos(radians);
	};

	SinCos(DEG2RAD(angles.y), &sy, &cy);
	SinCos(DEG2RAD(angles.x), &sp, &cp);
	SinCos(DEG2RAD(angles.z), &sr, &cr);

	if (forward)
	{
		forward->x = cp * cy;
		forward->y = cp * sy;
		forward->z = -sp;
	}

	if (right)
	{
		right->x = -1 * sr * sp * cy + -1 * cr * -sy;
		right->y = -1 * sr * sp * sy + -1 * cr * cy;
		right->z = -1 * sr * cp;
	}

	if (up)
	{
		up->x = cr * sp * cy + -sr * -sy;
		up->y = cr * sp * sy + -sr * cy;
		up->z = cr * cp;
	}
}


void VectorAngles(const Vector& forward, Vector& out)
{
	float yaw, pitch;

	if (forward[1] == 0 && forward[0] == 0)
	{
		yaw = 0;
		if (forward[2] > 0)
			pitch = 270;
		else
			pitch = 90;
	}
	else
	{
		yaw = (atan2(forward[1], forward[0]) * M_RADPI);
		if (yaw < 0)
			yaw += 360;

		auto tmp = fast_sqrt(forward[0] * forward[0] + forward[1] * forward[1]);
		pitch = (atan2(-forward[2], tmp) * M_RADPI);
		if (pitch < 0)
			pitch += 360;
	}

	out.x = pitch;
	out.y = yaw;
	out.z = 0;
}

void VectorCopy(const Vector & src, Vector & dst)
{
	dst.x = src.x;
	dst.y = src.y;
	dst.z = src.z;
}

float fast_sqrt(float x)
{
	if (x == 0.f)
		return 0.f;
	const auto _x = _mm_set_ss(x);
	return _mm_cvtss_f32(_mm_mul_ss(_x, _mm_rsqrt_ss(_x)));
}
