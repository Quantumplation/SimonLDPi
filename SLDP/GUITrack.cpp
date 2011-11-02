#include <string>
#include "GUITrack.h"
using namespace std;
namespace SLDP
{
	GUITrack::GUITrack() : startNode(NULL), endNode(NULL), nodeToLabel(NULL) {}
	void GUITrack::leftPressed(POINT p)
	{
		for (size_t i = 0; i < nodes.size(); ++i)
		{
			if (nodes[i]->contains(p))
				startNode = nodes[i];
		}
		if (!startNode)
			nodes.push_back(startNode = new GUINode(p));
	}
	GUINode* GUITrack::leftReleased(POINT p)
	{
		for (size_t i = 0; i < nodes.size(); ++i)
		{
			if (nodes[i]->contains(p))
				endNode = nodes[i];
		}
		if (!endNode)
			nodes.push_back(endNode = new GUINode(p));
		if (startNode != endNode && !startNode->connectsTo(endNode))
		{
			edges.push_back(new GUIEdge(startNode, endNode));
		}
		if (startNode == endNode)
		{
			nodeToLabel = startNode;
			nodeToLabel->setBeingRelabeled(true);
		}
		else
			nodeToLabel = NULL;
		startNode = NULL;
		endNode = NULL;
		return (nodeToLabel);
	}
	void GUITrack::rightReleased(POINT p)
	{
		for (size_t i = nodes.size(); i > 0; --i)
		{
			if (nodes[i - 1]->contains(p))
			{
				nodes[i - 1]->markForDeletion();
				break;
			}
		}
	}

	void GUITrack::removeDeletedElements()
	{
		for (size_t i = 0; i < nodes.size(); ++i)
		{
			if (nodes[i]->readyToDelete())
			{
				for (size_t j = i; j < nodes.size() - 1; ++j)
					nodes[j] = nodes[j + 1];
				nodes.pop_back();
				--i;
			}
		}
		for (size_t i = 0; i < edges.size(); ++i)
		{
			if (edges[i]->readyToDelete() || edges[i]->isDead())
			{
				for (size_t j = i; j < edges.size() - 1; ++j)
					edges[j] = edges[j + 1];
				edges.pop_back();
				--i;
			}
		}
	}

	void GUITrack::labelNode(const string& label)
	{
		if (nodeToLabel)
		{
			nodeToLabel->setLabel(label);
			nodeToLabel = NULL;
		}
	}

	void GUITrack::draw(CDC context) const
	{
		for (size_t i = 0; i < nodes.size(); ++i)
			nodes[i]->draw(context);
		for (size_t i = 0; i < edges.size(); ++i)
			edges[i]->draw(context);
	}
}