#pragma once

#include "Vector3.h"
#include <QDebug>

class Quaternion
{
public:
	Quaternion();
	Quaternion(double v);
	double r, i, j, k;
	static Quaternion betweenVectors( Vector3 previousRotation, Vector3 current);

	Quaternion operator * (const Quaternion & other) const;
	void operator *= (const Quaternion & other);
	void debug( const char* a ) const;
	void toAxisAngleRotation( Vector3& axis, double& angle ) const;
};