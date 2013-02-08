#ifndef VECTOR3_H
#define VECTOR3_H

#include "Axis.h"
/*
Represents a 3D Point together with vector operators
and utilary overloaded operators in C++, like vector addition, scaling
and dot products
*/

#ifdef __CUDACC__
#define NVCC_COMPILER_FLAGS_HOSTDEVICE __host__ __device__
#else
#define NVCC_COMPILER_FLAGS_HOSTDEVICE
#endif

class Vector3
{
public:
	NVCC_COMPILER_FLAGS_HOSTDEVICE Vector3(){}
	Vector3(double);
	Vector3(double, double, double);

	double length() const;
	void normalize();

	Vector3 normalized() const;
	Vector3 operator + (const Vector3& addend) const;
	Vector3 operator - (const Vector3& subtrahend) const;
	Vector3 operator - () const;
	Vector3 operator * (double scale) const;
	void operator *= (double scalar);
	void operator += (const Vector3& addend);
	void operator -= (const Vector3& subtrahend);
	void operator += (double addend);
	void operator -= (double subtrahend);
	void debug(const char* a) const;
	double getField(Axis a) const;

	Vector3 reflect(const Vector3 & normal) const;

	static Vector3 cross(const Vector3 & a, const Vector3 & b);
	static double dot(const Vector3 & a, const Vector3 & b);
	//static Vector3 eulerRotationBetween(const Vector3 & a, const Vector3 & b);
	Axis getLargestAxis() const;
	void setField( Axis axis, double splitPosition );

	/*const double & x() const {return xy[0];}
	const double & y() const {return xy[1];}
	const double & z() const {return zw[0];}*/
	double x, y, z;
	//union { struct {double x; double y; double z; }; double xyz[3]; };
};

Vector3 operator * (double scale, const Vector3 & vector);

#endif