#pragma once
#include "GUIBase.h"
#include "GUIEdge.h"
#include "GUISwitch.h"
#include <string>
#include <vector>
namespace SLDP
{
	class GUIEdge;
	class GUINode : public GUIBase
	{
		friend class GUISwitch;
	public:
		GUINode();
		GUINode(long x, long y);
		GUINode(long x, long y, const std::string& label);
		GUINode(POINT p);
		~GUINode();
		bool connectsTo(const GUINode* other) const;
		bool isSwitchedLeft() const;
		bool isSwitchedRight() const;
		bool isSwitchableLeft() const;
		bool isSwitchableRight() const;
		bool hasConstraints() const;
		void setSwitchedLeft(bool isSwitched);
		void setSwitchedRight(bool isSwitched);
		void markForDeletion();
		void addEdge(GUIEdge* newEdge);
		void removeEdge(GUIEdge* removeEdge);
		void draw(CDC& context) const;

		std::vector<GUIEdge*>& getLeftEdges();
		std::vector<GUIEdge*>& getRightEdges();
		GUISwitch* getLeftSwitch();
		GUISwitch* getRightSwitch();
	private:
		std::vector<GUIEdge*> myLeftEdges;
		std::vector<GUIEdge*> myRightEdges;
		GUISwitch* leftConstraints;
		GUISwitch* rightConstraints;
		bool leftSwitched;
		bool rightSwitched;
	};
}