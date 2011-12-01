#include <string>
#include "GUISwitch.h"
using namespace std;
namespace SLDP
{
	GUISwitch::GUISwitch(GUINode* left1, GUINode* right1, GUINode* left2, GUINode* right2, const string& label)
		: GUIBase(left1->getX(),
		  left1->getY() + 8,
		  right2->getX() - left2->getX(),
		  right2->getY() - right1->getY() - 16,
		  label)
	{
		nodes[0] = left1;
		nodes[1] = right1;
		nodes[2] = left2;
		nodes[3] = right2;
		left1->rightConstraints = this;
		right1->leftConstraints = this;
		left2->rightConstraints = this;
		right2->leftConstraints = this;
	}

	void GUISwitch::markForDeletion()
	{
		GUIBase::markForDeletion();
		for (size_t i = 0; i < 4; ++i)
		{
			if (nodes[i]->leftConstraints == this)
				nodes[i]->leftConstraints = NULL;
			if (nodes[i]->rightConstraints == this)
				nodes[i]->rightConstraints = NULL;
		}
	}
	void GUISwitch::flipSwitch() { switchAll(!nodes[0]->rightSwitched); }
	void GUISwitch::switchAll(bool setting)
	{
		for (size_t i = 0; i < 4; ++i)
		{
			if (nodes[i]->leftConstraints == this)
				nodes[i]->leftSwitched = setting;
			if (nodes[i]->rightConstraints == this)
				nodes[i]->rightSwitched = setting;
		}
		nodes[0]->myRightEdges[0]->setVisible(!setting);
		nodes[0]->myRightEdges[1]->setVisible(setting);
		if(nodes[1]->myLeftEdges[0] != nodes[0]->myRightEdges[0]) nodes[1]->myLeftEdges[0]->setVisible(!setting);
		if(nodes[1]->myLeftEdges[1] != nodes[2]->myRightEdges[1]) nodes[1]->myLeftEdges[1]->setVisible(setting);
		nodes[2]->myRightEdges[0]->setVisible(!setting);
		nodes[2]->myRightEdges[1]->setVisible(setting);
//		if(nodes[3]->myLeftEdges[0] != nodes[0]->myRightEdges[1]) nodes[3]->myLeftEdges[0]->setVisible(!setting);
//		if(nodes[3]->myLeftEdges[1] != nodes[2]->myRightEdges[0]) nodes[3]->myLeftEdges[1]->setVisible(setting);
	}

	void GUISwitch::draw(CDC& context) const
	{
		drawLabel(context, 0, -24 - (getHeight() / 2));
	}

	GUINode** GUISwitch::getNodes() { return nodes; }
}