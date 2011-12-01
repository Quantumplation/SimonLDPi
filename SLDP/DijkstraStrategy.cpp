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
		startNode = new DNode(*track->getFirstNode(startNodeLbl));
		DNode* currentNode = startNode;
		vector<Node*> allNodes = track->getNodes();
		vector<Node*> path;
		
		for(int x = 0; x < allNodes.size(); x++)
		{
			if(allNodes[x]->getLabel() == currentNode->getLabel())
				continue;
			unexplored.push_back(new DNode(*allNodes[x]));
			unexplored.back()->accumulatedCost = INT_MAX;
			unexplored.back()->previous = NULL;
		}

		for(int x = 0; x < endNodeLbls.size(); x++)
		{
			DNode* other = inUnexplored(track->getFirstNode(endNodeLbls[x]));
			if(other != NULL)
				endNodes.push_back(other);
		}

		currentNode->accumulatedCost = 0;
		Direction TraverseDir = (startNodeLbl[0] == 'L' ? RIGHT : LEFT);
		while(!allFound())
		{
			vector<Edge*> edges = currentNode->getEdges();
			for(int x = 0; x < edges.size(); x++)
			{
				DNode* other = inUnexplored(edges[x]->follow(currentNode));

				if(other != NULL)
				{				
					if(currentNode->getDirection(edges[x]) != TraverseDir)
					{
						unexplored.erase(find(unexplored.begin(), unexplored.end(), other));
						continue;
					}
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

			if(minIndex == INT_MAX)
				break;

			currentNode = unexplored[minIndex];
			unexplored.erase(unexplored.begin() + minIndex);
			if(find(endNodes.begin(), endNodes.end(), currentNode) != endNodes.end())
				break;
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

		if(minVal == INT_MAX)
			return NOPATH;

		DNode* end = endNodes[minIndex];

		while(end->previous)
		{
			path.push_back(end);
			end = end->previous;
		}

		reverse(path.begin(), path.end());

		for(size_t x = 0; x < path.size()-1; x++)
		{
			vector<Edge*> edges = path[x]->getEdges();
			Edge* followEdge = NULL;
			Direction dir;
			for(size_t y = 0; y < edges.size(); y++)
			{
				if(edges[y]->follow(path[x])->getLabel() == path[x+1]->getLabel())
				{
					followEdge = edges[y];
					dir = path[x]->getDirection(edges[y]);
					break;
				}
			}
			if(followEdge->getLabel() == "RS" || followEdge->getLabel() == "QThree" || followEdge->getLabel() == "KL")
			{
				track->getFirstNode("S")->setCurrentEdge(LEFT, 1);
				track->getFirstNode("K")->setCurrentEdge(RIGHT, 0);
			}
			else if(followEdge->getLabel() == "KS" || followEdge->getLabel() == "QL")
			{
				track->getFirstNode("K")->setCurrentEdge(RIGHT, 1);
				track->getFirstNode("Three")->_setCurrentEdge(LEFT, 1);
			}
			else if(followEdge->getLabel() == "MN" || followEdge->getLabel() == "FG")
				track->getFirstNode("F")->setCurrentEdge(RIGHT, 0);
			else if(followEdge->getLabel() == "FN" || followEdge->getLabel() == "MG")
				track->getFirstNode("F")->setCurrentEdge(RIGHT, 1);
			else if(followEdge->getLabel() == "OP" || followEdge->getLabel() == "VW")
				track->getFirstNode("O")->setCurrentEdge(RIGHT, 0);
			else if(followEdge->getLabel() == "OW" || followEdge->getLabel() == "VP")
				track->getFirstNode("O")->setCurrentEdge(RIGHT, 1);
			else if(followEdge->getLabel() == "IJ" || followEdge->getLabel() == "AB")
				track->getFirstNode("A")->setCurrentEdge(RIGHT, 0);
			else if(followEdge->getLabel() == "AJ" || followEdge->getLabel() == "IB")
				track->getFirstNode("A")->setCurrentEdge(RIGHT, 1);
			else if(followEdge->getLabel() == "FourEMPTY19")
				track->getFirstNode("Four")->setCurrentEdge(RIGHT, 0);
			else if(followEdge->getLabel() == "FourEMPTY11")
				track->getFirstNode("Four")->setCurrentEdge(RIGHT, 1);
			else if(followEdge->getLabel() == "EMPTY10Eight")
				track->getFirstNode("Eight")->setCurrentEdge(LEFT, 0);
			else if(followEdge->getLabel() == "EMPTY7Eight")
				track->getFirstNode("Eight")->setCurrentEdge(LEFT, 1);
			else if(followEdge->getLabel() == "REMPTY5")
				track->getFirstNode("Three")->setCurrentEdge(RIGHT, 1);
			else if(followEdge->getLabel() == "EMPTY6Six")
				track->getFirstNode("Six")->setCurrentEdge(LEFT, 1);
			else if(followEdge->getLabel() == "EMPTY12Six")
				track->getFirstNode("Six")->setCurrentEdge(LEFT, 0);
			if(followEdge->getLabel() == "RS")
				track->getFirstNode("Three")->setCurrentEdge(RIGHT, 0);
		}

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