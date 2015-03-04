#include "oxygine-framework.h"
#include "Scene.h"
#include "Menu.h"
#include <map>

using namespace oxygine;

class Game
{
private:
	spMenu menu;
	spScene scene;
	spSoundInstance menuMusic;
	std::map<std::string, int> highscores;
public:
	Game();
	~Game();
	void showMenu();
	void onNewGame(Event* e);
	void onNewGameSurvival(Event* e);
	void onNewGameClassic(Event* e);
	void onMenuFadeOut(Event* e);
	void onGameOver(Event* e);
	void onExit(Event* e);
};
