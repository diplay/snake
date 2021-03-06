#include "Snake.h"
#include "Bonus.h"
#include "Scoreboard.h"
#include "SoundPlayer.h"
#include "SoundInstance.h"
#include "SoundSystem.h"
#include "ResSound.h"

class GameOverEvent : public Event
{
public:
	enum {EVENT = eventID('G', 'm', 'O', 'r')};

	int score, mode;

	GameOverEvent(int score, GAME_MODE mode): Event(EVENT), score(score), mode(mode){}
};

class Scene : public Actor
{
private:
	GAME_MODE mode;
	spSnake snake;
	spBonus bonus;
	spScoreboard scoreboard;
	spSoundInstance music;
	spTextField pauseText;
	std::list<spBonus> redstones;
	int score, handicap, handicapEnergy, energy;
	bool paused;

	int duration;
	int gridW, gridH;

	void nextTact(Event* e);
	BONUS_TYPE getRandomBonus();
	void genBonus();
	void genRedstone();
	void gameOver();
	void pause();
	void anyKey(Event* e);
	void backButton(Event* e);
	void setGameoverCallbacks(Event* e);
	void unpauseTouch(Event* e);

public:
	Scene(GAME_MODE mode);
	void start();
};

typedef oxygine::intrusive_ptr<Scene> spScene;
