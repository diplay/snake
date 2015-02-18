#include "Snake.h"
#include "Bonus.h"
#include "Scoreboard.h"

class Scene : public Actor
{
private:
	spSnake snake;
	spBonus bonus;
	spScoreboard scoreboard;
	int score, handicap, energy;

	static const int duration = DURATION;
	int gridW, gridH;

	void nextTact(Event* e);
	BONUS_TYPE getRandomBonus();
	void genBonus();

public:
	Scene();
};

typedef oxygine::intrusive_ptr<Scene> spScene;
