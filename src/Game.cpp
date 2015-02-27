#include "Game.h"

extern SoundPlayer splayer;

Game::Game()
{
	menu = new Menu();
	menu->setPosition(getStage()->getSize() / 2);
	menu->addItem("New Game", CLOSURE(this, &Game::onNewGame));
	menu->addItem("Exit", CLOSURE(this, &Game::onExit));
}

void Game::showMenu()
{
	getStage()->addChild(menu);
	menuMusic = splayer.play("menu", true);
}

void Game::onNewGame(Event* e)
{
	menuMusic->fadeOut(500);
	getStage()->removeChild(menu);
	scene = new Scene();
	scene->addEventListener(GameOverEvent::EVENT, CLOSURE(this, &Game::onGameOver));
	getStage()->addChild(scene);
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
