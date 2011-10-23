#pragma once

#include "Track.h"

namespace SLDP
{
	enum Result
	{
		SUCCESS = 0,
		NOPATH = 1,
	};

	class Strategy
	{
		virtual Result Execute(Track* track) = 0;
	};
}