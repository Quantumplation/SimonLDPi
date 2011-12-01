#include <string>
#include <vector>
#include "Strategy.h"

namespace SLDP
{
	template<typename T>
	struct DijkstraDecorator : public T
	{
		DijkstraDecorator(const T& original) : T(original), accumulatedCost(INT_MAX), previous(NULL) {}

		double accumulatedCost;
		DijkstraDecorator<T>* previous;
	private:
	};

	typedef DijkstraDecorator<Node> DNode;

	class DijkstraStrategy : public Strategy
	{
	public:
		DijkstraStrategy(const std::string& startNode, std::vector<std::string> viableEnds) : startNodeLbl(startNode), endNodeLbls(viableEnds) {}
		virtual Result Execute(Track* track);

	private:

		bool allFound();
		DNode* inUnexplored(Node* node);
		bool inUnexplored(DNode* node);

		std::string startNodeLbl;
		std::vector<std::string> endNodeLbls;
		DNode* startNode;
		std::vector<DNode*> endNodes;
		std::vector<DNode*> path;
		std::vector<DNode*> unexplored;
	};	
}