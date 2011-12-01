#include "GUINode.h"
#include <string>
#include <stdio.h>
using namespace std;
namespace SLDP
{
	GUINode::GUINode()
		: GUIBase(0, 0, 16, 16),
		leftConstraints(NULL), rightConstraints(NULL),
		leftSwitched(false), rightSwitched(false) {}
	GUINode::GUINode(long x, long y)
		: GUIBase(x - 8, y - 8, 16, 16),
		leftConstraints(NULL), rightConstraints(NULL),
		leftSwitched(false), rightSwitched(false) {}
	GUINode::GUINode(long x, long y, const string& newLabel)
		: GUIBase(x - 8, y - 8, 16, 16, newLabel),
		leftConstraints(NULL), rightConstraints(NULL),
		leftSwitched(false), rightSwitched(false) {}
	GUINode::GUINode(POINT p)
		: GUIBase(p.x - 8, p.y - 8, 16, 16),
		leftConstraints(NULL), rightConstraints(NULL),
		leftSwitched(false), rightSwitched(false) {}
	bool GUINode::connectsTo(const GUINode* other) const
	{
		for (size_t i = 0; i < max(myLeftEdges.size(), myRightEdges.size()); ++i)
		{
			if (i < myLeftEdges.size() && myLeftEdges[i]->follow(this) == other)
				return true;
			if (i < myRightEdges.size() && myRightEdges[i]->follow(this) == other)
				return true;
		}
		return false;
	}
	bool GUINode::isSwitchedLeft() const { return leftSwitched; }
	bool GUINode::isSwitchedRight() const { return rightSwitched; }
	bool GUINode::isSwitchableLeft() const { return (myLeftEdges.size() > 1); }
	bool GUINode::isSwitchableRight() const { return (myRightEdges.size() > 1); }
	bool GUINode::hasConstraints() const { return (leftConstraints || rightConstraints); }
	void GUINode::setSwitchedLeft(bool isSwitched)
	{
		if (leftConstraints)
			leftConstraints->switchAll(isSwitched);
		else
		{
			leftSwitched = isSwitched;
			myLeftEdges[0]->setVisible(!isSwitched);
			myLeftEdges[1]->setVisible(isSwitched);
		}
	}
	void GUINode::setSwitchedRight(bool isSwitched)
	{
		if (rightConstraints)
			rightConstraints->switchAll(isSwitched);
		else
		{
			rightSwitched = isSwitched;
			myRightEdges[0]->setVisible(!isSwitched);
			myRightEdges[1]->setVisible(isSwitched);
		}
	}
	void GUINode::markForDeletion()
	{
		GUIBase::markForDeletion();
		while (myLeftEdges.size() > 0)
		{
			GUIEdge* temp = myLeftEdges[myLeftEdges.size() - 1];
			myLeftEdges.pop_back();
			GUINode* other = temp->follow(this);
			other->removeEdge(temp);
			temp->markForDeletion();
		}
		while (myRightEdges.size() > 0)
		{
			GUIEdge* temp = myRightEdges[myRightEdges.size() - 1];
			myRightEdges.pop_back();
			GUINode* other = temp->follow(this);
			other->removeEdge(temp);
			temp->markForDeletion();
		}
		if (leftConstraints)
			leftConstraints->markForDeletion();
		if (rightConstraints)
			rightConstraints->markForDeletion();
	}
	void GUINode::addEdge(GUIEdge* newEdge)
	{
		if (newEdge->getX() < getX())
			myLeftEdges.push_back(newEdge);
		else
			myRightEdges.push_back(newEdge);
	}
	void GUINode::removeEdge(GUIEdge* removeEdge)
	{
		for (size_t i = 0; i < max(myLeftEdges.size(), myRightEdges.size()); ++i)
		{
			if (i < myLeftEdges.size() && myLeftEdges[i] == removeEdge)
			{
				myLeftEdges[i] = myLeftEdges[myLeftEdges.size() - 1];
				myLeftEdges.pop_back();
				return;
			}
			if (i < myRightEdges.size() && myRightEdges[i] == removeEdge)
			{
				myRightEdges[i] = myRightEdges[myRightEdges.size() - 1];
				myRightEdges.pop_back();
				return;
			}
		}
	}
	void GUINode::draw(CDC& context) const
	{
		//if (!leftConstraints && !rightConstraints)
		{
			context.Ellipse(getMask());
			drawLabel(context, 0, -24);
		}
	}

	vector<GUIEdge*>& GUINode::getLeftEdges() { return myLeftEdges; }
	vector<GUIEdge*>& GUINode::getRightEdges() { return myRightEdges; }
	GUISwitch* GUINode::getLeftSwitch() { return leftConstraints; }
	GUISwitch* GUINode::getRightSwitch() { return rightConstraints; }
}