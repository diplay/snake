#include "Game.h"

extern SoundPlayer splayer;

Game::Game()
{
	menu = new Menu();
	menu->setPosition(getStage()->getSize() / 2);
	menu->addItem("Infinity Mode", CLOSURE(this, &Game::onNewGame));
	menu->addItem("Survival Mode", CLOSURE(this, &Game::onNewGameSurvival));
	menu->addItem("Exit", CLOSURE(this, &Game::onExit));
}

void Game::showMenu()
{
	getStage()->addChild(menu);
	menu->setAlpha(0);
	menu->addTween(Actor::TweenAlpha(255), 1000);
	menuMusic = splayer.play("menu", true, 1000);
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
	getStage()->removeChild(scene);
	showMenu();
	scene = NULL;
}
