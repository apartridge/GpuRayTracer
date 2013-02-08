#include "MTLinearTracer.h"
#include "Panel.h"
#include <QDebug>
#include "Result.h"
#include "SurfaceObject.h"

MTLinearTracer::MTLinearTracer( PanelList & panellist )
{
	this->panels = panellist;
}

bool MTLinearTracer::traceRayAgainstPanels(const Ray & ray, RayHitInfo & hit)
{
	bool hasHit = false;

	for(int j = 0; j < panels.size(); j++)
	{
		if(panels[j]->findIntersectionPoint(ray, hit, 0, hit.t))
		{
			hit.hitPanel = panels[j];
			hasHit = true;
		}
	}
	return hasHit;
}

void MTLinearTracer::traceRay(const Ray & ray, int rank, ResultList & results, int & numberOfRaysGenerated)
{
	RayHitInfo hit;
	if(this->traceRayAgainstPanels(ray, hit))
	{
		if(Vector3::dot(hit.normal, ray.getDirection()) < 0)
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

			Result res = Result(ray, hit.t);

			#pragma omp critical
			{
				results[rank].push_back(res);
				hit.hitPanel->incrementIntensity(1.0);
			}

			if (hit.hitPanel->getParent().absorbtion < 1)
			{
				if (rank < 1000)
				{
					#pragma omp atomic
					numberOfRaysGenerated++;
					Vector3 reflectedDirection = ray.getDirection().reflect(hit.normal).normalized();
					Ray reflected (res.endPoint() + 0.00001*reflectedDirection, reflectedDirection);
					this->traceRay(reflected, rank + 1, results, numberOfRaysGenerated);
				}
			}

		}
	}
}

void MTLinearTracer::traceRays( RayList & rays, ResultList & results, int & numberOfRaysGenerated )
{
	qDebug() << "Multi-threaded Linear Traverser";

	RankResultList rankResultList = RankResultList();
	rankResultList.reserve(rays.size());
	results.push_back(rankResultList);

	#pragma omp parallel for schedule(dynamic)
	for(int i = 0; i < rays.size(); i++)
	{
		traceRay(rays[i], 0, results, numberOfRaysGenerated);
	}
}
