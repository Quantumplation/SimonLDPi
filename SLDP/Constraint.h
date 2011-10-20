#pragma once

#include <vector>
#include "Node.h"

namespace SLDP
{
	class Constraint
	{
		friend class Node;
	public:
		virtual std::vector<Node*> getAffectedNodes() = 0;
	protected:
		virtual void OnChange(Node* edge) = 0;
	};

	class CurrentEdge : public Constraint
	{
	public:
		CurrentEdge(std::vector<Node*> nodes, std::vector<Direction> dirs) 
		{
			if(nodes.size() != dirs.size())
			{
				exit(1);
			}
			for(size_t x = 0; x < nodes.size(); ++x)
			{
				DNP dnp;
				dnp.n = nodes[x];
				dnp.d = dirs[x];
				this->nodes.push_back(dnp);
			}
		}

		virtual std::vector<Node*> getAffectedNodes()
		{
			std::vector<Node*> ret;
			for(size_t x = 0; x < nodes.size(); ++x)
			{
				ret.push_back(nodes[x].n);
			}
			return ret;
		}
	protected:
		virtual void OnChange(Node* node)
		{
			bool element;
			int currentEdge;

			for(size_t x = 0; x < nodes.size(); ++x)
			{
				if(nodes[x].n == node)
				{
					element = true;
					currentEdge = node->getCurrentEdgeIndex(nodes[x].d);
					break;
				}
			}

			if(element)
			{
				for(size_t x = 0; x < nodes.size(); ++x)
				{
					if(nodes[x].n != node)
					{
						nodes[x].n->_setCurrentEdge(nodes[x].d, currentEdge);
					}
				}
			}
		}
	private:
		struct DNP
		{
			Direction d;
			Node* n;
		};
		std::vector<DNP> nodes;
	};
}