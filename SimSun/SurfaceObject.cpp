#include "SurfaceObject.h"

SurfaceObject::SurfaceObject(SurfaceObjectConfiguration soc) : Object(soc)
{
	this->absorbtion = soc.absorbtion;
	this->transparency = soc.transparency;
	this->normalDirection = soc.normalDirection;
	this->isCurrentlyEditing = false;
	this->name = soc.name;
}

void SurfaceObject::incrementIntensity( double sunIntensity )
{
	this->sunIntensity += sunIntensity;
	this->hits++;
}

void SurfaceObject::resetIntensity( )
{
	this->sunIntensity = 0;
	this->hits = 0;
}
