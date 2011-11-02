#pragma once
#include "GUIBase.h"
#include "GUINode.h"
namespace SLDP
{
	class GUINode;
	class GUIEdge : public GUIBase
	{
	public:
		GUIEdge(GUINode* side1, GUINode* side2);
		bool isDead() const;
		GUINode* follow(const GUINode* caller) const;
		void draw(CDC context) const;
	private:
		GUINode* side1;
		GUINode* side2;
	};
}
