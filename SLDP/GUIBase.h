#pragma once
#include <string>
#include "Win32++\wincore.h"
namespace SLDP
{
	class GUIBase
	{
	public:
		GUIBase();
		GUIBase(long x, long y, long width, long height);
		GUIBase(long x, long y, long width, long height, const std::string& newLabel);
		virtual void draw(CDC context) const = 0;
		bool contains(POINT p) const;
		long getX() const;
		long getY() const;
		long getLeft() const;
		long getRight() const;
		long getTop() const;
		long getBottom() const;
		RECT getMask() const;
		bool readyToDelete() const;
		void setLocation(long x, long y);
		void setLocation(POINT p);
		std::string getLabel() const;
		void setLabel(const std::string& newLabel);
		void setBeingRelabeled(bool relabled);
		void addCharToLabel(wchar_t newChar);
		void removeCharFromLabel();
		void drawLabel(CDC context, long x, long y) const;
		virtual void markForDeletion();
	private:
		RECT mask;
		long wRadius, hRadius;
		bool relabeling;
		wchar_t* label;
		size_t labelSize;
		bool deleteThis;
	};
}