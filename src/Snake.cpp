#include "Snake.h"

void Snake::makeHead(Vector2 pos)
{
	spSprite head = new ColorRectSprite();
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
	static float width = getStage()->getWidth();
	static float height = getStage()->getHeight();
	if(pos.x >= width || pos.x < 0)
		pos.x = (pos.x + width) > width ? pos.x - width : pos.x + width;
	if(pos.y >= height || pos.y < 0)
		pos.y = (pos.y + height) > height ? pos.y - height : pos.y + height;
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
		for(size_t i = 0; i < snakeBody.size() / 2; i++)
		{
			removeChild(snakeBody.back());
			snakeBody.pop_back();
		}
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
	initSnake(pos);
	getStage()->addEventListener(KeyEvent::KEY_DOWN, CLOSURE(this, &Snake::keyPressed));
	getStage()->addEventListener(TouchEvent::TOUCH_UP, CLOSURE(this, &Snake::swipe));
	getStage()->addEventListener(TouchEvent::TOUCH_DOWN, CLOSURE(this, &Snake::swipe));
}
