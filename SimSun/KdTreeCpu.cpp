#include "KdTreeCpu.h"
#include "KdTreeCpuNode.h"
#include <QDebug>
#include "BenchmarkTimer.h"
#include "OpenGL.h"
#include "RayList.h"
#include "RayHitInfo.h"
#include "RankResultList.h"
#include "ResultList.h"
#include "Result.h"
#include "SurfaceObject.h"

KdTreeCpu::KdTreeCpu(PanelList & panels )
{
	this->rootNode = new KdTreeCpuNode(panels, 0);

	if(panels.size() > 0)
	{
		sceneBoundingBox = getPanelListBoundingBox(panels);
	}
	else
	{
		this->sceneBoundingBox = BoundingBox();
	}
}

KdTreeCpu::KdTreeCpu( )
{
	this->rootNode = NULL;
}

KdTreeCpu::~KdTreeCpu()
{
}

bool KdTreeCpu::traceRayAgainstKdTree(const Ray & ray, RayHitInfo & hitInfo) const
{
	BoundingBoxTNearFarPair pair;
	if(sceneBoundingBox.traceRay(ray, pair))
	{
		return rootNode->traceRay(ray, hitInfo, max(0, pair.tNear), pair.tFar);
	}

	return false;
}

extern int kdTreeNavigation[100];
extern int kdTreeNavigationLevel;

void KdTreeCpu::drawTree() const
{
	BoundingBox a = this->sceneBoundingBox;
	BoundingBox b = this->sceneBoundingBox;

	Vector3 va = a.getMin();
	va.setField(rootNode->getAxis(), rootNode->getS());
	a.setMin(va);

	Vector3 vb = b.getMax();
	vb.setField(rootNode->getAxis(), rootNode->getS());
	b.setMax(vb);


	glColor3f(1,0,0);
	a.drawBoundingBox();
	glColor3f(0,1,0);
	b.drawBoundingBox();

	if(this->rootNode->left != NULL)
	{
		if(kdTreeNavigation[0] == 1 || kdTreeNavigationLevel == 0)
			this->rootNode->left->drawKdTreeNodePanels();
		if(kdTreeNavigation[0] == 2 || kdTreeNavigationLevel == 0)
			this->rootNode->right->drawKdTreeNodePanels();
	}
}

void KdTreeCpu::traceRay(const Ray& ray, int rank, ResultList & results, int & numberOfRaysGenerated)
{
	RayHitInfo hit;
	RayHitInfo closestHit;
	closestHit.hitPanel = NULL;

	if(this->traceRayAgainstKdTree(ray, hit))
	{
		closestHit = hit;
	}
	else
		return;

	if(closestHit.hitPanel != NULL)
	{

		if(Vector3::dot(closestHit.normal, ray.getDirection()) < 0 )
		{

			if (rank >= results.size())
			{    
				#pragma omp critical
				{
					if (rank >= results.size())
					{
						RankResultList rankResultList = RankResultList();
						rankResultList.reserve(results[rank-1].size());
						results.push_back(rankResultList);
					}
				}
			}

			Result res = Result(ray, closestHit.t);
			
			#pragma omp critical
			{
				results[rank].push_back(res);
				closestHit.hitPanel->incrementIntensity(1.0);
			}

			if (closestHit.hitPanel->getParent().absorbtion < 1)
			{
				if (rank < 1000)
				{
					#pragma omp atomic
					numberOfRaysGenerated++;

					Vector3 reflectedDirection = ray.getDirection().reflect(closestHit.normal).normalized();
					Ray reflected (res.endPoint() + 0.00001*reflectedDirection, reflectedDirection);
					this->traceRay(reflected, rank + 1, results, numberOfRaysGenerated);
				}
			}
		}
	}
}

void KdTreeCpu::traceRays( RayList & rays, ResultList & results, int & numberOfRaysGenerated )
{

	qDebug() << "Kd Tree Cpu";

	RankResultList rankResultList = RankResultList();
	rankResultList.reserve(rays.size());
	results.push_back(rankResultList);

	#pragma omp parallel for schedule(guided)
	for (int i = 0; i < rays.size(); i++)
	{
		this->traceRay(rays[i], 0, results, numberOfRaysGenerated);
	}
}