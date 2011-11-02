#include "GUINode.h"
#include <string>
#include <stdio.h>
using namespace std;
namespace SLDP
{
	GUINode::GUINode()
		: GUIBase(0, 0, 16, 16),
		relabeling(false), label(new wchar_t[10]), labelSize(0)
	{
		label[0] = '\0';
	}
	GUINode::GUINode(long x, long y)
		: GUIBase(x - 8, y - 8, 16, 16),
		relabeling(false), label(new wchar_t[10]), labelSize(0)
	{
		label[0] = '\0';
	}
	GUINode::GUINode(POINT p)
		: GUIBase(p.x - 8, p.y - 8, 16, 16),
		relabeling(false), label(new wchar_t[10]), labelSize(0)
	{
		label[0] = '\0';
	}
	string GUINode::getLabel() const
	{
		string result = "";
		for (size_t i = 0; i < labelSize; ++i)
			result += (char)label[i];
		return result;
	}
	void GUINode::setLabel(const string& newLabel)
	{
		const char* toConvert = newLabel.c_str();
		for (size_t i = 0; i < newLabel.length(); ++i)
			label[i] = (wchar_t)toConvert[i];
		label[newLabel.length()] = '\0';
		labelSize = newLabel.length();
	}
	void GUINode::setBeingRelabeled(bool relabeled)
	{
		relabeling = relabeled;
		if (relabeled) label[labelSize] = '|';
		else label[labelSize] = '\0';
	}
	void GUINode::addCharToLabel(wchar_t newChar)
	{
		if (labelSize > 9) return;
		else
		{
			label[labelSize++] = newChar;
			label[labelSize] = '|';
		}
	}
	void GUINode::removeCharFromLabel()
	{
		if (labelSize < 1) return;
		else
			label[--labelSize] = '|';
	}
	bool GUINode::connectsTo(const GUINode* other) const
	{
		for (size_t i = 0; i < myEdges.size(); ++i)
		{
			if (myEdges[i]->follow(this) == other)
				return true;
		}
		return false;
	}
	void GUINode::markForDeletion()
	{
		GUIBase::markForDeletion();
		while (myEdges.size() > 0)
		{
			GUIEdge* temp = myEdges[myEdges.size() - 1];
			myEdges.pop_back();
			GUINode* other = temp->follow(this);
			other->removeEdge(temp);
			temp->markForDeletion();
		}
	}
	void GUINode::addEdge(GUIEdge* newEdge)
	{
		myEdges.push_back(newEdge);
	}
	void GUINode::removeEdge(GUIEdge* removeEdge)
	{
		for (size_t i = 0; i < myEdges.size(); ++i)
		{
			if (myEdges[i] == removeEdge)
			{
				myEdges[i] = myEdges[myEdges.size() - 1];
				myEdges.pop_back();
			}
		}
	}
	void GUINode::draw(CDC context) const
	{
		context.Ellipse(getLeft(), getTop(), getRight(), getBottom());
		context.TextOutW(getX() - labelSize * 4, getBottom(), label, labelSize + 1);
	}
}