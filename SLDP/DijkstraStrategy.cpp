#include "DijkstraStrategy.h"
#include "Node.h"
#include "Edge.h"
#include <algorithm>
using namespace std;

namespace SLDP
{
	Result DijkstraStrategy::Execute(Track* track)
	{
		path.clear();
		unexplored.clear();
		startNode = &DNode(*track->getFirstNode(startNodeLbl));
		DNode* currentNode = startNode;
		vector<Node*> allNodes = track->getNodes();
		vector<Node*> path;
		
		for(int x = 0; x < allNodes.size(); x++)
		{
			unexplored.push_back(&DNode(*allNodes[x]));
			unexplored.back()->accumulatedCost = INT_MAX;
			unexplored.back()->previous = NULL;
		}

		for(int x = 0; x < endNodeLbls.size(); x++)
		{
			DNode* other = inUnexplored(track->getFirstNode(endNodeLbls[x]));
			if(other != NULL)
				endNodes.push_back(other);
		}

		while(!allFound())
		{
			vector<Edge*> edges = currentNode->getEdges();
			for(int x = 0; x < edges.size(); x++)
			{
				DNode* other = inUnexplored(edges[x]->follow(currentNode));
				if(other != NULL)
				{
					if(currentNode->accumulatedCost + edges[x]->getWeight() < other->accumulatedCost)
					{
						other->accumulatedCost = currentNode->accumulatedCost + edges[x]->getWeight();
						other->previous = currentNode;
					}
				}
			}

			int minVal = INT_MAX;
			int minIndex = INT_MAX;
			for(int x = 0; x < unexplored.size(); ++x)
			{
				if(unexplored[x]->accumulatedCost < minVal)
				{
					minVal = unexplored[x]->accumulatedCost;
					minIndex = x;
				}
			}

			currentNode = unexplored[minIndex];
			unexplored.erase(unexplored.begin() + minIndex);
		}

		int minVal = INT_MAX;
		int minIndex = INT_MAX;
		for(int x = 0; x < endNodes.size(); x++)
		{
			if(endNodes[x]->accumulatedCost < minVal)
			{
				minVal = endNodes[x]->accumulatedCost;
				minIndex = x;
			}
		}

		DNode* end = endNodes[minIndex];

		while(true)
		{
			path.push_back(end);
			end = end->previous;
		}

		reverse(path.begin(), path.end());

		return SUCCESS;
	}

	DNode* DijkstraStrategy::inUnexplored(Node* node)
	{
		for(int x = 0; x < unexplored.size(); x++)
			if(unexplored[x]->getLabel() == node->getLabel())
				return unexplored[x];
		return NULL;
	}

	bool DijkstraStrategy::inUnexplored(DNode* node)
	{
		for(int x = 0; x < unexplored.size(); x++)
			if(unexplored[x] == node)
				return true;
		return false;
	}

	bool DijkstraStrategy::allFound()
	{
		for(int x = 0; x < endNodes.size(); x++)
			if(inUnexplored(endNodes[x]))
				return false;
		return true;
	}
}