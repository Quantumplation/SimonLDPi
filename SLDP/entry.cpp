//#include "../include/NIDAQmx.h"
#include "NIDAQWrapper.h"
#include <map>
#include <climits>
#include <iostream> // REMOVE THIS
#include <Windows.h>
#include <string>
#include <vector>
#include "Track.h"
#include "Edge.h"
#include "Node.h"
#include "NaiveStrategy.h"
#include "DijkstraStrategy.h"
#include "../include/curses.h"
using namespace std;

#define PHYSICAL &t
#define YOUR_BODY_TALK &t

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
	if (input == "I" || input == "i") return "GD";
	if (input == "J" || input == "j") return "DR1";
	if (input == "K" || input == "k") return "PR2";
	if (input == "L" || input == "l") return "WR3";
	if (input == "NIDAQ" || input == "nidaq") return "NIDAQ";
	return "";
}

int main()
{
	SLDP::NIDAQWrapper lets;
	SLDP::NIDAQWrapper* let = &lets;
	lets.Initialize();

	bool NIDAQ = false;
	SLDP::Track t;
	t.loadFromFile("input.xml");
	while(true)
	{
		string input;
		cout << "Enter an obstacle (letters A through L): ";
		cin >> input;
		if (translate(input) == "") break;
		if (translate(input) == "NIDAQ")
		{
			NIDAQ = true;
			break;
		}
		t.getFirstEdge(translate(input))->setFlags(SLDP::EDGE_IMPASSABLE);
	}
	if (NIDAQ)
	{
		string s;
		cout << "Press enter to lock in the values..." << endl;
		getline(cin, s);
		getline(cin, s);
		lets.GetPhysical(PHYSICAL);
		lets.frozen = true;
	}
	string track;
	cout << "Starting track (L1, L2, L3, R1, R2, or R3): ";
	cin >> track;
	if (track[0] == 'l') track[0] = 'L';
	if (track[0] == 'r') track[0] = 'R';
	if ((track[0] != 'L' && track[0] != 'R') || (track[1] != '1' && track[1] != '2' && track[1] != '3'))
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
		cout << "X Switch 7: " << t.getFirstNode("O")->edgeNotDefault(SLDP::RIGHT) << " X\n";
		cout << "X Switch 8: " << t.getFirstNode("D")->edgeNotDefault(SLDP::LEFT) << " X\n";
		for (size_t i = 0; i < 15; ++i) { cout << "X"; }
		cout << endl;
		let->MeHearYourBodyTalk(YOUR_BODY_TALK);
		let->frozen = false;
	}
	string s;
	cin >> s;
	return 0;
}