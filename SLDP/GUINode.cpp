#include "GUINode.h"
#include <string>
#include <stdio.h>
using namespace std;
namespace SLDP
{
	GUINode::GUINode()
		: GUIBase(0, 0, 16, 16) {}
	GUINode::GUINode(long x, long y)
		: GUIBase(x - 8, y - 8, 16, 16) {}
	GUINode::GUINode(long x, long y, const string& newLabel)
		: GUIBase(x - 8, y - 8, 16, 16, newLabel) {}
	GUINode::GUINode(POINT p)
		: GUIBase(p.x - 8, p.y - 8, 16, 16) {}
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
	void GUINode::draw(CDC context) const
	{
		context.Ellipse(getLeft(), getTop(), getRight(), getBottom());
		drawLabel(context, 0, 8);
	}
}