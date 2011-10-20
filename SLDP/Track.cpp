#include <iostream>
#include "Track.h"
#include "Node.h"
#include "Edge.h"
#include "Constraint.h"
#include "../tinyxml/tinyxml.h"
#include "../tinyxml/tinystr.h"

using namespace std;

namespace SLDP
{
	NodeFlags parseNodeFlags(const string& flag)
	{
		NodeFlags flags = NODE_NONE;
		if(string::npos != flag.find("NODE_TERMINAL"))
			flags = (NodeFlags)(flags | NODE_TERMINAL);
		if(string::npos != flag.find("NODE_SWITCH"))
			flags = (NodeFlags)(flags | NODE_SWITCH);
		return flags;
	}

	Direction parseDirection(const string& flag)
	{
		if(flag == "LEFT")
			return LEFT;
		if(flag == "RIGHT")
			return RIGHT;
		cerr << "Invalid Direction" << endl;
		return NONE;
	}

	EdgeFlags parseEdgeFlags(const string& flag)
	{
		EdgeFlags flags = EDGE_NONE;
		if(string::npos != flag.find("EDGE_IMPASSABLE"))
			flags = (EdgeFlags)(flags | EDGE_IMPASSABLE);
		if(string::npos != flag.find("EDGE_PASSABLE"))
			flags = (EdgeFlags)(flags | EDGE_PASSABLE);
		if(string::npos != flag.find("EDGE_PREFERRED"))
			flags = (EdgeFlags)(flags | EDGE_PREFERRED);
		return flags;
	}

	Constraint* parseConstraint(TiXmlElement* elem, Track* track, Node* node)
	{
		if(strcmp(elem->Attribute("type"), "CurrentEdge") == 0)
		{
			vector<Node*> nodes;
			vector<Direction> dirs;
			TiXmlElement* inner;
			nodes.push_back(node);
			dirs.push_back(parseDirection(elem->Attribute("direction")));
			for(inner = elem->FirstChildElement("Element"); inner; inner = inner->NextSiblingElement("Element"))
			{
				string label(inner->Attribute("node"));
				Direction d = parseDirection(inner->Attribute("direction"));
				nodes.push_back(track->getFirstNode(label));
				dirs.push_back(d);
			}
			return new CurrentEdge(nodes, dirs);
		}
	}

	vector<Node*> Track::getNodesExact(int flags) const
	{
		std::vector<Node*> ret;
		
		for(size_t x = 0; x < nodes.size(); ++x)
		{
			if(nodes[x]->getFlags() == flags)
			{
				ret.push_back(nodes[x]);
			}
		}
		return ret;
	}

	vector<Node*> Track::getNodesAll(int flags) const
	{
		std::vector<Node*> ret;
		for(size_t x = 0; x < nodes.size(); ++x)
		{
			if((nodes[x]->getFlags() & flags) == flags)
			{
				ret.push_back(nodes[x]);
			}
		}
		return ret;
	}

	vector<Node*> Track::getNodesAny(int flags) const
	{
		std::vector<Node*> ret;
		for(size_t x = 0; x < nodes.size(); ++x)
		{
			if(nodes[x]->getFlags() & flags)
			{
				ret.push_back(nodes[x]);
			}
		}
		return ret;
	}

	vector<Node*> Track::getNodesNone(int flags) const
	{
		std::vector<Node*> ret;
		for(size_t x = 0; x < nodes.size(); ++x)
		{
			if((nodes[x]->getFlags() & flags) == 0)
			{
				ret.push_back(nodes[x]);
			}
		}
		return ret;
	}

	vector<Node*> Track::getNodes(const std::string& label) const
	{
		vector<Node*> ret;
		for(size_t x = 0; x < nodes.size(); ++x)
		{
			if(nodes[x]->getLabel() == label)
				ret.push_back(nodes[x]);
		}
		return ret;
	}

	vector<Node*> Track::getNodes() const
	{
		return nodes;
	}

