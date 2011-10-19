#pragma once
#include "Strategy.h"

namespace SLDP
{
	class Track;

	class NaiveStrategy : public Strategy
	{
		virtual Result Execute(Track* track);
		virtual void Destruct();
	};
}