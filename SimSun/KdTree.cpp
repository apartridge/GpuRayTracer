#include "KdTree.h"
#include "Panel.h"

static double min(double a, double b)
{
	return a < b ? a : b;
}

static double max(double a, double b)
{
	return a < b ? b : a;
}

BoundingBox KdTree::getPanelListBoundingBox(PanelList & panels)
{
	Vector3 mi = panels.at(0)->getBoundingBox().getMin();
	Vector3 ma = panels.at(0)->getBoundingBox().getMax();

	for(int i = 1; i < panels.size(); i++)
	{
		BoundingBox c = panels.at(i)->getBoundingBox();

		mi.x = min(mi.x, c.getMin().x);
		mi.y = min(mi.y, c.getMin().y);
		mi.z = min(mi.z, c.getMin().z);

		ma.x = max(ma.x, c.getMax().x);
		ma.y = max(ma.y, c.getMax().y);
		ma.z = max(ma.z, c.getMax().z);

	}

	return BoundingBox(mi, ma);

}