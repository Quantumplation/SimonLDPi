#include "../include/NIDAQmx.h"

#include <map>
#include <iostream> // REMOVE THIS
#include <string>
#include <vector>
#include "Track.h"
#include "Edge.h"
#include "Node.h"
#include "NaiveStrategy.h"
using namespace std;

string translate(const string& input)
{
	if (input == "A" || input == "a") return "BC";
	if (input == "B" || input == "b") return "JK";
	if (input == "C" || input == "c") return "LM";
	if (input == "D" || input == "d") return "SU";
	if (input == "E" || input == "e") return "CD";
	if (input == "F" || input == "f") return "CF";
	if (input == "G" || input == "g") return "NO";
	if (input == "H" || input == "h") return "RU";
	return "";
}

int main()
{
	if(true)
	{
		SLDP::Track t;
		t.loadFromFile("input.xml");
		while(true)
		{
			string input;
			cout << "Enter an obstacle (letters A through H): ";
			cin >> input;
			if (translate(input) == "") break;
			t.getFirstEdge(translate(input))->setFlags(SLDP::EDGE_IMPASSABLE);
		}
		string track;
		cout << "Starting track (L1, L2, L3, R1, R2, or R3: ";
		cin >> track;
		if (track == "l1") track = "L1";
		if (track == "l2") track = "L2";
		if (track == "l3") track = "L3";
		if (track == "r1") track = "R1";
		if (track == "r2") track = "R2";
		if (track == "r3") track = "R3";
		if (track != "L1" && track != "L2" && track != "L3" && track != "R1" && track != "R2" && track != "R3")
		{
			cout << "You probably thought this was going to work.  Nope, Chuck Testa!\n";
			track = "L1";
		}
		string mode;
		cout << "Normal or Reverse? ";
		cin >> mode;
		SLDP::NaiveStrategy* ns;
		if (mode == "Normal" || mode == "normal" || mode == "N" || mode == "n")	ns = new SLDP::NaiveStrategy(SLDP::NORMAL, track);
		else if (mode == "Reverse" || mode == "reverse" || mode == "R" || mode == "r") ns = new SLDP::NaiveStrategy(SLDP::REVERSE, track);
		else
		{
			cout << "Fuck you, it's normal.";
			ns = new SLDP::NaiveStrategy(SLDP::NORMAL, track);
		}
		if (ns->Execute(&t))
			cout << "\nNo Path\n";
		else
		{
			cout << "\nSuccess!\n";
			for (size_t i = 0; i < 15; ++i) { cout << "X"; }
			cout << "\nX Switch 1: " << t.getFirstNode("A")->edgeNotDefault(SLDP::RIGHT) << " X\n";
			cout << "X Switch 2: " << t.getFirstNode("K")->edgeNotDefault(SLDP::RIGHT) << " X\n";
			cout << "X Switch 3: " << t.getFirstNode("R")->edgeNotDefault(SLDP::RIGHT) << " X\n";
			cout << "X Switch 4: " << t.getFirstNode("C")->edgeNotDefault(SLDP::RIGHT) << " X\n";
			cout << "X Switch 5: " << t.getFirstNode("F")->edgeNotDefault(SLDP::RIGHT) << " X\n";
			cout << "X Switch 6: " << t.getFirstNode("U")->edgeNotDefault(SLDP::LEFT) << " X\n";
			for (size_t i = 0; i < 15; ++i) { cout << "X"; }
			cout << endl;
		}
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