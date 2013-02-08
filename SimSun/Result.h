
#ifndef RESULT_H
#define RESULT_H


#ifdef __CUDACC__
#define NVCC_COMPILER_FLAGS_HOSTDEVICE __host__ __device__
#else
#define NVCC_COMPILER_FLAGS_HOSTDEVICE
#endif

#include "Vector3.h"
#include "Ray.h"

class Panel;

class Result
{
public:
	Result()
	{

	}

	NVCC_COMPILER_FLAGS_HOSTDEVICE Result(const Ray & ray, double t/*, Panel* panel*/)
	{
		this->ray = ray;
		this->t = t;
		//this->panel = panel;
	}

	Vector3 startPoint()
	{
		return this->ray.getOrigin();
	}

	Vector3 endPoint()
	{
		return this->ray.getOrigin() + this->ray.getDirection() * this->t;
	}

	Ray ray;
	double t;

};

#endif
