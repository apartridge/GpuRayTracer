#ifndef SCENE_H
#define SCENE_H

#include <QStringListModel>
#include <QVector>
#include "Result.h"
#include "Ray.h"
#include "PanelList.h"
#include "RayList.h"
#include "ResultList.h"
#include "PanelList.h"

class SceneTracer;
class SurfaceObject;

class Scene
{
public:
	Scene();
	QStringListModel* sceneListModel;
	void addSurfaceObject(SurfaceObject* object);
	void drawObjects();
	void traceAllRays();

	PanelList panels;
	ResultList & getResults();
	RayList & getRayList();
	void createAccelerationStructure();
	int getNumberOfRaysGenerated() const { return numberOfRaysGenerated; }

private:
	QStringList sceneList;
	QVector<SurfaceObject*> surfaceObjects;
	ResultList results;
	SceneTracer* traverser;
	RayList rayList;
	int numberOfRaysGenerated;
	void traceRayList(RayList & rays, int rank);
	void traceRay(const Ray & ray, int rayIndex, int rank, RayList& newRays);
	void deleteResults();
};

#endif