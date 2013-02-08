#ifndef LINEAR_TRAVERSER_H
#define LINEAR_TRAVERSER_H

#include "Axis.h"
#include "PanelList.h"
#include "Configuration.h"
#include "RayList.h"
#include "SceneTracer.h"
#include "RayHitInfo.h"
#include "ResultList.h"

class LinearTracer : public SceneTracer
{
public:
	LinearTracer(PanelList & panels);
	~LinearTracer();
	virtual void traceRays(RayList & rays, ResultList & results, int & numberOfRaysGenerated);
	bool traceRayAgainstPanels(const Ray & ray, RayHitInfo & hit);
	void traceRay(const Ray & ray, int rank, ResultList & results, int & numberOfRaysGenerated);
private:
	PanelList panels;
};

#endif