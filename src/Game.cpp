#include "Game.h"
#include <sstream>

extern SoundPlayer splayer;

Game::Game()
{
	menu = new Menu();
	menu->setPosition(getStage()->getSize() / 2);
	menu->addItem("Infinity Mode", CLOSURE(this, &Game::onNewGame));
	menu->addItem("Survival Mode", CLOSURE(this, &Game::onNewGameSurvival));
	menu->addItem("Classic Mode", CLOSURE(this, &Game::onNewGameClassic));
	menu->addItem("Exit", CLOSURE(this, &Game::onExit));
	file::handle h = file::open("scores", "rb");
	char buf[256];
	std::string mode;
	int score;
	file::read(h, buf, sizeof(buf));
	file::close(h);
	stringstream ss;
	ss << buf;
	for(int i = 0; i < 3; i++)
	{
		ss >> mode >> score;
		highscores[mode] = score;
	}
	for(auto it : highscores)
	{
		log::messageln("%s: %d", it.first.c_str(), it.second);
	}
}

Game::~Game()
{
	log::messageln("lol game destuct");
	char buf[256] = {0};
	for(auto it : highscores)
	{
		log::messageln("%d", strlen(buf));
		sprintf(&buf[strlen(buf)], "%s %d\n", it.first.c_str(), it.second);
	}
	log::messageln(buf);
	file::handle h = file::open("scores", "wb");
	file::write(h, buf, strlen(buf));
	file::close(h);
}

void Game::showMenu()
{
	getStage()->addChild(menu);
	menu->setAlpha(0);
	menu->addTween(Actor::TweenAlpha(255), 1000);
	menuMusic = splayer.play("menu", true, 1000);
}

void Game::onNewGameClassic(Event* e)
{
	menuMusic->fadeOut(500);
	spTween t = menu->addTween(Actor::TweenAlpha(0), 1000);
	t->addDoneCallback(CLOSURE(this, &Game::onMenuFadeOut));
	scene = new Scene(MODE_CLASSIC);
}
void Game::onNewGameSurvival(Event* e)
{
	menuMusic->fadeOut(500);
	spTween t = menu->addTween(Actor::TweenAlpha(0), 1000);
	t->addDoneCallback(CLOSURE(this, &Game::onMenuFadeOut));
	scene = new Scene(MODE_SURVIVAL);
}

void Game::onNewGame(Event* e)
{
	menuMusic->fadeOut(500);
	spTween t = menu->addTween(Actor::TweenAlpha(0), 1000);
	t->addDoneCallback(CLOSURE(this, &Game::onMenuFadeOut));
	scene = new Scene(MODE_INFINITY);
}

void Game::onMenuFadeOut(Event* e)
{
	getStage()->removeChild(menu);
	scene->addEventListener(GameOverEvent::EVENT, CLOSURE(this, &Game::onGameOver));
	getStage()->addChild(scene);
	scene->start();
}

void Game::onExit(Event* e)
{
	core::requestQuit();
}

void Game::onGameOver(Event* e)
{
	GameOverEvent* ev = (GameOverEvent*)e;
	log::messageln("Score: %d", ev->score);
	switch(ev->mode)
	{
	case MODE_INFINITY:
		highscores["infinity"] = max(highscores["infinity"], ev->score);
		break;
	case MODE_SURVIVAL:
		highscores["survival"] = max(highscores["survival"], ev->score);
		break;
	case MODE_CLASSIC:
		highscores["classic"] = max(highscores["classic"], ev->score);
		break;
	}
	for(auto it : highscores)
	{
		log::messageln("%s: %d", it.first.c_str(), it.second);
	}
	getStage()->removeChild(scene);
	showMenu();
	scene = NULL;
}
