#ifndef SURFACE_OBJECT_H
#define SURFACE_OBJECT_H
#include "Object.h"
#include "PanelList.h"
#include <iostream>

class SurfaceObjectConfiguration : public ObjectConfiguration
{
public:
	std::string name;
	float absorbtion;
	float transparency;
	float normalDirection;
};

class SurfaceObject : public Object
{
public:
	SurfaceObject(SurfaceObjectConfiguration soc);
	void incrementIntensity( double sunIntensity );
	virtual void addPanelsToList(PanelList& list) = 0;
	void resetIntensity( );
	float absorbtion;
	float transparency;
	float normalDirection;
	int hits;
private:
	std::string name;
	bool isCurrentlyEditing;
	double sunIntensity;
};

#endif