
#ifndef PANEL_H
#define PANEL_H

#include "Triangle.h"
#include "Quadrangle.h"
#include "BoundingBox.h"

#define PI 3.1415926535

class Ray;
class RayHitInfo;
class SurfaceObject;

class Panel
{
public:
	Panel(SurfaceObject* parent, Quadrangle fP);
    ~Panel();

    void drawPanel() const;
	void drawBoundingBox() const;
	bool findIntersectionPoint(const Ray & ray, RayHitInfo& hitInfo, double tMin, double tMax) const;
	void incrementIntensity(double);
	void resetIntensity();
	const BoundingBox &  getBoundingBox() const;
	const Triangle & getTriangle1() const { return tri1; };
	const Triangle & getTriangle2() const { return tri2; };
	SurfaceObject & getParent() const { return *parent; }
	
	double getAbsorbtion()
	{
		return absorbtion;	
	}

	int hits;
private:

	Triangle  tri1;
	Triangle  tri2;
	BoundingBox boundingBox;

	double intensity;
	SurfaceObject* parent;
	float absorbtion;

};

#endif

