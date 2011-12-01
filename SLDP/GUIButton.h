#pragma once
#include <string>
#include "GUIBase.h"
#include "GUITrack.h"
#include "Strategy.h"
#include "NaiveStrategy.h"
namespace SLDP
{
	class GUITrack;
	class NIDAQWrapper;
	class GUIButton : public GUIBase
	{
	public:
		GUIButton(long x, long y, long width, long height, const std::string& label, GUITrack* track);
		virtual void onClick() = 0;
		void draw(CDC& context) const;
	protected:
		GUITrack* track;
	};

	class ModeButton : public GUIButton
	{
	public:
		ModeButton(long x, long y, long width, long height, GUITrack* track);
		void onClick();
	};

	class ReadButton : public GUIButton
	{
	public:
		ReadButton(long x, long y, long width, long height, GUITrack* track, NIDAQWrapper* wrapper);
		void onClick();
	private:
		NIDAQWrapper* wrapper;
	};

	class WriteButton : public GUIButton
	{
	public:
		WriteButton(long x, long y, long width, long height, GUITrack* track, NIDAQWrapper* wrapper);
		void onClick();
	private:
		NIDAQWrapper* wrapper;
	};

	class SaveButton : public GUIButton
	{
	public:
		SaveButton(long x, long y, long width, long height, GUITrack* track);
		void onClick();
	};

	class LoadButton : public GUIButton
	{
	public:
		LoadButton(long x, long y, long width, long height, GUITrack* track);
		void onClick();
	};

	class TrainModeButton : public GUIButton
	{
	public:
		TrainModeButton(long x, long y, long width, long height, TrainMode* mode);
		void onClick();
	private:
		TrainMode* currentMode;
	};

	class TrainStartButton : public GUIButton
	{
	public:
		TrainStartButton(long x, long y, long width, long height, GUITrack* track, TrainMode* mode, GUINode* start);
		void onClick();
	private:
		GUITrack* track;
		TrainMode* mode;
		GUINode* start;
	};
}

