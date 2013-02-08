#include "KdTreeGpu.h"
#include "KdTreeGpuNode.h"
#include <QDebug>
#include "BenchmarkTimer.h"
#include "OpenGL.h"
#include "RayList.h"
#include "Result.h"
#include "RayHitInfo.h"
#include "SurfaceObject.h"
#include "nvToolsExt.h"

KdTreeGpu::KdTreeGpu(PanelList & panels )
{
	KdTreeGpu();
	this->rootNodeIndex = this->addKdTreeNodeToList(KdTreeGpuNode(*this, panels, 0));

	if(panels.size() > 0)
	{
		sceneBoundingBox = getPanelListBoundingBox(panels);
	}
	else
	{
		this->sceneBoundingBox = BoundingBox();
	}
}

KdTreeGpu::KdTreeGpu( )
{
	this->rootNodeIndex = 0;
	this->deviceNodesPtr = NULL;
	this->deviceRayPtr = NULL;
	this->deviceResults = NULL;
	setupCuda();
}

KdTreeGpu::~KdTreeGpu()
{
}

void KdTreeGpu::traceRays(RayList & rays, ResultList & results, int & numberOfRaysGenerated)
{
	this->transferNodesToGpu();
	this->allocateResultGpuMemory();
	this->waitForGpu();

	this->numPrimaryRays = rays.size();
	this->newRays = new Ray[numPrimaryRays];
	this->activeRays = new bool[numPrimaryRays];
	memset(this->activeRays, 1, sizeof(bool)*this->numPrimaryRays);

	allocateHostResultBuffers();

	transferRaysToGpu(rays);
	waitForGpu();
	this->traceRayList(&rays[0], rays.size(), 0, results, numberOfRaysGenerated);
	this->freeGpuMemory();

	freeHostResultBuffers();

	delete this->newRays;
}


void KdTreeGpu::traceRayList(Ray* rays, int numRays, int rank, ResultList & resultList, int & numberOfRaysGenerated)
{
	if(rank > 1000)
	{
		return;
	}

	resultList.push_back(RankResultList());
	resultList[rank].reserve(numRays);

	int numNewRays = numberOfRaysGenerated;

	BenchmarkTimer bt;
	bt.start();

	/*
	Go through all the rays in several iterations, so that we keep CPU and GPU busy busy
	*/

	int numIterations = ceil(float(this->numPrimaryRays) / MAX_RAYS_PER_ITERATION);
	int numRaysPrevious = min(MAX_RAYS_PER_ITERATION, this->numPrimaryRays);
	traceRaysOnGpuAsync(0, numRaysPrevious, rank, 0);

	for(int iteration = 0; iteration < numIterations; iteration++)
	{
		int rayFrom = (iteration+1)*MAX_RAYS_PER_ITERATION;
		int rayTo = min((iteration+2)*MAX_RAYS_PER_ITERATION, this->numPrimaryRays) - 1;
		int numRaysIteration = rayTo-rayFrom+1;

		// Wait for results to finish and get them

		waitForGpu();

		// Trace the next iteration asynchronously. This will have data prepared for next iteration
	
		if(numRaysIteration > 0)
		{
			int nextBuffer = (iteration+1) & 1;
			traceRaysOnGpuAsync(rayFrom, numRaysIteration, rank, nextBuffer);
		}

		nvtxRangePushA("CpuWork");

		// Store results for current iteration
		
		int rayOffset = iteration*MAX_RAYS_PER_ITERATION;
		int buffer = iteration & 1;
		
		for(int i = 0; i < numRaysPrevious; i++)
		{
			bool active = this->activeRays[rayOffset+i];
			this->activeRays[rayOffset+i] = false;
			if(resultHitPanelIds[buffer][i] >= 0 && active)
			{

				GpuResult gresult = results[buffer][i];

				Result result = Result(rays[rayOffset+i], gresult.t);
				resultList[rank].push_back(result);
				gresult.panel->incrementIntensity(1.0);
				
				if (gresult.panel->getAbsorbtion() < 1)
				{
					numberOfRaysGenerated++;
					this->newRays[rayOffset+i] = gresult.reflectedRay;
					this->activeRays[rayOffset+i] = true;
				}
			}

		}
		numRaysPrevious = numRaysIteration;

		nvtxRangePop();

	}

	numNewRays = numberOfRaysGenerated - numNewRays;

	if(numNewRays > 0)
	{
		traceRayList(this->newRays, numNewRays, rank+1, resultList, numberOfRaysGenerated);
	}
}

extern int kdTreeNavigation[100];
extern int kdTreeNavigationLevel;

void KdTreeGpu::drawTree() const
{
	BoundingBox a = this->sceneBoundingBox;
	BoundingBox b = this->sceneBoundingBox;

	Vector3 va = a.getMin();
	va.setField(this->nodesList[this->rootNodeIndex].getAxis(), this->nodesList[this->rootNodeIndex].getS());
	a.setMin(va);

	Vector3 vb = b.getMax();
	vb.setField(this->nodesList[this->rootNodeIndex].getAxis(), this->nodesList[this->rootNodeIndex].getS());
	b.setMax(vb);

	glColor3f(1,0,0);
	b.drawBoundingBox();
	glColor3f(0,1,0);
	a.drawBoundingBox();

	if(this->nodesList.size() > 0 && this->nodesList[this->rootNodeIndex].left != -1)
	{
		if(kdTreeNavigation[0] == 1 || kdTreeNavigationLevel == 0)
		{
			KdTreeGpuNode a = this->nodesList[this->rootNodeIndex];
			this->nodesList[this->nodesList[this->rootNodeIndex].left].drawKdTreeNode(*this);
		}

		if(kdTreeNavigation[0] == 2 || kdTreeNavigationLevel == 0)
		{
			this->nodesList[this->nodesList[this->rootNodeIndex].right].drawKdTreeNode(*this);
		}
	}
}

int KdTreeGpu::addPanelsToTrianglesList( const PanelList& panels )
{
	int first = this->trianglesList.size();

	for(int i = 0; i < panels.size(); i++)
	{
		TrianglePanelPair p;
		p.panel = panels.at(i);
		p.panel_absorbtion = p.panel->getParent().absorbtion;
		p.tri = panels.at(i)->getTriangle1();
		this->trianglesList.push_back(p);
		p.tri = panels.at(i)->getTriangle2();
		this->trianglesList.push_back(p);
	}

	return first;
}

int KdTreeGpu::addKdTreeNodeToList(KdTreeGpuNode node)
{
	this->nodesList.push_back(node);
	return this->nodesList.size() - 1;
}

const KdTreeGpuNode & KdTreeGpu::getNode( int index ) const
{
	return this->nodesList[index];
}

const TrianglePanelPair & KdTreeGpu::getTriangle( int index ) const
{
	return this->trianglesList[index];
}