
#include "Panel.h"
#include "SurfaceObject.h"
#include "Misc.h"
#include "Ray.h"
#include "RayHitInfo.h"
#include "OpenGL.h"

Panel::Panel(SurfaceObject* parent, Quadrangle quad)
{
	this->parent = parent;
	this->absorbtion = parent->absorbtion;
	tri1 = Triangle(quad.p1, quad.p2, quad.p3);
	tri2 = Triangle(quad.p1, quad.p3, quad.p4);
	boundingBox = BoundingBox(quad);
}

Panel::~Panel()
{

}

void Panel::drawPanel() const
{
	glBegin(GL_LINES);
	glColor3f(0,0,1);

	glVertexV3(tri1.p1);
	glVertexV3(tri1.p2);

	glVertexV3(tri1.p2);
	glVertexV3(tri1.p3);

	glVertexV3(tri1.p3);
	glVertexV3(tri2.p3);

	glVertexV3(tri2.p3);
	glVertexV3(tri1.p1);

	glEnd();

}

void Panel::drawBoundingBox() const 
{
	this->getBoundingBox().drawBoundingBox();
}

bool Panel::findIntersectionPoint(const Ray & ray, RayHitInfo& hitInfo, double tMin, double tMax) const
{
	if(tri1.intersectedByRay(ray, hitInfo, tMin, tMax))
	{
		return true;
	}

	if(tri2.intersectedByRay(ray, hitInfo, tMin, tMax))
	{
		return true;
	}

	return false;
}

void Panel::incrementIntensity(double sunIntensity)
{
	this->hits++;
	this->intensity += sunIntensity;
	this->parent->incrementIntensity(sunIntensity);
}

void Panel::resetIntensity()
{
	this->hits = 0;
	this->intensity = 0;
}

const BoundingBox & Panel::getBoundingBox() const
{
	return boundingBox;
}

