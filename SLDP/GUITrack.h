#pragma once
#include "GUIEdge.h"
#include "GUINode.h"
#include "GUIButton.h"
#include "Track.h"
#include <string>
#include <vector>
namespace SLDP
{
	class GUITrack
	{
	public:
		GUITrack();
		void leftPressed(POINT p);
		GUIBase* leftReleased(POINT p); // returns currently selected object
		void rightReleased(POINT p);
		void removeDeletedElements();
		void labelObject (const std::string& label);
		GUINode* addNode(GUINode* node);
		void addEdge(GUIEdge* edge);
		void addSwitch(GUINode* left1, GUINode* right1, GUINode* left2, GUINode* right2, const std::string& label);
		void draw(CDC& context) const;
		void setMode(bool modify);
		bool getMode() const;
		void modifyTrack(Track& track) const;
		void loadFromFile(const std::string& filename);
		void saveToFile(const std::string& filename);

		GUINode* getFirstNode(const std::string& label);
	private:
		std::vector<GUINode*> nodes;
		std::vector<GUISwitch*> switches;
		std::vector<GUIEdge*> edges;
		GUINode* startNode;
		GUINode* endNode;
		GUIBase* objectToLabel;
		bool setToModify;
	};
};

