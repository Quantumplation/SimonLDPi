#include "GUIEdge.h"
#include <string>
using namespace std;
namespace SLDP
{
	GUIEdge::GUIEdge(GUINode* side1, GUINode* side2, const string& label)
		: GUIBase(
			min(side1->getX(), side2->getX()),
			min(side1->getY(), side2->getY()),
			abs(side1->getX() - side2->getX()),
			abs(side1->getY() - side2->getY()), label),
		side1(side1), side2(side2), visible(true), blocked(false)
	{
		side1->addEdge(this);
		side2->addEdge(this);
	}

	bool GUIEdge::isDead() const { return !(side1 || side2); }
	bool GUIEdge::isBlocked() const { return blocked; }

	bool GUIEdge::contains(POINT p) const
	{
        POINT ab = {side2->getX() - side1->getX(), side2->getY() - side1->getY()};
        POINT am = {p.x - side1->getX(), p.y - side1->getY()};
        double length = sqrt(double(ab.x * ab.x + ab.y * ab.y));
		const float radius = 16;
        double dot = (am.x * ab.x + am.y * ab.y) / length;
        if(dot > -radius && dot < length+radius)
        {
                POINT abp = {-ab.y, ab.x};
                dot = (am.x * abp.x + am.y * abp.y) / length;
                if(abs(dot)<radius)
                        return true;
        }
        return false;
	}

	bool GUIEdge::isPartOfSwitch() const
	{
		if(side2->isSwitchableLeft() || side1->isSwitchableRight())
			return true;
		return false;
	}
//		if (p.x < (getRight() + 8) && p.x > (getLeft() - 8) && p.y < (getBottom() + 8) && p.y > (getTop() - 8))
/*		{
			POINT point1, point2;
			point1.x = side1->getX();
			point1.y = side1->getY();
			point2.x = side2->getX();
			point2.y = side2->getY();
			return intersection(point1, point2, p, 100);
			//double slopeToClick = side1->getTop() < side2->getTop() ? p.y - getTop() : p.y - getBottom();
			//slopeToClick /= p.x - getLeft();
			//double slopeOfLine = side1->getTop() < side2->getTop() ? getBottom() - getTop() : getTop() - getBottom();
			//slopeOfLine /= getRight() - getLeft();
			//return (abs(slopeToClick - slopeOfLine) < 10.0);
		}
		return false;
	}*/

	void GUIEdge::setVisible(bool isVisible) { visible = isVisible; }

	GUINode* GUIEdge::follow(const GUINode* caller) const
	{
		if (side1 == caller)
			return side2;
		else
			return side1;
	}
	void GUIEdge::draw(CDC& context) const
	{
		context.CreatePen(PS_SOLID, 1, visible ? RGB(0, 0, 0) : RGB(223, 223, 223));
		context.MoveTo(side1->getX(), side1->getY());
		context.LineTo(side2->getX(), side2->getY());
		if (blocked)
			context.Ellipse(getX() - 16, getY() - 16, getX() + 16, getY() + 16);
		drawLabel(context, 0, 16);
		context.CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
	}

	void GUIEdge::alterBlocked() { blocked = !blocked; }
}