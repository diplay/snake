#include "oxygine-framework.h"
#include <list>
#include "params.h"

using namespace oxygine;

enum STATUS
{
	STATUS_MOVE = 0,
	STATUS_EAT = 1,
	STATUS_HALF = 2
};

class Snake : public Actor
{
private:
	enum DIRECTION
	{
		UP = 0,
		RIGHT = 1,
		DOWN = 2,
		LEFT = 3
	};

	friend class Scene;

	DIRECTION dir, prevDir;
	std::list<spColorRectSprite> snakeBody;
	int gridW, gridH;

	void makeHead(Vector2 pos);
	void initSnake(Vector2 pos);
	void keyPressed(Event* event);
	void swipe(Event* event);
	void detachTail(size_t sz);
	void changeDir(DIRECTION d);

public:

	Snake(Vector2 pos);
	void nextTact(STATUS s, int duration);
	void die();
	Point getGridPosition() const;
	bool isPointOnSnake(Point gridCorrd) const;
	bool isPointOnSnakeBody(Point gridCorrd) const;
};

typedef oxygine::intrusive_ptr<Snake> spSnake;

