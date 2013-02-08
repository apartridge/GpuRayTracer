#include "Triangle.h"
#include "Ray.h"
#include "RayHitInfo.h"


Triangle::Triangle()
{

}

Triangle::Triangle( Vector3 p1, Vector3 p2, Vector3 p3 )
{
	this->p1 = p1;
	this->p2 = p2;
	this->p3 = p3;
	
	Vector3 b = p3-p1;
	Vector3 c = p2-p1;
	Vector3 normal = Vector3::cross(b, c);
	
	nAxis = normal.getLargestAxis();
	uAxis = (Axis)(((int)nAxis + 1) % 3 );
	vAxis = (Axis)(((int)nAxis + 2) % 3 );

	nu = normal.getField(uAxis) / normal.getField(nAxis);
	nv = normal.getField(vAxis) / normal.getField(nAxis);
	nd = Vector3::dot(normal, this->p1 ) / normal.getField(nAxis);

	double bu = b.getField(uAxis);
	double bv = b.getField(vAxis);

	double cu = c.getField(uAxis);
	double cv = c.getField(vAxis);

	double denom = 1.0/(bu * cv - bv * cu);

	bun = bu *denom;
	bvn = -bv*denom;

	cun = cv*denom;
	cvn = -cu*denom; 

	this->normal_normalized = normal.normalized();

}

/*
If the ray and this triangle intersects, 
return true and update hitInfo struct with the value t of
origin+t*direction with the location of the intersection
*/

bool Triangle::intersectedByRay(const Ray& ray, RayHitInfo& hitInfo, double tMin, double tMax) const
{

	double d = 1.0/(ray.getDirection().getField(nAxis) + nu * ray.getDirection().getField(uAxis) + nv * ray.getDirection().getField(vAxis));
	double t = (nd - (ray.getOrigin().getField(nAxis) + nu * ray.getOrigin().getField(uAxis) + nv * ray.getOrigin().getField(vAxis)) ) * d;
	
	if(t >= tMin && t <= tMax)
	{
		Vector3 P = ray.getOrigin() + t*ray.getDirection() - this->p1;

		double Pu = P.getField(uAxis);
		double Pv = P.getField(vAxis);

		double beta = Pv * bun + Pu * bvn;
		if(beta < 0)
		{
			return false;
		}

		double gamma = Pu * cun + Pv * cvn;
		if(gamma < 0 || gamma + beta > 1)
		{
			return false;
		}

		hitInfo.t = t;
		hitInfo.normal = normal_normalized;
		return true;
	}

	return false;
}
