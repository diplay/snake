#include "oxygine-framework.h"
#include "Scene.h"
#include "Menu.h"
#include <map>

using namespace oxygine;

class Game
{
private:
	spMenu menu, scoreboard;
	spScene scene;
	spSoundInstance menuMusic;
	spSprite soundToggle, highscoresToggle;
	std::map<std::string, int> highscores;
	std::map<std::string, int> settings;
	bool soundStatus;

	void backButton(Event* e);
	void onNewGame(Event* e);
	void onNewGameSurvival(Event* e);
	void onNewGameClassic(Event* e);
	void onMenuFadeOut(Event* e);
	void onGameOver(Event* e);
	void onExit(Event* e);
	void onSoundToggle(Event* e);
	void onHighscoresToggle(Event* e);
	void prepareScene(GAME_MODE mode);
	void showScoreboard();
	void hideScoreboard();
public:
	Game();
	~Game();
	int getHighScore(GAME_MODE mode);
	bool getSoundStatus();
	void showMenu();
};
