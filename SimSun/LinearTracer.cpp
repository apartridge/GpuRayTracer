#include "LinearTracer.h"
#include "Panel.h"
#include <QDebug>
#include "Result.h"
#include "SurfaceObject.h"

LinearTracer::LinearTracer( PanelList & panellist )
{
	this->panels = panellist;
}

bool LinearTracer::traceRayAgainstPanels(const Ray & ray, RayHitInfo & hit)
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

void LinearTracer::traceRay(const Ray & ray, int rank, ResultList & results, int & numberOfRaysGenerated)
{
	RayHitInfo hit;
	if(this->traceRayAgainstPanels(ray, hit))
	{
		if(Vector3::dot(hit.normal, ray.getDirection()) < 0)
		{

			if (rank >= results.size())
			{
				RankResultList rankResultList = RankResultList();
				results.push_back(rankResultList);
				results[rank].reserve(results[rank-1].size());
			}

			Result res = Result(ray, hit.t);
			results[rank].push_back(res);

			hit.hitPanel->incrementIntensity(1.0);

			if (hit.hitPanel->getParent().absorbtion < 1)
			{
				if (rank < 1000)
				{
					numberOfRaysGenerated++;
					Vector3 reflectedDirection = ray.getDirection().reflect(hit.normal).normalized();
					Ray reflected (res.endPoint() + 0.00001*reflectedDirection, reflectedDirection);
					this->traceRay(reflected, rank + 1, results, numberOfRaysGenerated);
				}
			}

		}
	}
}

void LinearTracer::traceRays( RayList & rays, ResultList & results, int & numberOfRaysGenerated )
{
	qDebug() << "Linear Traverser";

	RankResultList rankResultList = RankResultList();
	results.push_back(rankResultList);
	results[0].reserve(rays.size());
	//qDebug() << "Reserve Capacity " << results[0].capacity();

	for(int i = 0; i < rays.size(); i++)
	{
		traceRay(rays[i], 0, results, numberOfRaysGenerated);
	}

}
