#pragma once
#include <string>
#include "GUIBase.h"
namespace SLDP
{
	class GUIButton : public GUIBase
	{
	public:
		GUIButton(long x, long y, long width, long height, const std::string& label);
		virtual void GUIButton::onClick(long x, long y) = 0;
		void draw(CDC context);
	};
}

