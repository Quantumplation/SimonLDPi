#pragma once
#include "Strategy.h"

namespace SLDP
{
	class Track;

	class NaiveStrategy : public Strategy
	{
		virtual void Execute(Track* track);
		virtual void Destruct();
	};
}