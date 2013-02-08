#ifndef KD_TREE_GPU_H
#define KD_TREE_GPU_H

#include "Axis.h"
#include "BoundingBox.h"
#include "TrianglePanelPair.h"
#include "PanelList.h"
#include "KdTreeGpuNode.h"
#include "Configuration.h"
#include "RayList.h"
#include "ResultList.h"
#include "KdTree.h"
#include "cuda.h"
#include "cuda_runtime.h"
#include "cuda_runtime_api.h"
#include "Result.h"

class Vector3;

struct GpuResult
{
public:
	double t;
	Panel* panel;
	//bool isAbsorbative;
	Ray reflectedRay;
	//Result result;
};


class KdTreeGpu : public KdTree
{
public:
	KdTreeGpu();
	~KdTreeGpu();
	KdTreeGpu(PanelList & panels);
	int addPanelsToTrianglesList(const PanelList& panels);
	int addKdTreeNodeToList(KdTreeGpuNode node);
	const KdTreeGpuNode & rootNode() const;
	const KdTreeGpuNode & getNode( int index ) const;
	const TrianglePanelPair & getTriangle (int index) const;
	void traceRayList(Ray* rays, int numRays, int rank, ResultList & results, int & numberOfRaysGenerated);
	void traceRaysOnGpuAsync(int firstRayIndex, int numRays, int rank, int buffer);
	
	void setupCuda();
	void waitForGpu();
	void transferNodesToGpu();
	void transferRaysToGpu(RayList & rayList);
	void allocateResultGpuMemory();
	void freeGpuMemory();
	
	virtual void drawTree() const;
	virtual void traceRays(RayList & rays, ResultList & results, int & numberOfRaysGenerated);
	void allocateHostResultBuffers();
	void freeHostResultBuffers();
	void transferPreviousIterationHitPanelIds(int rayOffset, int buffer, int numRays);

private:
	int rootNodeIndex;
	int numPrimaryRays;
	BoundingBox sceneBoundingBox;
	QVector<TrianglePanelPair> trianglesList;
	QVector<KdTreeGpuNode> nodesList;
	TrianglePanelPair* deviceTrianglesListPtr;
	KdTreeGpuNode* deviceNodesPtr;
	Ray* deviceRayPtr;

	int* deviceHitPanelIdPtr;

	//Vector3* deviceReflectionPtr;
	//double* deviceTHitPtr;
	GpuResult* deviceResults;

	int* resultHitPanelIds[2];
	//double* resultHitDistances[2];
	//Vector3* resultReflections[2];

	GpuResult* results[2];

	Ray* newRays;

	/*double* resultsTemp;
	bool* activeResultsTemp;*/

	static const int MAX_RAYS_PER_ITERATION = 100000;
	bool* activeRays;
};

#endif