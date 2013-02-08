#include "AbsorberPanel.h"
#include "Misc.h"
#include "Panel.h"
#include "PanelList.h"

AbsorberPanel::AbsorberPanel(AbsorberPanelConfiguration& acg) : SurfaceObject(acg)
{
	this->length = acg.length;
	this->width = acg.width;
	this->numLength = acg.numLength;
	this->numWidth = acg.numWidth;
}

void AbsorberPanel::addPanelsToList(PanelList& list)
{
	Quadrangle flatP;

	// make panels and add to model list and panel list

	double delta_L = length/this->numLength;
	double delta_W = width/this->numWidth;

	for(int i=0; i<this->numLength;i++)
	{
		for (int j=0; j<this->numWidth;j++)
		{
			double L1 =i*delta_L-length/2;
			double L2 =(i+1)*delta_L-length/2;
			double W2 =(j+1)*delta_W-width/2;
			double W1 = j*delta_W-width/2;

			makeQuadranglefromRectangle(flatP, L1, L2, W1, W2, 0, 0, this->rotation, this->position);
			Panel * FlP = new Panel(this, flatP);

			list.push_back(FlP);
		}
	}
}