	vector<Edge*> Track::getEdgesExact(int flags) const
	{
		vector<Edge*> ret;
		for(size_t x = 0; x < nodes.size(); ++x)
		{
			vector<Edge*> tempRet = nodes[x]->getEdgesExact(flags);
			ret.insert(ret.end(), tempRet.begin(), tempRet.end());
		}
		return ret;
	}

	vector<Edge*> Track::getEdgesAll(int flags) const
	{
		vector<Edge*> ret;
		for(size_t x = 0; x < nodes.size(); ++x)
		{
			vector<Edge*> tempRet = nodes[x]->getEdgesAll(flags);
			ret.insert(ret.end(), tempRet.begin(), tempRet.end());
		}
		return ret;
	}

	vector<Edge*> Track::getEdgesAny(int flags) const
	{
		vector<Edge*> ret;
		for(size_t x = 0; x < nodes.size(); ++x)
		{			
			vector<Edge*> tempRet = nodes[x]->getEdgesAny(flags);
			ret.insert(ret.end(), tempRet.begin(), tempRet.end());
		}
		return ret;
	}

	vector<Edge*> Track::getEdgesNone(int flags) const
	{
		vector<Edge*> ret;
		for(size_t x = 0; x < nodes.size(); ++x)
		{
			vector<Edge*> tempRet = nodes[x]->getEdgesNone(flags);
			ret.insert(ret.end(), tempRet.begin(), tempRet.end());
		}
		return ret;
	}

	vector<Edge*> Track::getEdges(const string& label) const
	{
		vector<Edge*> ret;
		for(size_t x = 0; x < nodes.size(); ++x)
		{
			vector<Edge*> tempRet = nodes[x]->getEdges(label);
			ret.insert(ret.end(), tempRet.begin(), tempRet.end());
		}
		return ret;
	}

	vector<Edge*> Track::getEdges() const
	{
		vector<Edge*> ret;
		for(size_t x = 0; x < nodes.size(); ++x)
		{
			vector<Edge*> tempRet = nodes[x]->getEdges();
			ret.insert(ret.end(), tempRet.begin(), tempRet.end());
		}
		return ret;
	}
	
	void Track::loadFromFile(const string& filename)
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
				NodeFlags flags = parseNodeFlags(elem->Attribute("flags"));
				nodes.push_back(new Node(label, flags));
			}
		}

		//block: Edges
		{			
			for(elem = root->FirstChildElement("Nodes")->FirstChildElement("Node"); elem; elem = elem->NextSiblingElement("Node"))
			{
				string nodeLabel(elem->Attribute("label"));
				Node* node = getNodes(nodeLabel)[0];
				for(innerElem = elem->FirstChildElement("Edge"); innerElem; innerElem = innerElem->NextSiblingElement("Edge"))
				{
					string label(innerElem->Attribute("label"));
					EdgeFlags flags = parseEdgeFlags(innerElem->Attribute("flags"));
					Direction dir = parseDirection(innerElem->Attribute("direction"));
					string endpointLabel(innerElem->Attribute("endpoint"));
					Node* otherNode = getNodes(endpointLabel)[0];
					node->makeEdge(*otherNode, flags, dir, label);
				}
			}
		}

		//block: Constraints
		{
			for(elem = root->FirstChildElement("Nodes")->FirstChildElement("Node"); elem; elem = elem->NextSiblingElement("Node"))
			{
				string nodeLabel(elem->Attribute("label"));
				Node* node = getFirstNode(nodeLabel);
				for(innerElem = elem->FirstChildElement("Constraint"); innerElem; innerElem = innerElem->NextSiblingElement("Constraint"))
				{
					Constraint* c = parseConstraint(innerElem, this, node);
					vector<Node*> nodes = c->getAffectedNodes();
					for(size_t x = 0; x < nodes.size(); ++x)
					{
						nodes[x]->addConstraint(c);
					}
				}
			}
		}
	}
}