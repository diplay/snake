#include "Game.h"
#include <sstream>

extern SoundPlayer splayer;
extern Resources resources;

void loadFile(std::map<std::string, int>& data, std::string filename)
{
	file::handle h = file::open(filename.c_str(), "rb");
	char buf[256] = {0};
	std::string setting;
	int value;
	file::read(h, buf, sizeof(buf));
	file::close(h);
	stringstream ss;
	ss << buf;
	while(ss >> setting >> value)
	{
		data[setting] = value;
		log::messageln("read %s: %d", setting.c_str(), value);
	}
}

void saveFile(std::map<std::string, int>& data, std::string filename)
{
	char buf[256];
	buf[0] = 0;
	for(auto it : data)
	{
		sprintf(&buf[strlen(buf)], "%s %d\n", it.first.c_str(), it.second);
	}
	file::handle h = file::open(filename.c_str(), "wb");
	log::messageln("writing into %s: %s", filename.c_str(), buf);
	file::write(h, buf, strlen(buf));
	file::close(h);
}

Game::Game()
{
	loadFile(highscores, "scores");
	loadFile(settings, "settings");
	soundStatus = (bool)settings["sound"];
	menu = new Menu();
	menu->setPosition(getStage()->getSize() / 2);
	menu->addItem("Infinity Mode", CLOSURE(this, &Game::onNewGame));
	menu->addItem("Survival Mode", CLOSURE(this, &Game::onNewGameSurvival));
	menu->addItem("Classic Mode", CLOSURE(this, &Game::onNewGameClassic));
	std::string soundStatusResAnim;
	if(soundStatus)
		soundStatusResAnim = "sound_on";
	else
		soundStatusResAnim = "sound_off";
	soundToggle = new Sprite();
	soundToggle->setResAnim(resources.getResAnim(soundStatusResAnim));
	soundToggle->setScale(0.5f);
	soundToggle->attachTo(menu);
	soundToggle->setPosition(getStage()->getSize() / 2 * (-1));
	soundToggle->addEventListener(TouchEvent::TOUCH_UP, CLOSURE(this, &Game::onSoundToggle));
	//menu->addItem("Exit", CLOSURE(this, &Game::onExit));
	getStage()->addEventListener(KeyEvent::KEY_UP, CLOSURE(this, &Game::backButton));
}

Game::~Game()
{
	saveFile(settings, "settings");
	saveFile(highscores, "scores");
}

void Game::onSoundToggle(Event* e)
{
	soundStatus = !soundStatus;
	std::string soundStatusResAnim;
	if(soundStatus)
	{
		splayer.setVolume(1.0f);
		soundStatusResAnim = "sound_on";
		settings["sound"] = 1;
	}
	else
	{
		splayer.setVolume(0.0f);
		settings["sound"] = 0;
		soundStatusResAnim = "sound_off";
	}
	soundToggle->setResAnim(resources.getResAnim(soundStatusResAnim));
}

int Game::getHighScore(GAME_MODE mode)
{
	switch(mode)
	{
	case MODE_INFINITY:
		return highscores["infinity"];
		break;
	case MODE_SURVIVAL:
		return highscores["survival"];
		break;
	case MODE_CLASSIC:
		return highscores["classic"];
		break;
	}
}

bool Game::getSoundStatus()
{
	return soundStatus;
}

void Game::backButton(Event* e)
{
	KeyEvent* ev = (KeyEvent*)e;
	if(scene.get() == NULL)
	{
		if(ev->data->keysym.sym == SDLK_AC_BACK ||
			ev->data->keysym.sym == SDLK_ESCAPE)
			onExit(e);
	}
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
