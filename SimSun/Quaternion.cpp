#include "Quaternion.h"

Quaternion::Quaternion()
{
	this->r = 1;
	this->i = 0;
	this->j = 0;
	this->k = 0;
}

Quaternion Quaternion::betweenVectors( Vector3 a, Vector3 b )
{
	Vector3 axis = Vector3::cross(a, b).normalized();
	Quaternion q;
	double angle = acos(Vector3::dot(a, b));
	q.i = axis.x * sin(angle/2);
	q.j = axis.y * sin(angle/2);
	q.k = axis.z * sin(angle/2);
	q.r = cos(angle/2);
	return q;
}

void Quaternion::operator*=( const Quaternion & other )
{
	(*this) = (*this) * other;
}

Quaternion Quaternion::operator*( const Quaternion & other ) const
{
	Quaternion product;

	product.r = (this->r*other.r - this->i*other.i - this->j*other.j - this->k*other.k);
	product.i = (this->r*other.i + this->i*other.r + this->j*other.k - this->k*other.j);
	product.j = (this->r*other.j - this->i*other.k + this->j*other.r + this->k*other.i);
	product.k = (this->r*other.k + this->i*other.j - this->j*other.i + this->k*other.r);

	return product;
}


void Quaternion::debug( const char* a ) const
{
	qDebug() << a << ": [" << this->r << ", " << this->i << ", " << this->j << ", " <<  this->k << "]";
}

void Quaternion::toAxisAngleRotation( Vector3& axis, double& angle ) const
{
	angle = 2*acos(this->r);
	double scale = 1.0/sin(angle/2);
	axis.x = scale*this->i;
	axis.y = scale*this->j;
	axis.z = scale*this->k;
}
