#include "Snake.h"

void Snake::makeHead(Vector2 pos)
{
	spColorRectSprite head = new ColorRectSprite();
	head->setWidth(SIZE);
	head->setHeight(SIZE);
	head->setColor(Color::Olive);
	head->setPosition(pos);
	addChild(head);
	snakeBody.push_front(head);
}

void Snake::initSnake(Vector2 pos)
{
	dir = UP;
	//Vector2 pos = getStage()->getSize()/2;
	makeHead(pos);
	int h = snakeBody.front()->getHeight();
	pos.y -= h;
	makeHead(pos);
	pos.y -= h;
	makeHead(pos);
	pos.y -= h;
	makeHead(pos);
}

void Snake::keyPressed(Event* event)
{
	KeyEvent* e = (KeyEvent*)event;
	DIRECTION newdir;
	switch(e->data->keysym.sym)
	{
		case SDLK_DOWN:
			newdir = DOWN;
			break;
		case SDLK_UP:
			newdir = UP;
			break;
		case SDLK_LEFT:
			newdir = LEFT;
			break;
		case SDLK_RIGHT:
			newdir = RIGHT;
			break;
		default:
			return;
			break;
	}
	if(abs(newdir - dir) != 2)
		dir = newdir;
}

void Snake::swipe(Event* event)
{
	TouchEvent* e = (TouchEvent*)event;
	static bool pressed = false;
	static Vector2 origin(0, 0);
	static const Vector2 up(0, -1), right(1, 0), down(0, 1), left(-1, 0);
	switch (e->type)
	{
	case TouchEvent::TOUCH_DOWN:
		pressed = true;
		origin = e->position;
		log::messageln("touch down");
		break;
	case TouchEvent::TOUCH_UP:
		if(pressed)
		{
			Vector2 d = e->position - origin;
			d.normalize();
			float ratings[4];
			ratings[UP] = d.dot(up);
			ratings[RIGHT] = d.dot(right);
			ratings[DOWN] = d.dot(down);
			ratings[LEFT] = d.dot(left);
			DIRECTION newdir = (DIRECTION)0;
			for(int i = 1; i < 4; i++)
				if(ratings[i] > ratings[newdir])
					newdir = (DIRECTION)i;
			if(abs(newdir - dir) != 2)
				dir = newdir;
		}
		pressed = false;
		break;
	}
}

void Snake::nextTact(STATUS s)
{
	Vector2 pos = snakeBody.front()->getPosition();
	int w = snakeBody.front()->getWidth();
	int h = snakeBody.front()->getHeight();
	switch(dir)
	{
		case UP:
			pos.y -= h;
			break;
		case RIGHT:
			pos.x += w;
			break;
		case DOWN:
			pos.y += h;
			break;
		case LEFT:
			pos.x -= w;
			break;
	}
	/*static float width = getStage()->getWidth();
	static float height = getStage()->getHeight();
	if(pos.x >= width || pos.x < 0)
		pos.x = (pos.x + width) > width ? pos.x - width : pos.x + width;
	if(pos.y >= height || pos.y < 0)
		pos.y = (pos.y + height) > height ? pos.y - height : pos.y + height;
	*/
	Point gridPos = Point((int)pos.x / w, (int)pos.y / h);
	if(gridPos.x >= gridW)
		gridPos.x -= gridW;
	else if(gridPos.x < 0)
		gridPos.x += gridW;
	if(gridPos.y >= gridH)
		gridPos.y -= gridH;
	else if(gridPos.y < 0)
		gridPos.y += gridH;
	pos.x = gridPos.x * w;
	pos.y = gridPos.y * h;
	makeHead(pos);
	switch(s)
	{
	case STATUS_MOVE:
		removeChild(snakeBody.back());
		snakeBody.pop_back();
		break;
	case STATUS_EAT:
		break;
	case STATUS_HALF:
		detachTail(snakeBody.size() / 2);
	}
}

Point Snake::getGridPosition() const
{
	Point p;
	p.x = snakeBody.front()->getX();
	p.x /= snakeBody.front()->getWidth();
	p.y = snakeBody.front()->getY();
	p.y /= snakeBody.front()->getHeight();
	return p;
}

Snake::Snake(Vector2 pos)
{
	gridW = getStage()->getWidth() / SIZE;
	gridH = getStage()->getHeight() / SIZE;
	initSnake(pos);
}

void Snake::die()
{
	detachTail(snakeBody.size());
}

void Snake::detachTail(size_t sz)
{
	int time = DURATION;
	for(size_t i = 0; i < sz; i++)
	{
		spTween t = snakeBody.back()->addTween(Actor::TweenAlpha(0), time);
		time += DURATION;
		t->setDetachActor(true);
		snakeBody.pop_back();
	}
}

bool Snake::isPointOnSnakeBody(Point gridCorrd) const
{
	int w = snakeBody.front()->getWidth();
	int h = snakeBody.front()->getHeight();
	auto el = snakeBody.begin();
	el++;
	for(; el != snakeBody.end(); el++)
	{
		Vector2 pos = (*el)->getPosition();
		Point gridPos = Point((int)pos.x / w, (int)pos.y / h);
		if(gridPos == gridCorrd)
			return true;
	}
	return false;
}

bool Snake::isPointOnSnake(Point gridCorrd) const
{
	return isPointOnSnakeBody(gridCorrd) || (gridCorrd == getGridPosition());
}
