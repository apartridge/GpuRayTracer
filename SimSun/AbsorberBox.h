#ifndef ABSORBER_BOX_H
#define ABSORBER_BOX_H

#include "SurfaceObject.h"
#include "Vector3.h"

class AbsorberBoxConfiguration : public SurfaceObjectConfiguration
{
public:
	Vector3 dimension;
	bool isTopChecked;
	bool isBottomChecked;
};

class AbsorberBox : public SurfaceObject
{
public:
	AbsorberBox(AbsorberBoxConfiguration&);
	void addPanelsToList(PanelList& list);

	Vector3 dimension;
	bool isTopChecked;
	bool isBottomChecked;
};

#endif