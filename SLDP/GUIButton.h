#pragma once
#include <string>
#include "GUIBase.h"
#include "GUITrack.h"
namespace SLDP
{
	class GUITrack;
	class GUIButton : public GUIBase
	{
	public:
		GUIButton(long x, long y, long width, long height, const std::string& label, GUITrack* track);
		virtual void onClick(long x, long y) = 0;
		void draw(CDC& context) const;
	protected:
		GUITrack* track;
	};

	class ModeButton : public GUIButton
	{
	public:
		ModeButton(long x, long y, long width, long height, GUITrack* track);
		void onClick(long x, long y);
	};

	class ReadButton : public GUIButton
	{
	public:
		ReadButton(long x, long y, long width, long height, GUITrack* track);
		void onClick(long x, long y);
	};

	class StrategyButton : public GUIButton
	{
	public:
		StrategyButton(long x, long y, long width, long height, GUITrack* track);
		void onClick(long x, long y);
	};
}

