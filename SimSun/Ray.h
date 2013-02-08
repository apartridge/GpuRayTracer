
#ifndef RAY_H
#define RAY_H

#ifdef __CUDACC__
#define NVCC_COMPILER_FLAGS_HOSTDEVICE __host__ __device__
#else
#define NVCC_COMPILER_FLAGS_HOSTDEVICE
#endif

#include "Vector3.h"
#include "Configuration.h"

class Ray
{
public:
    NVCC_COMPILER_FLAGS_HOSTDEVICE Ray()
	{

	}
    Ray(Vector3 pt, Vector3 dir);
    const Vector3 & getOrigin() const;
    const Vector3 & getDirection() const;
	//const Vector3& getDirectionInverse() const;
/*#ifdef USE_GPU_ACCELLERATION
	double tNear;
	double tFar;
#endif
	*/
//private:
    Vector3 origin;
    Vector3 direction;
	//Vector3 directionInverse;
};

#endif // RAY_H
