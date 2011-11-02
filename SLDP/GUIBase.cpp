#include <vector>
#include "Win32++\WinCore.h"
#include "GUIBase.h"
using namespace std;

namespace SLDP
{
	GUIBase::GUIBase() : wRadius(0), hRadius(0), deleteThis(false)
	{
		mask.left = 0;
		mask.right = 0;
		mask.top = 0;
		mask.bottom = 0;
	}
	GUIBase::GUIBase(long x = 320, long y = 240, long width = 16, long height = 16)
		: wRadius(width / 2), hRadius(height / 2), deleteThis(false)
	{
		mask.left = x;
		mask.right = x + width;
		mask.top = y;
		mask.bottom = y + height;
	}
	bool GUIBase::contains(POINT p) const
	{
		return (mask.left < p.x && p.x < mask.right && mask.top < p.y && p.y < mask.bottom);
	}
	long GUIBase::getX() const { return mask.left + wRadius; }
	long GUIBase::getY() const { return mask.top + hRadius; }
	long GUIBase::getLeft() const { return mask.left; }
	long GUIBase::getRight() const { return mask.right; }
	long GUIBase::getTop() const { return mask.top; }
	long GUIBase::getBottom() const { return mask.bottom; }
	RECT GUIBase::getMask() const { return mask; }
	bool GUIBase::readyToDelete() const { return deleteThis; }
	void GUIBase::setLocation(long x, long y)
	{
		mask.left = x - wRadius;
		mask.right = x + wRadius;
		mask.top = y - hRadius;
		mask.bottom = y + hRadius;
	}
	void GUIBase::setLocation(POINT p)
	{
		setLocation(p.x, p.y);
	}
	void GUIBase::markForDeletion()
	{
		deleteThis = true;
	}
}