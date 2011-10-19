#include "Node.h"
#include "Edge.h"

using namespace std;

namespace SLDP
{
	Edge* Node::getCurrentEdge(Direction d) const
	{
		Outbounds o = this->outbounds.at(d);
		return o.edges[o.currentSelection];
	}

	bool Node::edgeNotDefault(Direction d) const
	{
		return outbounds.at(d).currentSelection != 0;
	}

	vector<Edge*> Node::getAllEdges(Direction d) const
	{
		return outbounds.at(d).edges;
	}

	vector<Edge*> Node::getAllEdges(Direction d, EdgeFlags e) const
	{
		vector<Edge*> ret;
		Outbounds o = this->outbounds.at(d);
		for(size_t x = 0; x < o.edges.size(); ++x)
		{
			if(o.edges[x]->getFlags() == e)
				ret.push_back(o.edges[x]);
		}
		return ret;
	}

	vector<Edge*> Node::getEdgesExact(int flags) const
	{
		vector<Edge*> ret;
		for(map<Direction, Outbounds>::const_iterator it = outbounds.begin(); it != outbounds.end(); it++)
		{
			Outbounds o = it->second;
			for(size_t x = 0; x < o.edges.size(); ++x)
			{
				if(o.edges[x]->getFlags() == flags)
					ret.push_back(o.edges[x]);
			}
		}
		return ret;
	}

	vector<Edge*> Node::getEdgesAll(int flags) const
	{
		vector<Edge*> ret;
		for(map<Direction, Outbounds>::const_iterator it = outbounds.begin(); it != outbounds.end(); it++)
		{
			Outbounds o = it->second;
			for(size_t x = 0; x < o.edges.size(); ++x)
			{
				if((o.edges[x]->getFlags() & flags) == flags)
					ret.push_back(o.edges[x]);
			}
		}
		return ret;
	}

	vector<Edge*> Node::getEdgesAny(int flags) const
	{
		vector<Edge*> ret;
		for(map<Direction, Outbounds>::const_iterator it = outbounds.begin(); it != outbounds.end(); it++)
		{
			Outbounds o = it->second;
			for(size_t x = 0; x < o.edges.size(); ++x)
			{
				if(o.edges[x]->getFlags() & flags)
					ret.push_back(o.edges[x]);
			}
		}
		return ret;
	}

	vector<Edge*> Node::getEdgesNone(int flags) const
	{
		vector<Edge*> ret;
		for(map<Direction, Outbounds>::const_iterator it = outbounds.begin(); it != outbounds.end(); it++)
		{
			Outbounds o = it->second;
			for(size_t x = 0; x < o.edges.size(); ++x)
			{
				if((o.edges[x]->getFlags() & flags) == 0)
					ret.push_back(o.edges[x]);
			}
		}
		return ret;
	}

	vector<Edge*> Node::getEdges(const string& label) const
	{
		vector<Edge*> ret;
		for(map<Direction, Outbounds>::const_iterator it = outbounds.begin(); it != outbounds.end(); it++)
		{
			Outbounds o = it->second;
			for(size_t x = 0; x < o.edges.size(); ++x)
			{
				if(o.edges[x]->getLabel() == label)
					ret.push_back(o.edges[x]);
			}
		}
		return ret;
	}
	
	vector<Edge*> Node::getEdges() const
	{
		vector<Edge*> ret;
		for(map<Direction, Outbounds>::const_iterator it = outbounds.begin(); it != outbounds.end(); it++)
		{
			Outbounds o = it->second;
			ret.insert(ret.begin(), o.edges.begin(), o.edges.end());
		}
		return ret;
	}
	
	void Node::setCurrentEdge(Direction d, int index)
	{
		outbounds.at(d).currentSelection = index;
	}

	void Node::setCurrentEdge(Direction d, const Edge& edge)
	{
		Outbounds o = outbounds.at(d);
		for(size_t x = 0; x < o.edges.size(); ++x)
		{
			if(o.edges[x] == &edge)
				o.currentSelection = x;
		}
	}

	void Node::addEdge(Edge& e, Direction d)
	{
		outbounds[d].edges.push_back(&e);
	}

	void Node::makeEdge(Node& end, EdgeFlags flags, Direction d, const string& label, double weight)
	{
		Edge* e = new Edge(this, &end, flags, label, weight);
		outbounds[d].edges.push_back(e);
		end.addEdge(*e, REVERSE(d));
	}

	bool Node::removeEdge(Edge& e, Direction d)
	{
		Outbounds o = outbounds.at(d);
		for(size_t x = 0; x < o.edges.size(); ++x)
		{
			if(o.edges[x] == &e)
			{
				removeEdge(d, x);
			}
		}
		return false;
	}

	bool Node::removeEdge(Edge& e)
	{
		for(map<Direction, Outbounds>::const_iterator it = outbounds.begin(); it != outbounds.end(); it++)
		{
			if(removeEdge(e, it->first))
				return true;
		}
		return false;
	}

	bool Node::removeEdge(Node& n, Direction d)
	{
		Outbounds o = outbounds.at(d);
		for(size_t x = 0; x < o.edges.size(); ++x)
		{
			if(o.edges[x]->follow(this) == &n)
			{
				removeEdge(d, x);
			}
		}
		return false;
	}

	bool Node::removeEdge(Node& n)
	{
		for(map<Direction, Outbounds>::const_iterator it = outbounds.begin(); it != outbounds.end(); it++)
		{
			if(removeEdge(n, it->first))
				return true;
		}
		return false;
	}

	bool Node::removeEdge(Direction d, int num)
	{
		Outbounds o = outbounds.at(d);
		o.edges[num] = o.edges[o.edges.size() - 1];
		o.edges.pop_back();
		return true;
	}
}