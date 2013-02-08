#include "KdTreeGpuNode.h"
#include "KdTreeGpu.h"
#include "RayHitInfo.h"

KdTreeGpuNode::KdTreeGpuNode( )
{
	this->left = -1;
	this->right = -1;
}

KdTreeGpuNode::KdTreeGpuNode(KdTreeGpu & treeT, PanelList & panels, int depth )
{
	//this->tree = &treeT;
	if(depth < 7 && panels.size() >= 4 )
	{
		double cost = findSplitLocation(panels, depth);
		//qDebug() << "Depth " << depth << ": KD Tree Axis " << axis << ". Split at: " << this->s << " Size panels " << panels.size();

		if(cost < this->surfaceAreaHeuristicLeafCost(panels.size()))
		{
			PanelList listleft;
			PanelList listright;

			for(int i = 0; i < panels.size(); i++)
			{
				if(panels[i]->getBoundingBox().getMin().getField(axis) < s)
				{
					listleft.append(panels[i]);
				}

				if(panels[i]->getBoundingBox().getMax().getField(axis) > s)
				{
					listright.append(panels[i]);
				}
			}

			this->left = treeT.addKdTreeNodeToList(KdTreeGpuNode(treeT, listleft, depth+1));
			this->right = treeT.addKdTreeNodeToList(KdTreeGpuNode(treeT, listright, depth+1));
			return;

		}
	}

	this->left = -1;
	this->right = -1;

	this->globalListFirst = treeT.addPanelsToTrianglesList(panels);
	this->globalListSize = 2*panels.size();
	
}

KdTreeGpuNode::~KdTreeGpuNode()
{
	this->left = -1;
	this->right = -1;
}

void KdTreeGpuNode::drawKdTreeNode(const KdTreeGpu & treeT) const
{
	if(this->left != -1)
	{
		treeT.getNode(this->left).drawKdTreeNode(treeT);
		treeT.getNode(this->right).drawKdTreeNode(treeT);
	}
	else
	{
		for(int j = this->globalListFirst; j < this->globalListFirst + this->globalListSize; j++)
		{
			treeT.getTriangle(j).panel->drawPanel();
		}
	}

}