#pragma once
#include "GUIEdge.h"
#include "GUINode.h"
#include <string>
#include <vector>
namespace SLDP
{
	class GUITrack
	{
	public:
		GUITrack();
		void leftPressed(POINT p);
		GUINode* leftReleased(POINT p); // returns currently selected node
		void rightReleased(POINT p);
		void removeDeletedElements();
		void labelNode (const std::string& label);
		void draw(CDC context) const;
		void loadFromFile(const std::string& filename);
		void saveToFile(const std::string& filename);
	private:
		std::vector<GUINode*> nodes;
		std::vector<GUIEdge*> edges;
		GUINode* startNode;
		GUINode* endNode;
		GUINode* nodeToLabel;
	};
};

