#include "BoundingBox.h"
#include "OpenGL.h"
#include "Quadrangle.h"
#include "Ray.h"

void BoundingBox::drawBoundingBox() const
{
	glBegin(GL_LINES);

	// Bottom

	glVertex3f(mi.x, mi.y, mi.z);
	glVertex3f(ma.x, mi.y, mi.z);

	glVertex3f(ma.x, mi.y, mi.z);
	glVertex3f(ma.x, ma.y, mi.z);

	glVertex3f(ma.x, ma.y, mi.z);
	glVertex3f(mi.x, ma.y, mi.z);

	glVertex3f(mi.x, ma.y, mi.z);
	glVertex3f(mi.x, mi.y, mi.z);

	// Sides

	glVertex3f(mi.x, mi.y, mi.z);
	glVertex3f(mi.x, mi.y, ma.z);

	glVertex3f(mi.x, ma.y, mi.z);
	glVertex3f(mi.x, ma.y, ma.z);

	glVertex3f(ma.x, ma.y, mi.z);
	glVertex3f(ma.x, ma.y, ma.z);

	glVertex3f(ma.x, mi.y, mi.z);
	glVertex3f(ma.x, mi.y, ma.z);

	// Top

	glVertex3f(mi.x, mi.y, ma.z);
	glVertex3f(ma.x, mi.y, ma.z);

	glVertex3f(ma.x, mi.y, ma.z);
	glVertex3f(ma.x, ma.y, ma.z);

	glVertex3f(ma.x, ma.y, ma.z);
	glVertex3f(mi.x, ma.y, ma.z);

	glVertex3f(mi.x, ma.y, ma.z);
	glVertex3f(mi.x, mi.y, ma.z);

	glEnd();

}

BoundingBox::BoundingBox( const Quadrangle & quad )
{
	// Initialize bounding box from a quad

	double minx = min(min(quad.p1.x, quad.p2.x), min(quad.p3.x, quad.p4.x));
	double miny = min(min(quad.p1.y, quad.p2.y), min(quad.p3.y, quad.p4.y));
	double minz = min(min(quad.p1.z, quad.p2.z), min(quad.p3.z, quad.p4.z));

	double maxx = max(max(quad.p1.x, quad.p2.x), max(quad.p3.x, quad.p4.x));
	double maxy = max(max(quad.p1.y, quad.p2.y), max(quad.p3.y, quad.p4.y));
	double maxz = max(max(quad.p1.z, quad.p2.z), max(quad.p3.z, quad.p4.z));

	this->mi = Vector3(minx, miny, minz);
	//this->mi -= BOUNDING_BOX_PADDING;
	this->ma = Vector3(maxx, maxy, maxz);
	//this->ma += BOUNDING_BOX_PADDING;
}

bool BoundingBox::traceRay( const Ray & ray, BoundingBoxTNearFarPair & pair ) const
{
	// X

	double divx = 1 / ray.getDirection().x;
	double tNear, tFar;
	if (divx >= 0)
	{
		tNear = (this->mi.x- ray.getOrigin().x) * divx;
		tFar = (this->ma.x - ray.getOrigin().x) * divx;
	}
	else
	{
		tNear = (this->ma.x - ray.getOrigin().x) * divx;
		tFar = (this->mi.x - ray.getOrigin().x) * divx;
	}

	if(tFar < tNear )
	{
		return false;
	}

	// Y

	double divy = 1 / ray.getDirection().y;
	double tyNear, tyFar;
	if (divy >= 0)
	{
		tyNear = (this->mi.y- ray.getOrigin().y) * divy;
		tyFar = (this->ma.y - ray.getOrigin().y) * divy;
	}
	else {
		tyNear = (this->ma.y - ray.getOrigin().y) * divy;
		tyFar = (this->mi.y - ray.getOrigin().y) * divy;
	}

	if(tyFar < tyNear )
	{
		return false;
	}

	if(tyNear > tNear)
	{
		tNear = tyNear;
	}

	if(tyFar < tFar)
	{
		tFar = tyFar;
	}

	// Z

	double divz = 1 / ray.getDirection().z;
	double tzNear, tzFar;
	if (divz >= 0)
	{
		tzNear = (this->mi.z- ray.getOrigin().z) * divz;
		tzFar = (this->ma.z - ray.getOrigin().z) * divz;
	}
	else {
		tzNear = (this->ma.z - ray.getOrigin().z) * divz;
		tzFar = (this->mi.z - ray.getOrigin().z) * divz;
	}

	if(tzFar < tzNear )
	{
		return false;
	}

	if(tzNear > tNear)
	{
		tNear = tzNear;
	}

	if(tzFar < tFar)
	{
		tFar = tzFar;
	}

	if(tNear > 1E200 || tFar < 0 )
	{
		return false;
	}
	
	pair.tNear = tNear;
	pair.tFar = tFar;

	return true;
}

double BoundingBox::surfaceArea() const
{
	return 2*(ma.x - mi.x) + 2*(ma.y - mi.y) + 2*(ma.z - mi.z); 
}

