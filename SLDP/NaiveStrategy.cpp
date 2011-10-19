#include <vector>
#include "NaiveStrategy.h"
#include "Node.h"
#include "Edge.h"
#include "Track.h"
using namespace std;

namespace SLDP
{
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

		if (!(obstacles["JK"] || obstacles["SU"]))
		{
			// derp
		}
		return SUCCESS;
	}

	void NaiveStrategy::Destruct()
	{
	}
}