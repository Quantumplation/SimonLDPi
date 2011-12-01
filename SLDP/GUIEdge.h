#pragma once
#include "GUIBase.h"
#include "GUINode.h"
namespace SLDP
{
	class GUINode;
	class GUIEdge : public GUIBase
	{
	public:
		GUIEdge(GUINode* side1, GUINode* side2, const std::string& label = "");
		bool isDead() const;
		bool isBlocked() const;
		bool isPartOfSwitch() const;
		bool contains(POINT p) const;
		void setVisible(bool isVisible);
		GUINode* follow(const GUINode* caller) const;
		void draw(CDC& context) const;
		void alterBlocked();
	private:
		GUINode* side1;
		GUINode* side2;
		bool visible;
		bool blocked;
	};
}
