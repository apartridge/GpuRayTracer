#ifndef KdTreeNode_h__
#define KdTreeNode_h__

#include "PanelList.h"
#include "Axis.h"

class KdTreeNode
{
public:
	double s;
	Axis axis;
	Axis getAxis() const { return axis; }
	double getS() const {return s;}
protected:
	double findSplitLocation(PanelList & panels, int depth );
	double static surfaceAreaHeuristicCost(int NL, double SAL, int NR, double SAR, int straddle, double SARoot);
	double static surfaceAreaHeuristicLeafCost( int num_triangles );
};

#endif