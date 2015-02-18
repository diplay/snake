#include "Bonus.h"

Bonus::Bonus(BONUS_TYPE t, Vector2 pos)
{
	view = new ColorRectSprite();
	view->setWidth(SIZE);
	view->setHeight(SIZE);
	type = t;
	switch(t)
	{
	case BONUS_EAT:
		view->setColor(Color::Khaki);
		break;
	case BONUS_HALF:
		view->setColor(Color::LightGreen);
		break;
	}
	view->setPosition(pos);
	addChild(view);
}

Point Bonus::getGridPosition()
{
	Point p;
	p.x = view->getX();
	p.x /= (int)view->getWidth();
	p.y = view->getY();
	p.y /= (int)view->getHeight();
	return p;
}

BONUS_TYPE Bonus::getType()
{
	return type;
}
