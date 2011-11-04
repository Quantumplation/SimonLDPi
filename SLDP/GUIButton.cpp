#include <string>
#include "GUIButton.h"
using namespace std;
namespace SLDP
{
	GUIButton::GUIButton(long x, long y, long width, long height, const string& label)
		: GUIBase(x, y, width, height, label) {}
	void GUIButton::draw(CDC context)
	{
		context.Rectangle(getMask());
		drawLabel(context, 0, 0);
	}
}