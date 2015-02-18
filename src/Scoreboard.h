#include "oxygine-framework.h"

using namespace oxygine;

class Scoreboard : public Actor
{
private:
	spTextField text;
	int score, energy;

	void update();
public:
	Scoreboard(Vector2 pos);
	void setScore(int score, int energy);
};

typedef oxygine::intrusive_ptr<Scoreboard> spScoreboard;
