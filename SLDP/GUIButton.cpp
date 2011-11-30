#include <string>
#include "GUIButton.h"
#include "GUITrack.h"
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
		: GUIButton(x, y, width, height, "Run", track) {}


	void ModeButton::onClick(long x, long y)
	{
		track->setMode(!track->getMode());
		//if (track->getMode() == false)
		//	track->saveToFile("junk.xml");
		setLabel(track->getMode() ? "Modify" : "Run");
	}

	ReadButton::ReadButton(long x, long y, long width, long height, GUITrack* track)
		: GUIButton(x, y, width, height, "Read From DAQ", track) {}

	void ReadButton::onClick(long x, long y)
	{
		
	}

	StrategyButton::StrategyButton(long x, long y, long width, long height, GUITrack* track)
		: GUIButton(x, y, width, height, "Use Strategy", track) {}

	void StrategyButton::onClick(long x, long y)
	{
	}
}