#include "Scene.h"

extern Resources resources;

void Scene::genBonus()
{
	int gridX, gridY;
	gridX = (rand() % gridW);
	gridY = (rand() % gridH);
	//log::messageln("bonus at %dx%d (%dx%d)", gridX, gridY, gridX * SIZE, gridY * SIZE);
	Vector2 pos(gridX * SIZE, gridY * SIZE);
	energy += (abs(gridX - snake->getGridPosition().x));
	energy += (abs(gridY - snake->getGridPosition().y));
	bonus = new Bonus(getRandomBonus(), pos);
	addChild(bonus);
}

void Scene::nextTact(Event* e)
{
	if(bonus.get() &&
			(snake->getGridPosition() == bonus->getGridPosition()))
		switch(bonus->getType())
		{
			case BONUS_EAT:
				removeChild(bonus);
				bonus = NULL;
				snake->nextTact(STATUS_EAT);
				score += energy;
				break;
			case BONUS_HALF:
				removeChild(bonus);
				bonus = NULL;
				snake->nextTact(STATUS_HALF);
				score += energy;
				break;
		}
	else
	{
		snake->nextTact(STATUS_MOVE);
		energy--;
	}
	if(!bonus)
		genBonus();
	if(energy < 0)
	{
		gameOver();
		return;
	}
	spTween t = addTween(TweenDummy(), duration);
	t->setDoneCallback(CLOSURE(this, &Scene::nextTact));
	scoreboard->setScore(score, energy);
}

BONUS_TYPE Scene::getRandomBonus()
{
	int r = rand() % 100;
	if(r > 80)
		return BONUS_HALF;
	else
		return BONUS_EAT;
}

Scene::Scene()
{
	gridW = getStage()->getWidth() / SIZE;
	gridH = getStage()->getHeight() / SIZE;
	//log::messageln("gridW: %d\ngridH: %d", gridW, gridH);
	//log::messageln("SIZE: %d", SIZE);
	score = 0;
	handicap = 100;
	energy = 100;
	Vector2 pos(gridW / 2 * SIZE , gridH / 2 * SIZE);
	snake = new Snake(pos);
	addChild(snake);
	scoreboard = new Scoreboard(Vector2((gridW - 5) * SIZE, 0));
	addChild(scoreboard);
	genBonus();
	spTween t = addTween(TweenDummy(), duration);
	t->setDoneCallback(CLOSURE(this, &Scene::nextTact));
}

void Scene::gameOver()
{
	spTextField gameover = new TextField();
	TextStyle style;
	style.font = resources.getResFont("main")->getFont();
	style.color = Color::White;
	gameover->setText("Game Over");
	gameover->setPosition(getStage()->getSize() / 2);
	gameover->setStyle(style);
	addChild(gameover);
	removeChild(snake);
	removeChild(bonus);
	getStage()->addEventListener(KeyEvent::KEY_DOWN, CLOSURE(this, &Scene::anyKey));
	getStage()->addEventListener(TouchEvent::TOUCH_UP, CLOSURE(this, &Scene::anyKey));
	getStage()->addEventListener(TouchEvent::TOUCH_DOWN, CLOSURE(this, &Scene::anyKey));
}

void Scene::anyKey(Event* e)
{
	GameOverEvent ev(score);
	dispatchEvent(&ev);
}
