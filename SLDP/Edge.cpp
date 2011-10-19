#include "Edge.h"
#include "Node.h"

using namespace std;

namespace SLDP
{
	Node* Edge::follow(const Node* const start) const
	{
		if(start == nodeA)
			return nodeB;
		if(start == nodeB)
			return nodeA;
		return NULL;
	}
}