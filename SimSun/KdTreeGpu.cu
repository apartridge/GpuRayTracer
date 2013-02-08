#include "Configuration.h"

#include "BoundingBox.h"
#include <cuda.h>
#include <cuda_runtime.h>
#include <device_launch_parameters.h>
#include "Vector3.h"
#include "Ray.h"
#include "BenchmarkTimer.h"
#include "KdTreeGpu.h"
#include "Result.h"

extern BenchmarkTimer gpuTotalTime;
extern BenchmarkTimer gpuTransferTime;

void CUDA_VALIDATE(cudaError_t e)
{
	if(e != cudaSuccess)
	{
		fprintf(stderr, "Cuda Error: %d\n Exiting.", e);
		exit(e);
	}
}

__device__ double getField(const Vector3 & vector, Axis a)
{
	switch(a)
	{
	case X:
		return vector.x;
	case Y:
		return vector.y;
	}
	return vector.z;
}

/*
Returns the position of the ray o+td at point t, moved just sligtly in the direction of reflectedDirection 
to avoid to hit the same location again
*/

__device__ Vector3 rayPosition(const Vector3 & rayOrigin,  const Vector3 & rayDirection, const double t, const Vector3 & reflectedDirection)
{
	Vector3 a = rayOrigin;
	a.x += t*rayDirection.x + 0.00001*reflectedDirection.x;
	a.y += t*rayDirection.y + 0.00001*reflectedDirection.y;
	a.z += t*rayDirection.z + 0.00001*reflectedDirection.z;
	return a;
}

/*
 Reflect function
 Reflect this direction along normal, returns reflected vector
*/

__device__ double vectorDot(const Vector3& a, const Vector3& b)
{
	return a.x*b.x + a.y*b.y + a.z*b.z;
}

__device__ Vector3 reflectVector(const Vector3& direction, const Vector3 & normal)
{
	Vector3 result = direction;
	double dotP = vectorDot(direction, normal);
	result.x = direction.x - 2 * normal.x * dotP;
	result.y = direction.y - 2 * normal.y * dotP;
	result.z = direction.z - 2 * normal.z * dotP;
	return result;
}

/*
Find triangle intersections
*/

__device__ Vector3 rtIntersectionfindPoint(const Vector3 & a, const Vector3 & b, const double bScale, const Vector3 & c)
{
	Vector3 out = a;
	out.x = a.x + b.x*bScale - c.x;
	out.y = a.y + b.y*bScale - c.y;
	out.z = a.z + b.z*bScale - c.z;
	return out;
}

__device__ bool rayTriangleIntersection(const Vector3 & rayOrigin, const Vector3 & rayDirection, double tNear, double tFar,
										const TrianglePanelPair & triangle, double & tOut, Vector3 & reflection)
{

	Triangle tri = triangle.tri;

	double d = 1.0/(getField(rayDirection, tri.nAxis) + tri.nu * getField(rayDirection, tri.uAxis) + tri.nv * getField(rayDirection, tri.vAxis));
	double t = (tri.nd - (getField(rayOrigin, tri.nAxis) + tri.nu * getField(rayOrigin, tri.uAxis) + tri.nv * getField(rayOrigin, tri.vAxis))) * d;
	
	if(t >= tNear && t <= tFar)
	{
		Vector3 P = rtIntersectionfindPoint(rayOrigin, rayDirection, t, tri.p1);

		double Pu = getField(P, tri.uAxis);
		double Pv = getField(P, tri.vAxis);

		double beta = Pv * tri.bun + Pu * tri.bvn;
		if(beta < 0)
		{
			return false;
		}

		double gamma = Pu * tri.cun + Pv * tri.cvn;
		if(gamma < 0 || gamma + beta > 1)
		{
			return false;
		}

		tOut = t;
		reflection = reflectVector(rayDirection, tri.normal_normalized);
		return true;
	}

	
	return false;
}

