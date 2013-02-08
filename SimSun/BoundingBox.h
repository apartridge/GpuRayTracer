#ifndef BOUNDING_BOX_H
#define BOUNDING_BOX_H

#include "Vector3.h"
class Quadrangle;
class Ray;

#define BOUNDING_BOX_PADDING 0.00000001

class BoundingBoxTNearFarPair
{
public:
	double tNear;
	double tFar;
};

class BoundingBox
{
public:
	BoundingBox() : mi(Vector3(0)), ma(Vector3(0)) {}
	BoundingBox(Vector3 _min, Vector3 _max ) : mi(_min), ma(_max) {};
	BoundingBox(const Quadrangle & quad);
	double surfaceArea() const;
	void drawBoundingBox() const; 
	bool traceRay(const Ray & ray, BoundingBoxTNearFarPair & pair ) const;

	const Vector3 & getMin() const { return mi; }
	const Vector3 & getMax() const { return ma; }
	void setMin(const Vector3 & min){this->mi = min;}
	void setMax(const Vector3 & max){this->ma = max;}
//private:
	Vector3 mi;
	Vector3 ma;
};

#endif