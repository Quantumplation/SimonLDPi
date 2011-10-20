#pragma once

#include <vector>
#include <map>

#define OPPOSITE(x) INT_MAX - x
#define REVERSE(x) (Direction)(OPPOSITE(x))

namespace SLDP
{
	class Edge;
	class Constraint;
	enum EdgeFlags;

	enum Direction
	{
		NONE = 0,
		LEFT = 32,
		RIGHT = OPPOSITE(LEFT),
	};

	enum NodeFlags
	{
		NODE_NONE = 0,
		NODE_TERMINAL = 1,
		NODE_SWITCH = 2,
	};

	class Node
	{
	public:
		friend class Constraint;
		Node() : label(""), flags(NODE_NONE) {}
		Node(const std::string& lbl, NodeFlags flags) : label(lbl), flags(flags) {}

		std::string getLabel() const { return label; }
		NodeFlags getFlags() const { return flags; }

		void setLabel(const std::string& label) { this->label = label; }
		void setFlags(NodeFlags flag) { this->flags = flag; }

		Edge* getCurrentEdge(Direction d) const;
		int getCurrentEdgeIndex(Direction d) const;
		bool edgeNotDefault(Direction d) const;

		std::vector<Edge*> getAllEdges(Direction d) const;
		std::vector<Edge*> getAllEdges(Direction d, EdgeFlags e) const;
		std::vector<Edge*> getEdgesExact(int flags) const;
		std::vector<Edge*> getEdgesAll(int flags) const;
		std::vector<Edge*> getEdgesAny(int flags) const;
		std::vector<Edge*> getEdgesNone(int flags) const;
		std::vector<Edge*> getEdges(const std::string& label) const;
		std::vector<Edge*> getEdges() const;

		void setCurrentEdge(Direction d, int index);
		// DON'T USE
		void _setCurrentEdge(Direction d, int index);
		void setCurrentEdge(Direction d, const Edge& e);
		
		void addEdge(Edge& e, Direction d);
		void makeEdge(Node& end, EdgeFlags flags, Direction d, const std::string& label, double weight = 1);

		void addConstraint(Constraint* c);

		bool removeEdge(Edge& e, Direction d);
		bool removeEdge(Edge& e);
		bool removeEdge(Node& n, Direction d);
		bool removeEdge(Node& n);
		bool removeEdge(Direction d, int num);
	
	private:
		
		struct Outbounds
		{
			Outbounds() : currentSelection(0) {}

			std::vector<Edge*> edges;
			int currentSelection;
		};

		std::string label;
		NodeFlags flags;
		std::map<Direction, Outbounds> outbounds;
		std::vector<Constraint*> constraints;
	};
}