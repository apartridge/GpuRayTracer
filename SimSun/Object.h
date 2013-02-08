#ifndef OBJECT_H
#define OBJECT_H

#include "Vector3.h"

class ObjectConfiguration
{
public: 
	Vector3 position;
	Vector3 rotation;
};

class Object
{
public:
	Object(ObjectConfiguration& config);

	Vector3 position;
	Vector3 rotation;
};

#endif