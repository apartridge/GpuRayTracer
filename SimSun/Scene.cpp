#include "Scene.h"
#include "KdTreeGpu.h"
#include "KdTreeCpu.h"
#include "LinearTracer.h"
#include "MTLinearTracer.h"
#include <omp.h>
#include "Misc.h"
#include "SurfaceObject.h"
#include "Vector3.h"
#include "RayHitInfo.h"
#include "Panel.h"
#include "BenchmarkTimer.h"

Scene::Scene()
{
	this->sceneList = QStringList();
	this->sceneList << "demo #1";
	this->sceneList << "demo #2";

	this->sceneListModel = new QStringListModel();
	this->sceneListModel->setStringList(this->sceneList);

	this->sceneList << "Demo2";

	this->surfaceObjects = QVector<SurfaceObject*>();

	this->traverser = NULL;
}

void Scene::addSurfaceObject( SurfaceObject* object )
{
	this->surfaceObjects.push_back(object);
	object->addPanelsToList(this->panels);
	this->createAccelerationStructure();
}

void Scene::drawObjects()
{
	for(int i = 0; i < panels.size(); i++)
	{
		panels[i]->drawPanel();
	}
	/*if(this->traverser)
	{
		this->traverser->drawTree();
	}*/
}

BenchmarkTimer gpuTotalTime;
BenchmarkTimer gpuTransferTime;

void Scene::traceAllRays()
{
	gpuTotalTime.reset();
	gpuTransferTime.reset();
	numberOfRaysGenerated = 0;
	BenchmarkTimer bt;
	bt.start();
	this->deleteResults();
	bt.stop();
	qDebug() << "Deleted results in " << bt.elapsedSec();
	this->traverser->traceRays(rayList, results, numberOfRaysGenerated);
}

ResultList & Scene::getResults()
{
	return this->results;
}

void Scene::createAccelerationStructure()
{
	delete this->traverser;
	this->traverser = new KdTreeGpu(panels);
}

RayList & Scene::getRayList()
{
	return rayList;
}

void Scene::deleteResults()
{
	this->results = ResultList();

	for(int i = 0; i < this->panels.size(); i++)
	{
		this->panels[i]->resetIntensity();
	}

	for(int i = 0; i < this->surfaceObjects.size(); i++)
	{
		this->surfaceObjects[i]->resetIntensity();
	}

}
