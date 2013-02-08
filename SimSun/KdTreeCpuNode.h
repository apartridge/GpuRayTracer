#ifndef KdTreeCpuNode_h__
#define KdTreeCpuNode_h__

#include "Vector3.h"
#include "BoundingBox.h"
#include "Panel.h"
#include "TrianglePanelPair.h"
#include "KdTreeNode.h"
#include "PanelList.h"

class KdTreeCpu;

class KdTreeCpuNode : public KdTreeNode
{

public:
	KdTreeCpuNode();
	KdTreeCpuNode(PanelList &, int depth);
	~KdTreeCpuNode();

	void drawKdTreeNode() const;
	void drawKdTreeNodePanels() const;

	bool traceRay(const Ray & ray, RayHitInfo & hitInfo, double tNear, double tFar) const;

	KdTreeCpuNode* right;
	KdTreeCpuNode* left;

private:
	PanelList panels;
	
};
#endif