__device__ bool rayTrianglesIntersection(const Vector3 & rayOrigin, const Vector3 & rayDirection, double & tNear, double & tFar,
										 TrianglePanelPair* triangles, int trianglesListFrom, int trianglesListTo,
										double & t, int & hitIndex, Vector3 & reflection)
{
	int triangleIndexCandidate = -1;
	double tMinCandidate = 1E999;
	Vector3 reflectionCandidate;

	for(int i = trianglesListFrom; i < trianglesListTo; i++)
	{
		double tempT;
		Vector3 tempReflection;

		if(rayTriangleIntersection(rayOrigin, rayDirection, tNear, tFar, triangles[i], tempT, tempReflection))
		{
			if(tempT < tMinCandidate)
			{
				triangleIndexCandidate = i;
				tMinCandidate = tempT;
				reflectionCandidate = tempReflection;
			}
		}
	}

	if(triangleIndexCandidate >= 0)
	{
		t = tMinCandidate;
		hitIndex = triangleIndexCandidate;
		reflection = reflectionCandidate;
		return true;
	}
	return false;
}


/*
Intersects a Bounding Box with a ray. If the ray intersects, then we know that tNear < tFar. If it does not
exist, then we make sure the opposite is true.
*/
__device__ double2 rayBoxIntersection( BoundingBox sceneBB, const Vector3 & rayOrigin, const Vector3 & rayDirection)
{

	double2 tNearFar;

	// X

	double divx = 1 / rayDirection.x;
	double tNear, tFar;
	if (divx >= 0)
	{
		tNear = (sceneBB.mi.x- rayOrigin.x) * divx;
		tFar = (sceneBB.ma.x - rayOrigin.x) * divx;
	}
	else
	{
		tNear = (sceneBB.ma.x - rayOrigin.x) * divx;
		tFar = (sceneBB.mi.x - rayOrigin.x) * divx;
	}

	if(tFar < tNear )
	{
		tNearFar.x = 1;
		tNearFar.y = 0;
		return tNearFar;
	}

	// Y

	double divy = 1 / rayDirection.y;
	double tyNear, tyFar;
	if (divy >= 0)
	{
		tyNear = (sceneBB.mi.y- rayOrigin.y) * divy;
		tyFar = (sceneBB.ma.y - rayOrigin.y) * divy;
	}
	else {
		tyNear = (sceneBB.ma.y - rayOrigin.y) * divy;
		tyFar = (sceneBB.mi.y - rayOrigin.y) * divy;
	}

	if(tyFar < tyNear )
	{
		tNearFar.x = 1;
		tNearFar.y = 0;
		return tNearFar;
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

	double divz = 1 / rayDirection.z;
	double tzNear, tzFar;
	if (divz >= 0)
	{
		tzNear = (sceneBB.mi.z- rayOrigin.z) * divz;
		tzFar = (sceneBB.ma.z - rayOrigin.z) * divz;
	}
	else {
		tzNear = (sceneBB.ma.z - rayOrigin.z) * divz;
		tzFar = (sceneBB.mi.z - rayOrigin.z) * divz;
	}

	if(tzFar < tzNear )
	{
		tNearFar.x = 1;
		tNearFar.y = 0;
		return tNearFar;
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
		tNearFar.x = 1;
		tNearFar.y = 0;
		return tNearFar;
	}
	
	tNearFar.x = tNear;
	tNearFar.y = tFar;
	return tNearFar;
}

/*
Entry Kernel
*/

__global__ void kdTreeTraversal(BoundingBox sceneBB, Ray* rays, KdTreeGpuNode* nodes, TrianglePanelPair* triangles, 
								 int firstRayIndex, int numRays, int rank, int rootNode, int* hitPanelOut, GpuResult* results)
{
	unsigned int index = blockIdx.x*blockDim.x + threadIdx.x;

	if(index < numRays)
	{
		if(rank > 0 && hitPanelOut[index] == -1)
		{
			return;
		}

		hitPanelOut[index] = -1;
		Ray myRay = rays[index];
		Vector3 rayOrigin = myRay.origin;
		Vector3 rayDirection = myRay.direction;

		double2 tNearFar = rayBoxIntersection(sceneBB, rayOrigin, rayDirection);
		double tNear = tNearFar.x;
		double tFar = tNearFar.y;
		const double globalTFar = tFar;
		double pushdownNode = rootNode;
		bool pushDown = true;

		if(tNear < tFar) // Ray intersects box
		{
			if(tNear < 0)
			{
				tNear = 0;
			}

			int currentNode = rootNode;

			while(true)
			{

				while(nodes[currentNode].left != -1)
				{

					double nodeS = nodes[currentNode].s;
					Axis nodeAxis = nodes[currentNode].axis;
					double rayOriginAxis = getField(rayOrigin, nodeAxis);
					double rayDirectionAxis = getField(rayDirection, nodeAxis);
					double tSplit = (nodeS - rayOriginAxis) / rayDirectionAxis;

					int nearNode = nodes[currentNode].left, farNode = nodes[currentNode].right;
					if(rayDirectionAxis < 0)
					{
						int temp = nearNode;
						nearNode = farNode;
						farNode = temp;
					}

					// Evaluate children nodes
					if (tSplit >= tFar) // Near
					{ 
						currentNode = nearNode;
					}
					else if (tSplit <= tNear) // Far
					{
						currentNode = farNode;
						
					}
					else // Near then Far
					{ 
						currentNode = nearNode;
						tFar = tSplit;
						pushDown = false;
					}

					if(pushDown)
					{
						pushdownNode = currentNode;
					}
				}

				// Check Triangles for Intersection

				int trianglesListFrom = nodes[currentNode].globalListFirst;
				int trianglesListSize = nodes[currentNode].globalListSize;

				double tClosest;
				int hitPanelIndex;
				Vector3 reflection;

				if(rayTrianglesIntersection(rayOrigin, rayDirection, tNear, tFar,
											triangles, trianglesListFrom, trianglesListFrom+trianglesListSize,
											tClosest, hitPanelIndex, reflection))
				{
					if(tClosest >= tNear && tClosest <= tFar ) 
					{	
						// Check that we have hit on the correct side of closest triangle
						if(vectorDot(triangles[hitPanelIndex].tri.normal_normalized, rayDirection) < 0)
						{
							// Return result hit
							Ray reflectedRay;
							reflectedRay.origin = rayPosition(rayOrigin, rayDirection, tClosest, reflection);
							reflectedRay.direction = reflection;

							hitPanelOut[index] = hitPanelIndex;

							results[index].reflectedRay = reflectedRay;
							results[index].t = tClosest;
							results[index].panel = triangles[hitPanelIndex].panel;
							rays[index] = reflectedRay;
						}

						return;
					}
				}

				// Continue Search

				if(tFar == globalTFar)
				{
					return;
				}
				else
				{
					currentNode = pushdownNode;
					tNear = tFar;
					tFar = globalTFar;
				}
			}
		}
	}
}

/*
 Start tracing of a RayList on the GPU. The result can later be fetched, so CPU work can be
 done in the mean time.
*/

void KdTreeGpu::traceRaysOnGpuAsync(int firstRayIndex, int numRays, int rank, int buffer)
{
	int per_block = 128;
	int num_blocks = numRays/per_block + (numRays%per_block==0?0:1);

	Ray* rays = &this->deviceRayPtr[firstRayIndex];
	int* deviceLocalHitPanelIds = &this->deviceHitPanelIdPtr[firstRayIndex];

	kdTreeTraversal<<<num_blocks, per_block, 0>>>(sceneBoundingBox, rays, deviceNodesPtr, deviceTrianglesListPtr, 
											 	firstRayIndex, numRays, rank, rootNodeIndex, 
												deviceLocalHitPanelIds, deviceResults);

	cudaStreamQuery(0);
	CUDA_VALIDATE(cudaMemcpyAsync(resultHitPanelIds[buffer], deviceLocalHitPanelIds, numRays*sizeof(int), cudaMemcpyDeviceToHost));
	CUDA_VALIDATE(cudaMemcpyAsync(results[buffer], deviceResults, numRays*sizeof(GpuResult), cudaMemcpyDeviceToHost));
	cudaStreamQuery(0);
}

void KdTreeGpu::transferRaysToGpu(RayList & rays)
{
	gpuTransferTime.start();
	CUDA_VALIDATE(cudaMalloc(&this->deviceRayPtr, rays.size()*sizeof(Ray)));
	CUDA_VALIDATE(cudaMalloc(&this->deviceHitPanelIdPtr, rays.size()*sizeof(int)));
	CUDA_VALIDATE(cudaMemcpyAsync(this->deviceRayPtr, &rays[0], rays.size()*sizeof(Ray), cudaMemcpyHostToDevice));
	gpuTransferTime.stop();
}

void KdTreeGpu::waitForGpu()
{
	CUDA_VALIDATE(cudaDeviceSynchronize());
}

/*
Allocating pinned memory on the host that can be accessed asynchronously by the GPU
*/

void KdTreeGpu::allocateHostResultBuffers()
{

	unsigned int flag = cudaHostAllocPortable;

	CUDA_VALIDATE(cudaHostAlloc(&resultHitPanelIds[0], MAX_RAYS_PER_ITERATION*sizeof(int), flag));
	CUDA_VALIDATE(cudaHostAlloc(&resultHitPanelIds[1], MAX_RAYS_PER_ITERATION*sizeof(int), flag));

	CUDA_VALIDATE(cudaHostAlloc(&results[0], MAX_RAYS_PER_ITERATION*sizeof(GpuResult), flag));
	CUDA_VALIDATE(cudaHostAlloc(&results[1], MAX_RAYS_PER_ITERATION*sizeof(GpuResult), flag));
}

void KdTreeGpu::freeHostResultBuffers()
{
	CUDA_VALIDATE(cudaFreeHost(resultHitPanelIds[0]));
	CUDA_VALIDATE(cudaFreeHost(resultHitPanelIds[1]));
													
	CUDA_VALIDATE(cudaFreeHost(results[0]));
	CUDA_VALIDATE(cudaFreeHost(results[1]));

}													

/*
Transfer the triangle list and the kd tree nodes to the GPU
*/

void KdTreeGpu::transferNodesToGpu()
{

	gpuTransferTime.start();

	CUDA_VALIDATE(cudaMalloc(&this->deviceTrianglesListPtr, trianglesList.size()*sizeof(TrianglePanelPair)));
	CUDA_VALIDATE(cudaMemcpyAsync(this->deviceTrianglesListPtr, &this->getTriangle(0), trianglesList.size()*sizeof(TrianglePanelPair), cudaMemcpyHostToDevice));

	CUDA_VALIDATE(cudaMalloc(&this->deviceNodesPtr, nodesList.size()*sizeof(KdTreeGpuNode)));
	CUDA_VALIDATE(cudaMemcpyAsync(this->deviceNodesPtr, &this->getNode(0), nodesList.size()*sizeof(KdTreeGpuNode), cudaMemcpyHostToDevice));

	gpuTransferTime.stop();

}

void KdTreeGpu::allocateResultGpuMemory()
{
	CUDA_VALIDATE(cudaMalloc(&this->deviceResults, MAX_RAYS_PER_ITERATION*sizeof(GpuResult)));
}

void KdTreeGpu::freeGpuMemory()
{
	if(this->deviceTrianglesListPtr)
	{
		CUDA_VALIDATE(cudaFree(this->deviceTrianglesListPtr));
	}

	if(this->deviceNodesPtr)
	{
		CUDA_VALIDATE(cudaFree(this->deviceNodesPtr));
	}

	if(this->deviceRayPtr)
	{
		CUDA_VALIDATE(cudaFree(this->deviceRayPtr));
	}

	CUDA_VALIDATE(cudaFree(deviceHitPanelIdPtr));
	CUDA_VALIDATE(cudaFree(deviceResults));
}

void KdTreeGpu::setupCuda()
{
	cudaSetDeviceFlags(cudaDeviceMapHost);
}