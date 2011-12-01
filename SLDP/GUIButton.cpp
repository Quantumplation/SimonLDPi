#include <string>
#include "NIDAQWrapper.h"
#include "GUIButton.h"
#include "GUITrack.h"
#include "Strategy.h"
#include "NaiveStrategy.h"
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
		track->modifyTrack(runTrack);
		NaiveStrategy strat(*mode, start->getLabel());
		track->setNopath((strat.Execute(&runTrack) == NOPATH));
		track->readFromTrack(runTrack);
	}

}