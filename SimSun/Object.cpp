#include "Object.h"
Object::Object( ObjectConfiguration& config )
{
	this->position = config.position;
	this->rotation = config.rotation;
}

