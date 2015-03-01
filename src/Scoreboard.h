#include "oxygine-framework.h"

using namespace oxygine;

enum GAME_MODE
{
	MODE_INFINITY = 0,
	MODE_SURVIVAL = 1,
	MODE_CLASSIC = 2
};

class Scoreboard : public Actor
{
private:
	spTextField text;
	int score, energy;
	GAME_MODE mode;

	void updateText();
public:
	Scoreboard(Vector2 pos, GAME_MODE mode);
	void setScore(int score, int energy);
};

typedef oxygine::intrusive_ptr<Scoreboard> spScoreboard;
