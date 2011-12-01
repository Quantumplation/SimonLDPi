#include <string>
#include "NIDAQWrapper.h"
#include "GUIButton.h"
#include "GUITrack.h"
#include "Strategy.h"
#include "Edge.h"
#include "NaiveStrategy.h"
#include "DijkstraStrategy.h"
using namespace std;
namespace SLDP
{
	GUIButton::GUIButton(long x, long y, long width, long height, const string& label, GUITrack* track)
		: GUIBase(x, y, width, height, label), track(track) {}
	void GUIButton::draw(CDC& context) const
	{
		context.Rectangle(getMask());
		drawLabel(context, 0, -8);
	}

	ModeButton::ModeButton(long x, long y, long width, long height, GUITrack* track)
		: GUIButton(x, y, width, height, "Run Mode", track) {}


	void ModeButton::onClick()
	{
		track->setMode(!track->getMode());
		if (track->getMode() == false)
			track->saveToFile("junk.xml");
		setLabel(track->getMode() ? "Modify Mode" : "Run Mode");
	}

	ReadButton::ReadButton(long x, long y, long width, long height, GUITrack* track, NIDAQWrapper* wrapper)
		: GUIButton(x, y, width, height, "Read From DAQ", track), wrapper(wrapper) {}

	void ReadButton::onClick()
	{
		wrapper->GetPhysical(track);
	}

	WriteButton::WriteButton(long x, long y, long width, long height, GUITrack* track, NIDAQWrapper* wrapper)
		: GUIButton(x, y, width, height, "Write To DAQ", track), wrapper(wrapper) {}

	void WriteButton::onClick()
	{
		wrapper->MeHearYourBodyTalk(track);
	}

	SaveButton::SaveButton(long x, long y, long width, long height, GUITrack* track)
		: GUIButton(x, y, width, height, "Save To File", track) {}

	void SaveButton::onClick()
	{
		track->saveToFile("input.xml");
	}

	LoadButton::LoadButton(long x, long y, long width, long height, GUITrack* track)
		: GUIButton(x, y, width, height, "Load From File", track) {}

	void LoadButton::onClick()
	{
		track->loadFromFile("input.xml");
	}

	TrainModeButton::TrainModeButton(long x, long y, long width, long height, TrainMode* mode)
		: GUIButton(x, y, width, height, "Normal Mode", NULL), currentMode(mode) {}

	void TrainModeButton::onClick()
	{
		*currentMode = (*currentMode == NORMAL) ? REVERSE : NORMAL;
		setLabel((*currentMode == NORMAL) ? "Normal Mode" : "Reverse Mode");
	}

	TrainStartButton::TrainStartButton(long x, long y, long width, long height, GUITrack* track, TrainMode* mode, GUINode* start)
		: GUIButton(x, y, width, height, "Run From " + start->getLabel(), NULL), track(track), mode(mode), start(start) {}

	void TrainStartButton::onClick()
	{
		Track runTrack;
		runTrack.loadFromFile("junk.xml");
//		NaiveStrategy strat(*mode, start->getLabel());
		std::vector<std::string> ends;

		std::string cross[] = {"AJ", "IB", "KS", "QL", "REMPTY5", "FourEMPTY11", "FN", "MG", "OW", "VP", "EMPTY6Six", "EMPTY7Eight"};
		std::string zeroes[] = {"EMPTY3EMPTY4", "EMPTY4Four", "FourEMPTY19", "EMPTY10Eight", "WEMPTY8", "EMPTY9R3", "REMPTY5", "EMPTY2Q", "EMPTY1EMPTY2", "FourEMPTY11", "EMPTY7Eight"};
		runTrack.getFirstEdge("QThree")->setWeight(0.5);
		runTrack.getFirstEdge("RS")->setWeight(0.5);
		for(size_t x = 0;  x < 12; x++)
		{
			runTrack.getFirstEdge(cross[x])->setWeight(20);
		}
		for(size_t x = 0; x < 11; x++)
		{
			runTrack.getFirstEdge(zeroes[x])->setWeight(0);
		}
		track->modifyTrack(runTrack);

		if(*mode == NORMAL && (start->getLabel() == "L1" || start->getLabel() == "L2" || start->getLabel() == "L3"))
		{
			if(start->getLabel() == "L1")
			{
				track->setNopath(true);
				return;
			}
			else
			{
				runTrack.getFirstEdge("IB")->setWeight(INT_MAX);
				runTrack.getFirstEdge("MG")->setWeight(INT_MAX);
				runTrack.getFirstEdge("AJ")->setWeight(INT_MAX);
				runTrack.getFirstEdge("FN")->setWeight(INT_MAX);
				ends.push_back("R2");
				ends.push_back("R3");
			}
		}
		else if(*mode == NORMAL)
		{
			if(start->getLabel() == "R3")
			{
				track->setNopath(true);
				return;
			}
			else
			{
				runTrack.getFirstEdge("VP")->setWeight(INT_MAX);
				runTrack.getFirstEdge("QL")->setWeight(INT_MAX);
				runTrack.getFirstEdge("OW")->setWeight(INT_MAX);
				runTrack.getFirstEdge("KS")->setWeight(INT_MAX);
				ends.push_back("L1");
				ends.push_back("L2");
			}
		}
		else
		{
			if(start->getLabel() == "L1" || start->getLabel() == "L2" || start->getLabel() == "L3")
			{
				ends.push_back("R1");
				ends.push_back("R2");
				ends.push_back("R3");
			}
			else
			{
				ends.push_back("L1");
				ends.push_back("L2");
				ends.push_back("L3");
			}
		}
		DijkstraStrategy strat(start->getLabel(), ends);
		track->setNopath((strat.Execute(&runTrack) == NOPATH));
		track->readFromTrack(runTrack);
	}

}