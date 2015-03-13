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
#ifdef PC_VERSION
	menu->addItem("Exit", CLOSURE(this, &Game::onExit));
#endif
	std::string soundStatusResAnim;
	if(soundStatus)
	{
		splayer.setVolume(1.0f);
		soundStatusResAnim = "sound_on";
	}
	else
	{
		splayer.setVolume(0.0f);
		soundStatusResAnim = "sound_off";
	}
	soundToggle = new Sprite();
	soundToggle->setResAnim(resources.getResAnim(soundStatusResAnim));
	soundToggle->setScale(0.75f);
	soundToggle->addEventListener(TouchEvent::CLICK, CLOSURE(this, &Game::onSoundToggle));
	highscoresToggle = new Sprite();
	highscoresToggle->setResAnim(resources.getResAnim("icon_highscore"));
	highscoresToggle->setScale(0.75f);
	Vector2 pos(getStage()->getWidth(), 0);
	pos.x -= highscoresToggle->getWidth() * highscoresToggle->getScaleX();
	highscoresToggle->setPosition(pos);
	highscoresToggle->addEventListener(TouchEvent::CLICK, CLOSURE(this, &Game::onHighscoresToggle));
	getStage()->addEventListener(KeyEvent::KEY_UP, CLOSURE(this, &Game::backButton));
}

Game::~Game()
{
	saveFile(settings, "settings");
	saveFile(highscores, "scores");
}

void Game::showScoreboard()
{
	scoreboard = new Menu();
	scoreboard->style.hAlign = TextStyle::HALIGN_LEFT;
	scoreboard->setPosition(getStage()->getSize() / 2);
	std::stringstream ss;
	std::string score;
	ss << highscores["infinity"];
	ss >> score;
	ss.clear();
	scoreboard->addItem(score);
	ss << highscores["survival"];
	ss >> score;
	ss.clear();
	scoreboard->addItem(score);
	ss << highscores["classic"];
	ss >> score;
	scoreboard->addItem(score);
	getStage()->addChild(scoreboard);
	scoreboard->setAlpha(0);
	scoreboard->addTween(Actor::TweenAlpha(255), 1000, 1, false, 0, Tween::ease_inExpo);
	scoreboard->addTween(Actor::TweenX(scoreboard->getX() + 200), 1000);
}

void Game::hideScoreboard()
{
	scoreboard->addTween(Actor::TweenAlpha(0), 1000)->setDetachActor(true);
	scoreboard = NULL;
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

void Game::onHighscoresToggle(Event* e)
{
	settings["highscores"] = (settings["highscores"] + 1) % 2;
	if(settings["highscores"])
	{
		showScoreboard();
	}
	else
	{
		hideScoreboard();
	}
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
	getStage()->addChild(soundToggle);
	getStage()->addChild(highscoresToggle);
	menu->setAlpha(0);
	menu->addTween(Actor::TweenAlpha(255), 1000);
	soundToggle->setAlpha(0);
	soundToggle->addTween(Actor::TweenAlpha(255), 1000);
	highscoresToggle->setAlpha(0);
	highscoresToggle->addTween(Actor::TweenAlpha(255), 1000);
	menuMusic = splayer.play("menu", true, 1000);
	if(settings["highscores"] == 1)
		showScoreboard();
}

void Game::prepareScene(GAME_MODE mode)
{
	menuMusic->fadeOut(500);
	if(settings["highscores"] == 1)
		hideScoreboard();
	spTween t = menu->addTween(Actor::TweenAlpha(0), 1000);
	soundToggle->addTween(Actor::TweenAlpha(0), 1000);
	highscoresToggle->addTween(Actor::TweenAlpha(0), 1000);
	t->addDoneCallback(CLOSURE(this, &Game::onMenuFadeOut));
	scene = new Scene(mode);
}

void Game::onNewGameClassic(Event* e)
{
	prepareScene(MODE_CLASSIC);
}
void Game::onNewGameSurvival(Event* e)
{
	prepareScene(MODE_SURVIVAL);
}

void Game::onNewGame(Event* e)
{
	prepareScene(MODE_INFINITY);
}

void Game::onMenuFadeOut(Event* e)
{
	getStage()->removeChild(menu);
	getStage()->removeChild(soundToggle);
	getStage()->removeChild(highscoresToggle);
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
