#pragma once

#include <vector>
#include <string>

namespace SLDP
{
	enum NodeFlags;
	enum Direction;
	enum EdgeFlags;
	class Node;
	class Edge;

	class Track
	{
	public:
		std::vector<Node*> getNodesExact(int flags) const;
		std::vector<Node*> getNodesAll(int flags) const;
		std::vector<Node*> getNodesAny(int flags) const;
		std::vector<Node*> getNodesNone(int flags) const;
		std::vector<Node*> getNodes(const std::string& label) const;
		std::vector<Node*> getNodes() const;
		Node* getFirstNode(const std::string& label) const { return getNodes(label)[0]; }
		
		std::vector<Edge*> getEdgesExact(int flags) const;
		std::vector<Edge*> getEdgesAll(int flags) const;
		std::vector<Edge*> getEdgesAny(int flags) const;
		std::vector<Edge*> getEdgesNone(int flags) const;
		std::vector<Edge*> getEdges(const std::string& label) const;
		std::vector<Edge*> getEdges() const;
		Edge* getFirstEdge(const std::string& label) const { return getEdges(label)[0]; }

		void loadFromFile(const std::string& filename);
	private:
		std::vector<Node*> nodes;
	};

	NodeFlags parseNodeFlags(const std::string& flag);
	Direction parseDirection(const std::string& flag);
	EdgeFlags parseEdgeFlags(const std::string& flag);
}