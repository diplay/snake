#include "Game.h"
#include <sstream>

extern Resources resources;
extern SoundPlayer splayer;
extern Game* g;

void Scene::genBonus()
{
	Point gridPos;
	while(snake->isPointOnSnake(gridPos = Point(rand()%gridW, rand()%gridH)))
	{}
	//log::messageln("bonus at %dx%d (%dx%d)", gridX, gridY, gridX * SIZE, gridY * SIZE);
	Vector2 pos(gridPos.x * SIZE, gridPos.y * SIZE);
	energy += (abs(gridPos.x - snake->getGridPosition().x));
	energy += (abs(gridPos.y - snake->getGridPosition().y));
	bonus = new Bonus(getRandomBonus(), pos);
	addChild(bonus);
}

void Scene::genRedstone()
{
	int r = rand() % 1000;
	if(r < (int)redstones.size())
	{
		removeChild(*redstones.begin());
		redstones.erase(redstones.begin());
	}
	if(r < 10)
	{
		Point gridPos;
		while(snake->isPointOnSnake(gridPos = Point(rand()%gridW, rand()%gridH))
				|| gridPos == bonus->getGridPosition())
		{}
		//log::messageln("bonus at %dx%d (%dx%d)", gridX, gridY, gridX * SIZE, gridY * SIZE);
		Vector2 pos(gridPos.x * SIZE, gridPos.y * SIZE);
		redstones.push_back(new Bonus(BONUS_REDSTONE, pos));
		addChild(redstones.back());
	}
}

void Scene::nextTact(Event* e)
{
	if(paused)
		return;
	for(auto stone : redstones)
	{
		if(stone->getGridPosition() == snake->getGridPosition())
		{
			gameOver();
			return;
		}
	}
	if(bonus.get() &&
			(snake->getGridPosition() == bonus->getGridPosition()))
	{
		removeChild(bonus);
		switch(bonus->getType())
		{
			case BONUS_EAT:
				snake->nextTact(STATUS_EAT);
				splayer.play("bite");
				break;
			case BONUS_HALF:
				snake->nextTact(STATUS_HALF);
				splayer.play("bonus");
				if(mode == MODE_SURVIVAL)
				{
					TextStyle style;
					style.font = resources.getResFont("invaders")->getFont();
					style.vAlign = TextStyle::VALIGN_MIDDLE;
					style.hAlign = TextStyle::HALIGN_CENTER;
					spTextField speedup = new TextField();
					addChild(speedup);
					speedup->setText("Speed Up!");
					speedup->setPosition(getStage()->getSize() / 2);
					speedup->setStyle(style);
					speedup->addTween(TweenAlpha(0), DURATION * 10)->setDetachActor(true);
					speedup->addTween(Actor::TweenY(speedup->getY() - SIZE*2), DURATION * 10, Tween::ease_outQuad);
					duration--;
					if(duration < 50)
						duration = 50;
				}
				break;
			default:
				break;
		}
		if(mode != MODE_CLASSIC)
		{
			if(energy >= handicapEnergy)
			{
				score += energy;
				score -= handicapEnergy;
			}
			score += DURATION - duration;
			energy += handicap;
			handicapEnergy += handicap;
			handicap--;
			if(handicap < 0)
				handicap = 0;
		}
		else
		{
			score += 10;
		}
		bonus = NULL;
	}
	else
	{
		snake->nextTact(STATUS_MOVE);
		energy--;
	}
	if(!bonus)
		genBonus();
	if(mode == MODE_CLASSIC)
	{
		if(snake->isPointOnSnakeBody(snake->getGridPosition()))
		{
			gameOver();
			return;
		}
	}
	else if(energy < 0)
	{
		gameOver();
		return;
	}
	if(mode == MODE_SURVIVAL)
		genRedstone();
	spTween t = addTween(TweenDummy(), duration);
	t->setDoneCallback(CLOSURE(this, &Scene::nextTact));
	scoreboard->setScore(score, energy);
}

BONUS_TYPE Scene::getRandomBonus()
{
	int r = rand() % 100;
	int R = 100;
	if(mode == MODE_INFINITY)
		R = 80;
	else if (mode == MODE_SURVIVAL)
		R = 85;
	if(r > R)
		return BONUS_HALF;
	else
		return BONUS_EAT;
}

