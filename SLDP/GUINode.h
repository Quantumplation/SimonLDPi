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
		GUINode(POINT p);
		std::string getLabel() const;
		void setLabel(const std::string& newLabel);
		void setBeingRelabeled(bool relabled);
		void addCharToLabel(wchar_t newChar);
		void removeCharFromLabel();
		bool connectsTo(const GUINode* other) const;
		void markForDeletion();
		void addEdge(GUIEdge* newEdge);
		void removeEdge(GUIEdge* removeEdge);
		void draw(CDC context) const;
	private:
		std::vector<GUIEdge*> myEdges;
		bool relabeling;
		wchar_t* label;
		size_t labelSize;
	};
}