#include <iostream>
#include <string>
#include <vector>
#include "NaiveStrategy.h"
#include "Node.h"
#include "Edge.h"
#include "Track.h"
using namespace std;

namespace SLDP
{
	NaiveStrategy::NaiveStrategy(TrainMode mode, const string& start)
		: mode(mode), start(start) {}
	Result NaiveStrategy::Execute(Track* track)
	{
		const string obstacleLabels[] = {"L1A", "AB", "BC", "CD", "DR1", "CF", "FG", "GD", "AJ", "IB", "FN", "MG", "L2I", "IJ", "JK", "KL", "LM", "MN", "NO", "OP", "PR2", "KS", "QL", "OW", "VX", "L3Q", "QR", "RS", "SU", "UV", "VW", "WR3", "RU"};

		map<string, bool> obstacles;

		for(size_t x = 0; x < obstacleLabels->length(); x++)
		{
			obstacles[obstacleLabels[x]] = false;
		}

		vector<Edge*> blocked = track->getEdgesAll(EDGE_IMPASSABLE);

		for(size_t x = 0; x < blocked.size(); x++)
		{
			obstacles[ blocked[x]->getLabel() ] = true;
		}
		
		bool A = obstacles["BC"];
		bool B = obstacles["JK"];
		bool C = obstacles["LM"];
		bool D = obstacles["SU"];
		bool E = obstacles["CD"];
		bool F = obstacles["CF"];
		bool G = obstacles["NO"];
		bool H = obstacles["RU"];
		bool I = obstacles["GD"];
		bool J = obstacles["DR1"];
		bool K = obstacles["PR2"];
		bool L = obstacles["WR3"];

		if (mode == NORMAL)
		{
			if (start == "L1")
			{
				return NOPATH;
			}
			if (start == "L2")
			{
				track->getFirstNode("K")->setCurrentEdge(RIGHT, !D);
				track->getFirstNode("O")->setCurrentEdge(RIGHT, (track->getFirstNode("K")->edgeNotDefault(RIGHT) && L) || (D && K) );
				if (B || ( D && (C || G) ) || (K && L) ) return NOPATH;
				else return SUCCESS;
			}
			if (start == "L3")
			{
				track->getFirstNode("K")->setCurrentEdge(RIGHT, D && H);
				track->getFirstNode("R")->setCurrentEdge(RIGHT, D);
				track->getFirstNode("U")->setCurrentEdge(LEFT, track->getFirstNode("R")->edgeNotDefault(RIGHT) );
				track->getFirstNode("O")->setCurrentEdge(RIGHT, (track->getFirstNode("K")->edgeNotDefault(RIGHT) && K) || L);
				if ( (track->getFirstNode("K")->edgeNotDefault(RIGHT) && (C || G) ) || (K && L) ) return NOPATH;
				else return SUCCESS;
			}
		}
		if (mode == REVERSE)
		{
			if (start == "L1")
			{
				track->getFirstNode("A")->setCurrentEdge(RIGHT, A || (F && (E || J) ) || (G && J) );
				track->getFirstNode("K")->setCurrentEdge(RIGHT, !D);
				track->getFirstNode("C")->setCurrentEdge(RIGHT, E || J);
				track->getFirstNode("F")->setCurrentEdge(RIGHT,
					( !track->getFirstNode("A")->edgeNotDefault(RIGHT) && track->getFirstNode("C")->edgeNotDefault(RIGHT) )
					|| ( track->getFirstNode("A")->edgeNotDefault(RIGHT) && D && !(I || J) ) );
				track->getFirstNode("O")->setCurrentEdge(RIGHT,
					(track->getFirstNode("K")->edgeNotDefault(RIGHT) && L) || (D && K) );
				track->getFirstNode("D")->setCurrentEdge(LEFT,
					track->getFirstNode("C")->edgeNotDefault(RIGHT) || track->getFirstNode("F")->edgeNotDefault(RIGHT));
				if (track->getFirstNode("A")->edgeNotDefault(RIGHT)
					&& (B || (D && (C || (!track->getFirstNode("F")->edgeNotDefault(RIGHT) && G) ) ) || (K && L) ) )
					return NOPATH;
				else return SUCCESS;
			}
			if (start == "L2")
			{
				track->getFirstNode("A")->setCurrentEdge(RIGHT, B || (C && D) || (K && L) );
				track->getFirstNode("K")->setCurrentEdge(RIGHT, !D);
				track->getFirstNode("C")->setCurrentEdge(RIGHT, E);
				track->getFirstNode("F")->setCurrentEdge(RIGHT,
					(track->getFirstNode("A")->edgeNotDefault(RIGHT) && (I || J) )
					|| (!track->getFirstNode("A")->edgeNotDefault(RIGHT) && G) );
				track->getFirstNode("O")->setCurrentEdge(RIGHT, (track->getFirstNode("K")->edgeNotDefault(RIGHT) && L) || (D && K) );
				track->getFirstNode("D")->setCurrentEdge(LEFT,
					track->getFirstNode("C")->edgeNotDefault(RIGHT) || !track->getFirstNode("A")->edgeNotDefault(RIGHT) );
				if (track->getFirstNode("A")->edgeNotDefault(RIGHT) && ( A || ( F && (E || J) ) || (G && J) ) )
					return NOPATH;
				else return SUCCESS;
			}
			if (start == "L3")
			{
				track->getFirstNode("K")->setCurrentEdge(RIGHT, (D && H) || (K && L) );
				track->getFirstNode("R")->setCurrentEdge(RIGHT, D);
				track->getFirstNode("F")->setCurrentEdge(RIGHT, G || (K && L) );
				track->getFirstNode("U")->setCurrentEdge(LEFT, track->getFirstNode("R")->edgeNotDefault(RIGHT));
				track->getFirstNode("O")->setCurrentEdge(RIGHT,
					(!track->getFirstNode("K")->edgeNotDefault(RIGHT) && L) || (track->getFirstNode("K")->edgeNotDefault(RIGHT) && K) );
				track->getFirstNode("D")->setCurrentEdge(LEFT, true);
				if (track->getFirstNode("K")->edgeNotDefault(RIGHT) && track->getFirstNode("F")->edgeNotDefault(RIGHT) && (I || J) )
					return NOPATH;
				else return SUCCESS;
			}
		}
		return NOPATH;
	}
}