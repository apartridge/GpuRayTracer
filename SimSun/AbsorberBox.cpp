#include "PanelList.h"
#include "AbsorberBox.h"
#include "Misc.h"
#include "Panel.h"
#include "Quadrangle.h"

AbsorberBox::AbsorberBox(AbsorberBoxConfiguration& acg) : SurfaceObject(acg)
{
	this->dimension = acg.dimension;
	this->isBottomChecked = acg.isBottomChecked;
	this->isTopChecked = acg.isTopChecked;
}

void AbsorberBox::addPanelsToList(PanelList& panelList)
{
	double x = this->dimension.x/2.0;
	double y = this->dimension.y/2.0;
	double z = this->dimension.z/2.0;

	//double a = this->absorbtion;
	/*double r = 1-a;
	double t = this->transparency;
	double nDir = this->normalDirection;*/
	Quadrangle flatP;

	//panelCount++;

	Panel * FlP = new Panel(this, flatP);

	// make panels and add to model list and panel list

	Vector3 n1, n2, n3, n4, s1, s2, s3, s4;
	s1.x = -x+this->position.x; s1.y=-y+this->position.y; s1.z=-z+this->position.z;
	s2.x = -x+this->position.x; s2.y= y+this->position.y; s2.z=-z+this->position.z;
	s3.x =  x+this->position.x; s3.y= y+this->position.y; s3.z=-z+this->position.z;
	s4.x =  x+this->position.x; s4.y=-y+this->position.y; s4.z=-z+this->position.z;
	n1.x = -x+this->position.x; n1.y=-y+this->position.y; n1.z= z+this->position.z;
	n2.x = -x+this->position.x; n2.y= y+this->position.y; n2.z= z+this->position.z;
	n3.x =  x+this->position.x; n3.y= y+this->position.y; n3.z= z+this->position.z;
	n4.x =  x+this->position.x; n4.y=-y+this->position.y; n4.z= z+this->position.z;

	if (this->isBottomChecked)
	{
		flatP.p1=s1; flatP.p2=s2; flatP.p3=s3; flatP.p4=s4;
		FlP = new Panel(this, flatP);                // bottom
		panelList.push_back(FlP);
	}
	 
	if (this->isTopChecked)
	{
		flatP.p1=n1; flatP.p2=n2; flatP.p3=n3; flatP.p4=n4;
		FlP = new Panel(this, flatP);                // top
		panelList.push_back(FlP);
	}

	flatP.p1=s1; flatP.p2=s2; flatP.p3=n2; flatP.p4=n1;
	FlP = new Panel(this, flatP);                // left
	panelList.push_back(FlP);

	flatP.p1=s4; flatP.p2=s3; flatP.p3=n3; flatP.p4=n4;
	FlP = new Panel(this, flatP);                // right
	panelList.push_back(FlP);

	flatP.p1=s2; flatP.p2=n2; flatP.p3=n3; flatP.p4=s3;
	FlP = new Panel(this, flatP);                // back
	panelList.push_back(FlP);

	flatP.p1=s1; flatP.p2=n1; flatP.p3=n4; flatP.p4=s4;
	FlP = new Panel(this, flatP);                // front
	panelList.push_back(FlP);
}




