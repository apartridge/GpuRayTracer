#include "KdTreeCpuNode.h"
#include "Ray.h"
#include "RayHitInfo.h"
#include "KdTreeCpu.h"
#include "PanelList.h"
#include "QDebug"

KdTreeCpuNode::KdTreeCpuNode( )
{
	this->left = NULL;
	this->right = NULL;
}

KdTreeCpuNode::KdTreeCpuNode(PanelList & panellist, int depth )
{
	if(panels.size() > 4 && depth < 8)
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

			//this->children = new KdTreeNode[2];
			this->left = new KdTreeCpuNode(listleft, depth+1);
			this->right = new KdTreeCpuNode(listright, depth+1);
			return;

		}
	}

	this->left = NULL;
	this->right = NULL;
	this->panels = panellist;

}

bool KdTreeCpuNode::traceRay(const Ray & ray, RayHitInfo & hitInfo, double tNear, double tFar) const
{

	if(this->left != NULL)
	{
		double tSplit = (this->s - ray.getOrigin().getField(this->axis))/ray.getDirection().getField(this->axis);

		const KdTreeCpuNode *nearNode, *farNode;
		if(ray.getDirection().getField(this->axis) >= 0 )
		{
			nearNode = this->left;
			farNode = this->right;
		}
		else
		{
			nearNode = this->right;
			farNode = this->left;
		}

		if (tSplit >= tFar) // Near
		{ 
			return nearNode->traceRay(ray, hitInfo, tNear, tFar);
		}
		else if (tSplit <= tNear) // Far
		{
			return farNode->traceRay(ray, hitInfo, tNear, tFar);
		}
		else // Evaluate Both
		{ 
			bool closeHit = nearNode->traceRay(ray, hitInfo, tNear, tSplit); 
			if(closeHit)
			{
				return true;
			}

			return farNode->traceRay(ray, hitInfo, tSplit, tFar);
		}

	}
	

	bool hit = false;
	hitInfo.t = tFar;

	for(int j = 0; j < panels.size(); j++)
	{
		if(panels[j]->findIntersectionPoint(ray, hitInfo, tNear, hitInfo.t ))
		{
			hitInfo.hitPanel =  this->panels[j];
			hit = true;
		}
	}

	return hit;
	

}

void KdTreeCpuNode::drawKdTreeNodePanels() const
{
	if(this->left != NULL)
	{
		this->left->drawKdTreeNodePanels();
		this->right->drawKdTreeNodePanels();
	}
	else
	{
		for(int j = 0; j < panels.size(); j++)
		{
			panels.at(j)->drawPanel();
		}
	}
}

KdTreeCpuNode::~KdTreeCpuNode()
{
	delete this->left;
	delete this->right;
	this->left = NULL;
	this->right = NULL;
	//this->children = NULL;
}