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

class Scene : public Actor
{
private:
	spSnake snake;
	spBonus bonus;
	spScoreboard scoreboard;
	spSoundInstance music;
	int score, handicap, energy;

	static const int duration = DURATION;
	int gridW, gridH;

	void nextTact(Event* e);
	BONUS_TYPE getRandomBonus();
	void genBonus();
	void gameOver();
	void anyKey(Event* e);

public:
	Scene();
};

typedef oxygine::intrusive_ptr<Scene> spScene;
