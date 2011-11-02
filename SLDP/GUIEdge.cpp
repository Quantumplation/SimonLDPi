#include "GUIEdge.h"
#include <string>
using namespace std;
namespace SLDP
{
	GUIEdge::GUIEdge(GUINode* side1, GUINode* side2)
		: GUIBase(
			min(side1->getX(), side2->getX()),
			min(side1->getY(), side2->getY()),
			abs(side1->getX() - side2->getX()),
			abs(side1->getY() - side2->getY())),
		side1(side1), side2(side2)
	{
		side1->addEdge(this);
		side2->addEdge(this);
	}
	bool GUIEdge::isDead() const
	{
		return !(side1 || side2);
	}
	GUINode* GUIEdge::follow(const GUINode* caller) const
	{
		if (side1 == caller)
			return side2;
		else
			return side1;
	}
	void GUIEdge::draw(CDC context) const
	{
		context.MoveTo(side1->getX(), side1->getY());
		context.LineTo(side2->getX(), side2->getY());
	}
}