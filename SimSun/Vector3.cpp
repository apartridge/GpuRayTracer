#include "Vector3.h"
#include <cmath>
#include <pmmintrin.h>
#include <QDebug>

Vector3::Vector3( double x, double y, double z )
{
	this->x = x;
	this->y = y;
	this->z = z;
}

Vector3::Vector3( double a )
{
	this->x = a;
	this->y = a;
	this->z = a;
}


double Vector3::length() const
{
	return sqrt(this->x*this->x + this->y*this->y  + this->z*this->z);
}

Vector3 Vector3::normalized() const
{
	Vector3 oth = *this;
	oth.normalize();
	return oth;
}

void Vector3::normalize()
{
	double len = 1/this->length();
	this->x *= len;
	this->y *= len;
	this->z *= len;
}

Vector3 Vector3::operator + (const Vector3& addend) const
{
	return Vector3(this->x + addend.x, this->y + addend.y, this->z + addend.z);
} 

Vector3 Vector3::operator - (const Vector3& subtrahend) const
{
	return Vector3(this->x - subtrahend.x, this->y - subtrahend.y, this->z - subtrahend.z);
}

Vector3 Vector3::operator - () const
{
	return Vector3(-this->x, -this->y, -this->z);
}

Vector3 Vector3::operator * ( double scale ) const
{
	Vector3 result = *this;
	result.x *= scale;
	result.y *= scale;
	result.z *= scale;
	return result;
}

Vector3 operator * ( double scale, const Vector3 & vector )
{
	return vector*scale;
}

void Vector3::operator *= (double scalar)
{
	this->x *= scalar;
	this->y *= scalar;
	this->z *= scalar;
} 

void Vector3::operator+=( const Vector3& addend )
{
	this->x += addend.x;
	this->y += addend.y;
	this->z += addend.z;
}

void Vector3::operator-=( const Vector3& subtrahend )
{
	this->x -= subtrahend.x;
	this->y -= subtrahend.y;
	this->z -= subtrahend.z;
}

void Vector3::operator+=( double addend)
{
	this->x += addend;
	this->y += addend;
	this->z += addend;
}

void Vector3::operator-=( double subtrahend)
{
	this->x -= subtrahend;
	this->y -= subtrahend;
	this->z -= subtrahend;
}

Vector3 Vector3::cross(const Vector3 & a, const Vector3 & b)
{
	return Vector3(a.y * b.z - a.z * b.y,
		a.z * b.x - a.x * b.z,
		a.x * b.y - a.y * b.x);
}

double Vector3::dot(const Vector3 & a, const Vector3 & b)
{
	/*__m128d a1;
	__m128d a2;

	__m128d b1;
	__m128d b2;


	__m128d r1;
	__m128d r2;

	a1 = _mm_set_pd(a.x, a.y);
	b1 = _mm_set_pd(b.x, b.y);

	r1 = _mm_mul_pd(a1, b1);

	a2 = _mm_set_pd(a.z, 0);
	b2 = _mm_set_pd(b.z, 0);

	r2 = _mm_mul_pd(a2, b2);

	__m128d res;
	res = _mm_hadd_pd(r1, r2);

	double result[2];
	_mm_store_pd(result, res);

	return result[0] + result[1];*/

	return a.x * b.x + a.y * b.y + a.z * b.z;
}

/*
 Reflect function
 Reflect this direction along normal, returns reflected vector
*/

Vector3 Vector3::reflect(const Vector3 & normal) const
{
	Vector3 result;
	double dotP = Vector3::dot(*this, normal);
	result.x = this->x - 2 * normal.x * dotP;
	result.y = this->y - 2 * normal.y * dotP;
	result.z = this->z - 2 * normal.z * dotP;
	return result;
}

void Vector3::debug( const char* a ) const
{
	qDebug() << a << ": [" << this->x << ", " << this->y << ", " << this->z << "]";
}

Axis Vector3::getLargestAxis() const
{
	if(abs(this->x) > abs(this->y) && abs(this->x) > abs(this->z))
	{
		return X;
	}
	else if(abs(this->y) > abs(this->z))
	{
		return Y;
	}
	return Z;
}

double Vector3::getField( Axis a ) const
{
	return *(double*)((size_t)this + (size_t)a*sizeof(double));
}

void Vector3::setField( Axis axis, double v )
{
	switch(axis)
	{
	case X:
		this->x = v;
		return;
	case Y:
		this->y = v;
		return;
	}
	this->z = v;
}
