#ifndef RAY_HIT_INFO_H
#define RAY_HIT_INFO_H

class Panel;

/*
t is the hit position, by ray.origin+t*ray.direction
normal should always be normalized
*/

class RayHitInfo
{
public:
	RayHitInfo()
	{
		t = 1E20;
		hitPanel = 0;
	}
	double t;
	Vector3 normal;
	Panel* hitPanel;
};

#endif