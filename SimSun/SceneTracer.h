#ifndef SceneTraverser_h__
#define SceneTraverser_h__

#include "RayList.h"
#include "ResultList.h"

class SceneTracer
{
public:
	virtual void traceRays(RayList & rays, ResultList & results, int & numberOfRaysGenerated) = 0;
};
#endif