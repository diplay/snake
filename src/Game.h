#include "oxygine-framework.h"
#include "Scene.h"
#include "Menu.h"

using namespace oxygine;

class Game
{
private:
	spMenu menu;
	spScene scene;
public:
	Game();
	void showMenu();
	void onNewGame(Event* e);
	void onGameOver(Event* e);
	void onExit(Event* e);
};
