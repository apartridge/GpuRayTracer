#ifndef PARABOLOID_H
#define PARABOLOID_H

#include "SurfaceObject.h"

class ParaboloidConfiguration : public SurfaceObjectConfiguration
{
public:
	double diameterOuter;
	double diameterInner;
	double focus;
	int numRings;
	int numSectors;
	double tileSize;
	double shiftX;
	double shiftDiameter;
	bool isTilesChecked;
};

class Paraboloid : public SurfaceObject
{
public:
	Paraboloid(ParaboloidConfiguration pc);
	void addPanelsToList(PanelList& list);

	double diameterOuter;
	double diameterInner;
	double focus;
	int numRings;
	int numSectors;
	double tileSize;
	double shiftX;
	double shiftDiameter;
	bool isTilesChecked;

};

#endif
