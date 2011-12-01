#include <string>
#include <iostream>
#include "../tinyxml/tinyxml.h"
#include "../tinyxml/tinystr.h"
#include "GUITrack.h"
#include "Track.h"
#include "Edge.h"
using namespace std;
namespace SLDP
{
	GUITrack::GUITrack()
		: startNode(NULL), endNode(NULL), objectToLabel(NULL), setToModify(false), noPath(false) {}
	void GUITrack::leftPressed(POINT p)
	{
		if (!setToModify)
		for (size_t i = 0; i < switches.size(); ++i)
		{
			if (switches[i]->contains(p))
			{
				switches[i]->flipSwitch();
				return;
			}
		}
		GUIEdge* edgeToLabel = NULL;
		for (size_t i = 0; i < edges.size(); ++i)
		{
			if (edges[i]->contains(p))
			{
				if(edgeToLabel == NULL)
					edgeToLabel = edges[i];
				else
				{
					edgeToLabel = NULL;
					break;
				}
			}
		}
		if(edgeToLabel && !edgeToLabel->isPartOfSwitch())
		{
			if(!setToModify)
				edgeToLabel->alterBlocked();
			else
				objectToLabel = edgeToLabel;
			return;
		}

		for (size_t i = 0; i < nodes.size(); ++i)
		{
			if (nodes[i]->contains(p))
				startNode = nodes[i];
		}
		if (!startNode && setToModify)
			nodes.push_back(startNode = new GUINode(p));
	}
	GUIBase* GUITrack::leftReleased(POINT p)
	{
		if (!startNode && objectToLabel && setToModify)
		for (size_t i = 0; i < edges.size(); ++i)
			if (objectToLabel == edges[i])
			{
				objectToLabel->setBeingRelabeled(true);
				return objectToLabel;
			}
		if (startNode && setToModify)
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
				objectToLabel = startNode;
				objectToLabel->setBeingRelabeled(true);
			}
			else
				objectToLabel = NULL;
			startNode = NULL;
			endNode = NULL;
			return (objectToLabel);
		}

		if (startNode && !setToModify)
		{
			if (startNode->isSwitchableLeft())
				startNode->setSwitchedLeft(!startNode->isSwitchedLeft());
			if (startNode->isSwitchableRight())
				startNode->setSwitchedRight(!startNode->isSwitchedRight());
		}
		startNode = NULL;
		endNode = NULL;
		return NULL;
	}
	void GUITrack::rightReleased(POINT p)
	{
		if (!setToModify) return;
		for (size_t i = nodes.size(); i > 0; --i)
		{
			if (nodes[i - 1]->contains(p))
			{
				nodes[i - 1]->markForDeletion();
				break;
			}
		}
	}

	void GUITrack::loadFromFile(const string& filename)
	{
		TiXmlDocument doc( filename.c_str() );
		
		bool loadOkay = doc.LoadFile();
		if(!loadOkay)
		{
			cerr << "Unable to open file." << endl;
			return;
		}

		TiXmlElement* root = doc.FirstChildElement();
		TiXmlElement* elem;
		TiXmlElement* innerElem;
		if(!root) return;

		//block: Nodes
		{
			nodes.clear();

			for(elem = root->FirstChildElement("Nodes")->FirstChildElement("Node"); elem; elem = elem->NextSiblingElement("Node"))
			{
				string label(elem->Attribute("label"));
				int x, y;
				elem->QueryIntAttribute("x", &x);
				elem->QueryIntAttribute("y", &y);
				GUINode* n = new GUINode((long)x, (long)y, label);
				if(label.length() == 1)
					n->SetLabelVisible(false);
				if(string(elem->Attribute("flags")) == "NODE_NONE")
					n->SetLabelVisible(false);
				nodes.push_back(n);
			}
		}

		//block: Edges
		// NODES ON EDGES MUST BE DEFINED FROM LEFT TO RIGHT
		{			
			for(elem = root->FirstChildElement("Nodes")->FirstChildElement("Node"); elem; elem = elem->NextSiblingElement("Node"))
			{
				string nodeLabel(elem->Attribute("label"));
				GUINode* node = getFirstNode(nodeLabel);
				for(innerElem = elem->FirstChildElement("Edge"); innerElem; innerElem = innerElem->NextSiblingElement("Edge"))
				{
					string label(innerElem->Attribute("label"));
					string flags(innerElem->Attribute("flags"));
					string endpointLabel(innerElem->Attribute("endpoint"));
					GUINode* otherNode = getFirstNode(endpointLabel);
					GUIEdge* edge = new GUIEdge(node, otherNode, label);
					if(flags == "EDGE_NONE")
						edge->SetLabelVisible(false);
					edges.push_back(edge);
				}
			}
		}

		//block: Constraints
		// XML FILE VERY FRAGILE
		//   MUST LIST CONSTRAINTS IN ORDER RIGHT1, LEFT2, RIGHT2, AND MUST ONLY BE DEFINED ON LEFT1
		{
			for(elem = root->FirstChildElement("Nodes")->FirstChildElement("Node"); elem; elem = elem->NextSiblingElement("Node"))
			{
				string nodeLabel(elem->Attribute("label"));
				GUINode* node = getFirstNode(nodeLabel);
				for(innerElem = elem->FirstChildElement("Constraint"); innerElem; innerElem = innerElem->NextSiblingElement("Constraint"))
				{
					if(strcmp(innerElem->Attribute("type"), "CurrentEdge") == 0)
					{
						string constraintLabel = innerElem->Attribute("label");
						vector<GUINode*> nodes;
						TiXmlElement* inner;
						nodes.push_back(node);
						for(inner = innerElem->FirstChildElement("Element"); inner; inner = inner->NextSiblingElement("Element"))
						{
							string label(inner->Attribute("node"));
							nodes.push_back(getFirstNode(label));
						}
						GUISwitch* g = new GUISwitch(nodes[0], nodes[1], nodes[2], nodes[3], constraintLabel);
						switches.push_back(g);
					}
				}
			}
		}
	}

	void GUITrack::saveToFile(const string& filename)
	{
		TiXmlDocument doc;
		TiXmlDeclaration * decl = new TiXmlDeclaration( "1.0", "", "" );
		doc.LinkEndChild(decl);

		TiXmlElement* root = new TiXmlElement("Track");
		//block:Nodes
		{
			TiXmlElement* nodes = new TiXmlElement("Nodes");
			for(size_t x = 0; x < this->nodes.size(); x++)
			{
				TiXmlElement* node = new TiXmlElement("Node");
				string label(this->nodes[x]->getLabel());
				if(label == "")
					node->SetAttribute("flags", "NODE_NONE");
				long xPos = this->nodes[x]->getX();
				long yPos = this->nodes[x]->getY();
				node->SetAttribute("label", label.c_str());
				node->SetAttribute("x", xPos);
				node->SetAttribute("y", yPos);
				// SUPERHACK
				if(x <= 2 || this->nodes[x]->getLabel() == "R1" || this->nodes[x]->getLabel() == "R2" || this->nodes[x]->getLabel() == "R3")
					node->SetAttribute("flags", "NODE_TERMINAL");
				else if(this->nodes[x]->isSwitchableLeft() || this->nodes[x]->isSwitchableRight())
				{
					node->SetAttribute("flags", "NODE_SWITCH");
					GUISwitch* s = (this->nodes[x]->isSwitchableLeft() ? this->nodes[x]->getLeftSwitch() : this->nodes[x]->getRightSwitch());
					if(s != NULL && s->getNodes()[0] == this->nodes[x])
					{
						TiXmlElement* constraint = new TiXmlElement("Constraint");
						constraint->SetAttribute("type", "CurrentEdge");
						constraint->SetAttribute("direction", "RIGHT");
						constraint->SetAttribute("label", s->getLabel().c_str());
						TiXmlElement* elem = new TiXmlElement("Element");
						elem->SetAttribute("direction", "LEFT");
						elem->SetAttribute("node", s->getNodes()[1]->getLabel().c_str());
						constraint->LinkEndChild(elem);
						elem = new TiXmlElement("Element");
						elem->SetAttribute("direction", "RIGHT");
						elem->SetAttribute("node", s->getNodes()[2]->getLabel().c_str());
						constraint->LinkEndChild(elem);
						elem = new TiXmlElement("Element");
						elem->SetAttribute("direction", "LEFT");
						elem->SetAttribute("node", s->getNodes()[3]->getLabel().c_str());
						constraint->LinkEndChild(elem);
						node->LinkEndChild(constraint);
					}
				}

				if(!this->nodes[x]->GetLabelVisible())
						node->SetAttribute("flags", "NODE_NONE");

				//block:Edges
				{
					vector<GUIEdge*> rightEdges = this->nodes[x]->getRightEdges();
					for(size_t y = 0; y < rightEdges.size(); y++)
					{
						TiXmlElement* edge = new TiXmlElement("Edge");
						string edgeLabel(rightEdges[y]->getLabel());
						edge->SetAttribute("label", edgeLabel.c_str());
						edge->SetAttribute("direction", "RIGHT");
						edge->SetAttribute("flags", "EDGE_NONE");
						edge->SetAttribute("endpoint", rightEdges[y]->follow(this->nodes[x])->getLabel().c_str());
						node->LinkEndChild(edge);
					}
				}
				nodes->LinkEndChild(node);
			}
			root->LinkEndChild(nodes);
		}
		doc.LinkEndChild(root);
		doc.SaveFile(filename.c_str());
	}


	void GUITrack::removeDeletedElements()
	{
		vector<GUIBase*> deleteThese;
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
		for (size_t i = 0; i < switches.size(); ++i)
		{
			if (switches[i]->readyToDelete())
			{
				deleteThese.push_back(switches[i]);
				for (size_t j = i; j < switches.size() - 1; ++j)
					switches[j] = switches[j + 1];
				switches.pop_back();
				--i;
			}
		}
		for (size_t i = 0; i < deleteThese.size(); ++i)
			delete deleteThese[i];
	}

	void GUITrack::labelObject(const string& label)
	{
		if (objectToLabel)
		{
			objectToLabel->setLabel(label);
			objectToLabel = NULL;
		}
	}

	GUINode* GUITrack::addNode(GUINode* node)
	{
		nodes.push_back(node);
		return node;
	}

	void GUITrack::addEdge(GUIEdge* edge) { edges.push_back(edge); }

	void GUITrack::addSwitch(GUINode* left1, GUINode* right1, GUINode* left2, GUINode* right2, const std::string& label)
	{	
		switches.push_back(new GUISwitch(left1, right1, left2, right2, label));
	}

	void GUITrack::draw(CDC& context) const
	{
		for (size_t i = 0; i < edges.size(); ++i)
			edges[i]->draw(context);
		for (size_t i = 0; i < switches.size(); ++i)
			switches[i]->draw(context);
		for (size_t i = 0; i < nodes.size(); ++i)
			nodes[i]->draw(context);
		context.TextOutW(32, 32, noPath ? L"No Path" : L"Path");
	}

	void GUITrack::setMode(bool modify) { setToModify = modify; }
	void GUITrack::setNopath(bool result) { noPath = result; }
	bool GUITrack::getMode() const { return setToModify; }

	void GUITrack::modifyTrack(Track& track) const
	{
		for (size_t i = 0; i < edges.size(); ++i)
		{
			Edge* edge = track.getFirstEdge(edges[i]->getLabel());
			edge->setFlags(edges[i]->isBlocked() ? EDGE_IMPASSABLE : EDGE_PASSABLE);
		}
	}

	void GUITrack::readFromTrack(Track& track)
	{
		for(size_t x = 0; x < switches.size(); ++x)
		{
			Node* n = track.getFirstNode(switches[x]->getNodes()[0]->getLabel());
			switches[x]->getNodes()[0]->setSwitchedRight(n->edgeNotDefault(RIGHT));
		}

		vector<Node*> switchNodes = track.getNodesAll(NODE_SWITCH);
		for(size_t x = 0; x < switchNodes.size(); ++x)
		{
			if(!switchNodes[x]->hasConstraints())
			{
				GUINode* matchingNode = getFirstNode(switchNodes[x]->getLabel());
				if (matchingNode->isSwitchableLeft())
					matchingNode->setSwitchedLeft(switchNodes[x]->edgeNotDefault(LEFT));
				if (matchingNode->isSwitchableRight())
					matchingNode->setSwitchedRight(switchNodes[x]->edgeNotDefault(RIGHT));
			}
		}
	}

	GUINode* GUITrack::getFirstNode(const std::string& label)
	{
		for(size_t x = 0; x < nodes.size(); x++)
		{
			if(nodes[x]->getLabel() == label)
				return nodes[x];
		}
		return NULL;
	}
}