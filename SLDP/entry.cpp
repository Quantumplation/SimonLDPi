#include "../include/NIDAQmx.h"

#include <map>
#include <string>
#include <vector>
#include "Track.h"
#include "Edge.h"
#include "Node.h"
using namespace std;

int main()
{
	if(true)
	{
		SLDP::Track t;
		t.loadFromFile("input.xml");
		vector<SLDP::Node*> nodes = t.getNodesAll(SLDP::NODE_TERMINAL | SLDP::NODE_SWITCH);
		SLDP::Node* a = t.getFirstNode("A");
		a->edgeNotDefault(SLDP::RIGHT);
	}
	/*
	map<string, bool> vars;

	bool output = ( vars["L"] || vars["K"] ) && vars["M"];



	vector<Edge*> impassable = track.getEdges(EdgeFlags::IMPASSABLE);
	for(size_t x = 0; x < impassable.size(); ++x)
	{
		vars[ impassable[x]->getLabel() ] = true;
	}

	vector<Edge*> passable = track.getEdges(EdgeFlags::PASSABLE);
	for(size_t x = 0; x < passable.size(); ++x)
	{
		vars[ passable[x]->getLabel() ] = false;
	}

	bool s1 = vars["L"] || vars["K"] && vars["M"];
	bool s2 = vars["L"];

	track.getNodes("S1")->setDestination("S3");
	track.getNodes("S2")->setDestination("S4");
	*/
	return 0;
}