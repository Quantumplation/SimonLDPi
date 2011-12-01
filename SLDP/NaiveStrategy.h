#pragma once
#include <string>
#include "Node.h"
#include "Strategy.h"

namespace SLDP
{
	class Track;

	enum TrainMode
	{
		NORMAL = 0,
		REVERSE = 1,
	};

	class NaiveStrategy : public Strategy
	{
	public:
		NaiveStrategy(TrainMode mode, const std::string& start);
		virtual Result Execute(Track* track);
	private:
		bool isSwitched(Track* track, const std::string& input, Direction dir);
		void setSwitched(Track* track, const std::string& input, Direction dir, bool switched);
		TrainMode mode;
		std::string start;
	};
}