#pragma once
#include <string>
#include "GUINode.h"
namespace SLDP
{
	class GUINode;
	class GUISwitch : public GUIBase
	{
	public:
		GUISwitch(GUINode* left1, GUINode* right1, GUINode* left2, GUINode* right2, const std::string& label);
		void markForDeletion();
		void flipSwitch();
		void switchAll(bool setting);
		void draw(CDC& context) const;

		GUINode** getNodes();
	private:
		GUINode* nodes[4];
	};
}
