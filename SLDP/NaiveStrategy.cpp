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

		if (mode == NORMAL)
		{
			if (start == "L1")
			{
				return NOPATH;
			}
			if (start == "L2")
			{
				track->getFirstNode("K")->setCurrentEdge(RIGHT, !(obstacles["JK"] || obstacles["SU"]));
				if (obstacles["JK"] || (obstacles["SU"] && (obstacles["LM"] || obstacles["NO"]))) return NOPATH;
				else return SUCCESS;
			}
			if (start == "L3")
			{
				track->getFirstNode("K")->setCurrentEdge(RIGHT, obstacles["SU"] && !(obstacles["LM"] || obstacles["NO"]));
				track->getFirstNode("R")->setCurrentEdge(RIGHT, obstacles["SU"] && (obstacles["LM"] || obstacles["NO"]));
				track->getFirstNode("U")->setCurrentEdge(LEFT, track->getFirstNode("R")->edgeNotDefault(RIGHT));
				if (track->getFirstNode("R")->edgeNotDefault(LEFT) || obstacles["RU"]) return NOPATH;
				else return SUCCESS;
			}
		}
		if (mode == REVERSE)
		{
			if (start == "L1")
			{
				track->getFirstNode("A")->setCurrentEdge(RIGHT, obstacles["BC"] || obstacles["CD"] && obstacles["CF"]);
				track->getFirstNode("K")->setCurrentEdge(RIGHT, obstacles["LM"]);
				track->getFirstNode("C")->setCurrentEdge(RIGHT, obstacles["CD"]);
				track->getFirstNode("F")->setCurrentEdge(RIGHT, obstacles["BC"]);
				if (track->getFirstNode("A")->edgeNotDefault(RIGHT) && (obstacles["JK"] || obstacles["LM"] && obstacles["SU"])) return NOPATH;
				else return SUCCESS;
			}
			if (start == "L2")
			{
				track->getFirstNode("A")->setCurrentEdge(RIGHT, obstacles["JK"] || obstacles["LM"] && obstacles["SU"]);
				track->getFirstNode("K")->setCurrentEdge(RIGHT, obstacles["LM"]);
				track->getFirstNode("C")->setCurrentEdge(RIGHT, obstacles["CD"]);
				track->getFirstNode("F")->setCurrentEdge(RIGHT, !(obstacles["JK"] || obstacles["LM"]) && obstacles["NO"]);
				if (track->getFirstNode("A")->edgeNotDefault(RIGHT) && (obstacles["BC"] || obstacles["CD"] && obstacles["CF"])) return NOPATH;
				else return SUCCESS;
			}
			if (start == "L3")
			{
				track->getFirstNode("K")->setCurrentEdge(RIGHT, obstacles["SU"] && !(obstacles["LM"]));
				track->getFirstNode("R")->setCurrentEdge(RIGHT, obstacles["SU"]);
				track->getFirstNode("F")->setCurrentEdge(RIGHT, track->getFirstNode("K")->edgeNotDefault(RIGHT));
				track->getFirstNode("U")->setCurrentEdge(LEFT, obstacles["SU"]);
				if (obstacles["LM"] && obstacles["SU"] && obstacles["RU"]) return NOPATH;
				else return SUCCESS;
			}
		}
		return NOPATH;
	}

	void NaiveStrategy::Destruct()
	{
	}
}