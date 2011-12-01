#include "Edge.h"
#include "Node.h"

using namespace std;

namespace SLDP
{
	Node* Edge::follow(const Node* const start) const
	{
		if(start->getLabel() == nodeA->getLabel())
			return nodeB;
		if(start->getLabel() == nodeB->getLabel())
			return nodeA;
		return NULL;
	}
}