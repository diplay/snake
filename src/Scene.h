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

	int score;

	GameOverEvent(int score): Event(EVENT), score(score){}
};

enum GAME_MODE
{
	MODE_INFINITY = 0,
	MODE_SURVIVAL = 1
};

class Scene : public Actor
{
private:
	GAME_MODE mode;
	spSnake snake;
	spBonus bonus;
	spScoreboard scoreboard;
	spSoundInstance music;
	int score, handicap, energy;

	int duration;
	int gridW, gridH;

	void nextTact(Event* e);
	BONUS_TYPE getRandomBonus();
	void genBonus();
	void gameOver();
	void anyKey(Event* e);
	void setGameoverCallbacks(Event* e);

public:
	Scene(GAME_MODE mode);
	void start();
};

typedef oxygine::intrusive_ptr<Scene> spScene;
