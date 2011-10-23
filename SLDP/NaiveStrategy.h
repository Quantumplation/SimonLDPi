#pragma once
#include <string>
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
		TrainMode mode;
		std::string start;
	};
}