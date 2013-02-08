#pragma once
#include "Triangle.h"
class Panel;

class TrianglePanelPair
{
public:
	Triangle tri;
	Panel* panel;
	float panel_absorbtion;
};