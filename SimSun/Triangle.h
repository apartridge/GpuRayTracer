#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "Vector3.h"

class Ray;
class RayHitInfo;

class Triangle
{
public:
	Triangle();
	Triangle(Vector3 p1, Vector3 p2, Vector3 p3);
	bool intersectedByRay(const Ray& ray, RayHitInfo& hitInfo, double tMin, double tMax) const;
	Vector3 p1, p2, p3;

	//bool hasNormal;
	Vector3 normal_normalized;
	/*Vector3 b;
	Vector3 c;*/

	Axis nAxis;
	Axis uAxis;
	Axis vAxis;
	
	double nu;
	double nv;
	double nd;

	double bun;
	double bvn;

	double cun;
	double cvn;

};

#endif