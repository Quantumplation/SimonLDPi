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

	bool NaiveStrategy::isSwitched(Track* track, const std::string& input, Direction dir)
	{
		return track->getFirstNode(input)->edgeNotDefault(dir);
	}
	void NaiveStrategy::setSwitched(Track* track, const std::string& input, Direction dir, bool switched)
	{
		track->getFirstNode(input)->setCurrentEdge(dir, switched);
	}

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
				setSwitched(track, "K", RIGHT, !D);
				setSwitched(track, "O", RIGHT, (isSwitched(track, "K", RIGHT) && L) || (D && K) );
				if (B || ( D && (C || G) ) || (K && L) )
					return NOPATH;
				else return SUCCESS;
			}
			if (start == "L3")
			{
				setSwitched(track, "K", RIGHT, D && H);
				setSwitched(track, "R", RIGHT, D);
				setSwitched(track, "U", LEFT, isSwitched(track, "R", RIGHT) );
				setSwitched(track, "O", RIGHT, (isSwitched(track, "K", RIGHT) && K) || L);
				if ( (isSwitched(track, "K", RIGHT) && (C || G) ) || (K && L) )
					return NOPATH;
				else return SUCCESS;
			}

			if (start == "R1")
			{
				setSwitched(track, "D", LEFT, E || A);
				setSwitched(track, "G", LEFT, A || F);
				setSwitched(track, "C", RIGHT, isSwitched(track, "D", LEFT));
				if (J || (E && I) || (isSwitched(track, "D", LEFT) && F && (B || C)))
					return NOPATH;
				else return SUCCESS;
			}
			if (start == "R2")
			{
				setSwitched(track, "G", LEFT, B || C);
				setSwitched(track, "C", RIGHT, true);
				if (K || G || (isSwitched(track, "G", LEFT) && A))
					return NOPATH;
				else return SUCCESS;
			}
			if (start == "R3")
			{
				return NOPATH;
			}
		}
		if (mode == REVERSE)
		{
			if (start == "L1")
			{
				setSwitched(track, "A", RIGHT, A || (F && (E || J) ) || (G && J) );
				setSwitched(track, "K", RIGHT, !D);
				setSwitched(track, "C", RIGHT, E || J);
				setSwitched(track, "F", RIGHT,
					( !isSwitched(track, "A", RIGHT) && isSwitched(track, "C", RIGHT) )
					|| ( isSwitched(track, "A", RIGHT) && D && !(I || J) ) );
				setSwitched(track, "O", RIGHT, (isSwitched(track, "K", RIGHT) && L) || (D && K) );
				setSwitched(track, "D", LEFT, isSwitched(track, "C", RIGHT) || isSwitched(track, "F", RIGHT));
				if (isSwitched(track, "A", RIGHT) && (B || (D && (C || (!isSwitched(track, "F", RIGHT) && G) ) ) || (K && L) ) )
					return NOPATH;
				else return SUCCESS;
			}
			if (start == "L2")
			{
				setSwitched(track, "A", RIGHT, B || (C && D) || (K && L) );
				setSwitched(track, "K", RIGHT, !D);
				setSwitched(track, "C", RIGHT, E || J);
				setSwitched(track, "F", RIGHT,
					(isSwitched(track, "A", RIGHT) && (I || J) )
					|| (!isSwitched(track, "A", RIGHT) && G) );
				setSwitched(track, "O", RIGHT, (isSwitched(track, "K", RIGHT) && L) || (D && K) );
				setSwitched(track, "D", LEFT, isSwitched(track, "C", RIGHT) || !isSwitched(track, "A", RIGHT) );
				if (isSwitched(track, "A", RIGHT) && ( A || ( F && (E || J) ) || (G && J) ) )
					return NOPATH;
				else return SUCCESS;
			}
			if (start == "L3")
			{
				setSwitched(track, "K", RIGHT, (D && H) || (K && L) );
				setSwitched(track, "R", RIGHT, D);
				setSwitched(track, "F", RIGHT, G || (K && L) );
				setSwitched(track, "U", LEFT, isSwitched(track, "R", RIGHT));
				setSwitched(track, "O", RIGHT,
					(!isSwitched(track, "K", RIGHT) && L) || (isSwitched(track, "K", RIGHT) && K) );
				setSwitched(track, "D", LEFT, true);
				if (isSwitched(track, "K", RIGHT) && isSwitched(track, "F", RIGHT) && (I || J) )
					return NOPATH;
				else return SUCCESS;
			}

			if (start == "R1")
			{
				setSwitched(track, "D", LEFT, E || A);
				setSwitched(track, "G", LEFT, A || F);
				setSwitched(track, "C", RIGHT, isSwitched(track, "D", LEFT));
				setSwitched(track, "L", LEFT, true);
				if (J || (E && I) || (isSwitched(track, "D", LEFT) && isSwitched(track, "C", RIGHT) && C))
					return NOPATH;
				else return SUCCESS;
			}
			if (start == "R2")
			{
				setSwitched(track, "P", LEFT, !(D && H));
				setSwitched(track, "U", LEFT, D);
				setSwitched(track, "G", LEFT, C);
				setSwitched(track, "C", RIGHT, true);
				setSwitched(track, "R", RIGHT, isSwitched(track, "U", LEFT));
				setSwitched(track, "L", LEFT, !isSwitched(track, "P", LEFT));
				if (K || ( !isSwitched(track, "P", LEFT) && (G || C && (F || A) ) ) )
					return NOPATH;
				else return SUCCESS;
			}
			if (start == "R3")
			{
				setSwitched(track, "P", LEFT, D && H);
				setSwitched(track, "U", LEFT, D);
				setSwitched(track, "G", LEFT, C);
				setSwitched(track, "C", RIGHT, true);
				setSwitched(track, "R", RIGHT, isSwitched(track, "U", LEFT));
				setSwitched(track, "L", LEFT, isSwitched(track, "P", LEFT));
				if (L || ( isSwitched(track, "P", LEFT) && (G || C && (F || A) ) ) )
					return NOPATH;
				else return SUCCESS;
			}
		}
		return NOPATH;
	}
}