#pragma once
#include "GUIBase.h"
#include "GUIEdge.h"
#include <string>
#include <vector>
namespace SLDP
{
	class GUIEdge;
	class GUINode : public GUIBase
	{
	public:
		GUINode();
		GUINode(long x, long y);
		GUINode(long x, long y, const std::string& label);
		GUINode(POINT p);
		bool connectsTo(const GUINode* other) const;
		void markForDeletion();
		void addEdge(GUIEdge* newEdge);
		void removeEdge(GUIEdge* removeEdge);
		void draw(CDC context) const;
	private:
		std::vector<GUIEdge*> myLeftEdges;
		std::vector<GUIEdge*> myRightEdges;
	};
}