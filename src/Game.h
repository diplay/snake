#include "oxygine-framework.h"
#include "Scene.h"
#include "Menu.h"

using namespace oxygine;

class Game
{
private:
	spMenu menu;
	spScene scene;
	spSoundInstance menuMusic;
	GAME_MODE mode;
public:
	Game();
	void showMenu();
	void onNewGame(Event* e);
	void onNewGameSurvival(Event* e);
	void onMenuFadeOut(Event* e);
	void onGameOver(Event* e);
	void onExit(Event* e);
};
