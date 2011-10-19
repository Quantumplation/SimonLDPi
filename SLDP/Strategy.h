#pragma once

#include "Track.h"

namespace SLDP
{
	class Strategy
	{
		virtual void Execute(Track* track) = 0;
	};
}