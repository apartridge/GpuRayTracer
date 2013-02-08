#ifndef KdTree_h__
#define KdTree_h__

#include "SceneTracer.h"
#include "BoundingBox.h"
#include "PanelList.h"

class KdTree : public SceneTracer
{
public:
	static BoundingBox getPanelListBoundingBox(PanelList & panels);
};

#endif