#ifndef KD_TREE_CPU_H
#define KD_TREE_CPU_H

#include "Axis.h"
#include "BoundingBox.h"
#include "TrianglePanelPair.h"
#include "PanelList.h"
#include "KdTreeCpuNode.h"
#include "Configuration.h"
#include "RayList.h"
#include "KdTree.h"

class KdTreeCpu : public KdTree
{
public:
	KdTreeCpu();
	~KdTreeCpu();
	KdTreeCpu(PanelList & panels);
	void traceRay(const Ray& ray, int rank, ResultList & results, int & numberOfRaysGenerated);
	bool traceRayAgainstKdTree(const Ray & ray, RayHitInfo & hitInfo) const;

	virtual void drawTree() const;
	virtual void traceRays(RayList & rays, ResultList & results, int & numberOfRaysGenerated);

private:
	KdTreeCpuNode* rootNode;
	BoundingBox sceneBoundingBox;
};

#endif