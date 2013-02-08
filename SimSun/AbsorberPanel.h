#ifndef ABSORBER_PANEL_H
#define ABSORBER_PANEL_H

#include "SurfaceObject.h"
#include "PanelList.h"

class AbsorberPanelConfiguration : public SurfaceObjectConfiguration
{
public:
	double length;
	double width;
	double numLength;
	double numWidth;
};

class AbsorberPanel : public SurfaceObject
{
public:
	AbsorberPanel(AbsorberPanelConfiguration&);
	void addPanelsToList(PanelList& list);
	double length;
	double width;
	double numLength;
	double numWidth;
};

#endif