Scene::Scene(GAME_MODE mode)
{
	paused = false;
	this->mode = mode;
	duration = DURATION;
	pauseText = new TextField();
	TextStyle style;
	style.font = resources.getResFont("invaders")->getFont();
	style.color = Color::White;
	style.vAlign = TextStyle::VALIGN_MIDDLE;
	style.hAlign = TextStyle::HALIGN_CENTER;
	pauseText->setStyle(style);
	pauseText->setText("Game Paused\n\nContinue");
	pauseText->setPosition(getStage()->getSize() / 2);
	pauseText->addEventListener(TouchEvent::TOUCH_UP, CLOSURE(this, &Scene::unpauseTouch));
	gridW = getStage()->getWidth() / SIZE;
	gridH = getStage()->getHeight() / SIZE;
	//log::messageln("gridW: %d\ngridH: %d", gridW, gridH);
	//log::messageln("SIZE: %d", SIZE);
	score = 0;
	handicap = 0;
	handicapEnergy = 0;
	if(mode == MODE_INFINITY)
		handicap = 20;
	energy = 100;
	Vector2 pos(gridW / 2 * SIZE , gridH / 2 * SIZE);
	snake = new Snake(pos);
	addChild(snake);
	scoreboard = new Scoreboard(Vector2(0, 0), mode);
	addChild(scoreboard);
	genBonus();
	getStage()->addEventListener(KeyEvent::KEY_DOWN, CLOSURE(this, &Scene::backButton));
}

void Scene::start()
{
	if(!paused)
	{
		if(mode == MODE_INFINITY)
			music = splayer.play("infinity", true);
		else if(mode == MODE_SURVIVAL)
			music = splayer.play("survival", true);
		else
			music = splayer.play("classic", true);
	}
	paused = false;
	spTween t = addTween(TweenDummy(), duration);
	t->setDoneCallback(CLOSURE(this, &Scene::nextTact));
}

void Scene::gameOver()
{
	music->stop();
	spTextField gameover = new TextField();
	TextStyle style;
	style.font = resources.getResFont("invaders")->getFont();
	style.color = Color::White;
	style.vAlign = TextStyle::VALIGN_MIDDLE;
	style.hAlign = TextStyle::HALIGN_CENTER;
	std::string text = "Game Over\n\n";
	int high = g->getHighScore(mode);
	std::string strHigh;
	std::ostringstream os;
	os << high;
	strHigh = os.str();
	if(score > high)
		text += "You've made a new high score!";
	else
		text += "High Score: " + strHigh;
	gameover->setText(text);
	gameover->setPosition(getStage()->getSize() / 2);
	gameover->setStyle(style);
	addChild(gameover);
	snake->die();
	removeChild(bonus);
	spSoundInstance s = splayer.play("gameover");
	s->setDoneCallback(CLOSURE(this, &Scene::setGameoverCallbacks));
}

void Scene::setGameoverCallbacks(Event* e)
{
	getStage()->addEventListener(KeyEvent::KEY_DOWN, CLOSURE(this, &Scene::anyKey));
	getStage()->addEventListener(TouchEvent::TOUCH_UP, CLOSURE(this, &Scene::anyKey));
	getStage()->addEventListener(TouchEvent::TOUCH_DOWN, CLOSURE(this, &Scene::anyKey));
}

void Scene::anyKey(Event* e)
{
	getStage()->removeEventListener(KeyEvent::KEY_DOWN, CLOSURE(this, &Scene::anyKey));
	getStage()->removeEventListener(TouchEvent::TOUCH_UP, CLOSURE(this, &Scene::anyKey));
	getStage()->removeEventListener(TouchEvent::TOUCH_DOWN, CLOSURE(this, &Scene::anyKey));
	GameOverEvent ev(score, mode);
	dispatchEvent(&ev);
}

void Scene::pause()
{
	addChild(pauseText);
	paused = true;
}

void Scene::unpauseTouch(Event* e)
{
	removeChild(pauseText);
	e->stopImmediatePropagation();
	start();
}

void Scene::backButton(Event* e)
{
	KeyEvent* ev = (KeyEvent*)e;
	if(ev->data->keysym.sym == SDLK_AC_BACK ||
		ev->data->keysym.sym == SDLK_ESCAPE)
	{
		if(paused)
		{
			music->stop();
			GameOverEvent ev(0, mode);
			dispatchEvent(&ev);
		}
		else
		{
			pause();
		}
	}
}
