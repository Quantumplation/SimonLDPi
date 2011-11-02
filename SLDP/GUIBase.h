#pragma once
#include "Win32++\wincore.h"
namespace SLDP
{
	class GUIBase
	{
	public:
		GUIBase();
		GUIBase(long x, long y, long width, long height);
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
		virtual void markForDeletion();
	private:
		RECT mask;
		long wRadius, hRadius;
		bool deleteThis;
	};
}