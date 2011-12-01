#include <string>
#include "Win32++\WinCore.h"
#include "GUIBase.h"
using namespace std;

namespace SLDP
{
	GUIBase::GUIBase() : wRadius(0), hRadius(0), deleteThis(false),
		relabeling(false), label(new wchar_t[10]), labelSize(0), showLabel(true)
	{
		mask.left = 0;
		mask.right = 0;
		mask.top = 0;
		mask.bottom = 0;
	}
	GUIBase::GUIBase(long x, long y, long width, long height)
		: wRadius(width / 2), hRadius(height / 2), deleteThis(false),
		  relabeling(false), label(new wchar_t[10]), labelSize(0), showLabel(true)
	{
		mask.left = x;
		mask.right = x + width;
		mask.top = y;
		mask.bottom = y + height;
		label[0] = '\0';
	}
	GUIBase::GUIBase(long x, long y, long width, long height, const string& newLabel)
		: wRadius(width / 2), hRadius(height / 2), deleteThis(false),
		relabeling(false), label(new wchar_t[10]), showLabel(true)
	{
		mask.left = x;
		mask.right = x + width;
		mask.top = y;
		mask.bottom = y + height;
		setLabel(newLabel);
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
	long GUIBase::getWidth() const { return mask.right - mask.left; }
	long GUIBase::getHeight() const { return mask.bottom - mask.top; }
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
	string GUIBase::getLabel() const
	{
		string result = "";
		for (size_t i = 0; i < labelSize; ++i)
			result += (char)label[i];
		return result;
	}
	void GUIBase::setLabel(const string& newLabel)
	{
		const char* toConvert = newLabel.c_str();
		labelSize = newLabel.size();
		for (size_t i = 0; i < labelSize; ++i)
			label[i] = (wchar_t)toConvert[i];
		relabeling = false;
		label[labelSize] = '\0';
	}
	void GUIBase::setBeingRelabeled(bool relabeled)
	{
		relabeling = relabeled;
		if (relabeled) label[labelSize] = '|';
		else label[labelSize] = '\0';
	}
	void GUIBase::addCharToLabel(wchar_t newChar)
	{
		if (labelSize > 9) return;
		else
		{
			label[labelSize++] = newChar;
			label[labelSize] = '|';
		}
	}
	void GUIBase::removeCharFromLabel()
	{
		if (labelSize < 1) return;
		else
			label[--labelSize] = '|';
	}

	void GUIBase::drawLabel(CDC& context, long shiftX, long shiftY) const
	{
		if(showLabel)
		{
			if (relabeling)
				context.TextOutW(getX() - labelSize * 4 + shiftX, getY() + shiftY, label, labelSize + 1);
			else
				context.TextOutW(getX() - labelSize * 4 + shiftX, getY() + shiftY, label);
		}
	}
	void GUIBase::markForDeletion()
	{
		deleteThis = true;
	}
	void GUIBase::SetLabelVisible(bool val) { showLabel = val; }
	bool GUIBase::GetLabelVisible() { return showLabel; }
}