#ifndef KD_TREE_NODE_H
#define KD_TREE_NODE_H

#include "Vector3.h"
#include "BoundingBox.h"
#include "Panel.h"
#include "TrianglePanelPair.h"
#include "KdTreeNode.h"

class KdTreeGpu; 

class KdTreeGpuNode : public KdTreeNode
{
public:
	KdTreeGpuNode();
	~KdTreeGpuNode();
	KdTreeGpuNode(KdTreeGpu & , PanelList &, int depth);

	//void drawKdTreeNode() const;
	void drawKdTreeNode(const KdTreeGpu & treeT) const;

	int left;
	int globalListFirst;
	union
	{
		int right;
		int globalListSize;
	};

//private:
	//KdTreeGpu* tree;
};

#endif