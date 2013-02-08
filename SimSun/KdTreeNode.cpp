#include "KdTreeNode.h"
#include "Panel.h"
#include "KdTreeGpu.h"

static const double costTriangles = 1;
static const double costBoxes = 0.5;

double KdTreeNode::surfaceAreaHeuristicCost( int NL, double SAL, int NR, double SAR, int straddle, double SARoot )
{
	double factor = 1;

	if(straddle == 0)
	{
		factor *= 0.7;
	}

	if(NL == 0 || NR == 0)
	{
		factor *= 0.7;
	}

	return factor * ( 2*costBoxes +  costTriangles*(NL*SAL/SARoot + NR*SAR/SARoot) );
}


double KdTreeNode::surfaceAreaHeuristicLeafCost( int num_triangles )
{
	return costTriangles * num_triangles;
}


static bool compareFunctionX(const Panel* a, const Panel* b)
{
	return a->getBoundingBox().getMax().x < b->getBoundingBox().getMax().x;
}

static bool compareFunctionY(const Panel* a, const Panel* b)
{
	return a->getBoundingBox().getMax().y < b->getBoundingBox().getMax().y;
}

static bool compareFunctionZ(const Panel* a, const Panel* b)
{
	return a->getBoundingBox().getMax().z < b->getBoundingBox().getMax().z;
}

static bool (*compareFunction(Axis axis))(const Panel*, const Panel*)
{
	switch(axis)
	{
	case X:
		return compareFunctionX;
	case Y:
		return compareFunctionY;
	}
	return compareFunctionZ;
}


double KdTreeNode::findSplitLocation( PanelList & panels, int depth )
{
	BoundingBox surroundingBox = KdTreeGpu::getPanelListBoundingBox(panels);
	double surfRootArea = surroundingBox.surfaceArea();

	// Store best split
	double bestCost = 1e200;
	//qDebug() << "* Checking axises";
	for(int axisI = X; axisI <= Z; axisI++)
	{
		//qDebug() << "## Axis " << axisI;

		BoundingBox fromLeft = surroundingBox;
		BoundingBox fromRight = surroundingBox;

		Axis axis = Axis(axisI);
		qSort(panels.begin(), panels.end(), compareFunction(axis));
		double lastSplitPosition = -1e100;

		for(int i = 0; i < panels.size(); i++)
		{
			double splitPosition = panels[i]->getBoundingBox().getMax().getField(axis);

			if(splitPosition <= lastSplitPosition)
			{
				continue;
			}

			// Set the left and right boxes

			Vector3 max = fromLeft.getMax();
			max.setField(axis, splitPosition);
			fromLeft.setMax(max);

			Vector3 min = fromRight.getMin();
			min.setField(axis, splitPosition);
			fromRight.setMin(min);

			// Calculate Num Panels in Left and Right Box

			int numLeft = 0;
			int numRight = 0;

			for(int j = 0; j < panels.size(); j++)
			{
				if(panels[j]->getBoundingBox().getMin().getField(axis) < splitPosition)
				{
					numLeft++;
				}
				if(panels[j]->getBoundingBox().getMax().getField(axis) > splitPosition)
				{
					numRight++;
				}
			}

			double surfAreaLeft = fromLeft.surfaceArea();
			double surfAreaRight = fromRight.surfaceArea();
			int straddle = numLeft + numRight - panels.size();

			double cost = surfaceAreaHeuristicCost(numLeft, surfAreaLeft, numRight, surfAreaRight, straddle, surfRootArea);

			if(cost < bestCost && numLeft > 0 && numRight > 0)
			{
				this->s = splitPosition;
				this->axis = axis;
				bestCost = cost;
			}

			lastSplitPosition = splitPosition;
		}
	}

	return bestCost;

}
