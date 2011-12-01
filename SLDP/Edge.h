#pragma once

#include <string>

namespace SLDP
{

	class Node;
	enum NodeFlags;
	enum EdgeFlags
	{
		EDGE_NONE = 0,
		EDGE_IMPASSABLE = 1,
		EDGE_PASSABLE = 2,
		EDGE_PREFERRED = 4,
	};

	class Edge
	{
	public:
		Edge(Node* nodeA, Node* nodeB, EdgeFlags flags, const std::string& label, double weight) 
			: nodeA(nodeA), 
			  nodeB(nodeB), 
			  flags(flags), 
			  label(label), 
			  weight(weight) 
		{
		}

		Node* follow(const Node* const start) const;
		
		std::string getLabel() const { return label; }
		double getWeight() const { return weight; }
		EdgeFlags getFlags() const { return flags; }

		void setLabel(const std::string& value) { label = value; }
		void setWeight(const double weight) { this->weight = weight; }
		void setFlags(EdgeFlags flags) { this->flags = flags; }
	private:
		std::string label;
		EdgeFlags flags;
		double weight;
		Node* nodeA;
		Node* nodeB;
	};
}