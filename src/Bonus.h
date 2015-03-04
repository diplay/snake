#include "oxygine-framework.h"
#include "params.h"

using namespace oxygine;

enum BONUS_TYPE
{
	BONUS_EAT = 0,
	BONUS_HALF = 1,
	BONUS_REDSTONE = 2
};

class Bonus : public Actor
{
private:
	spSprite view;
	BONUS_TYPE type;
public:
	Bonus(BONUS_TYPE t, Vector2 pos);
	Point getGridPosition();
	BONUS_TYPE getType();
};

typedef oxygine::intrusive_ptr<Bonus> spBonus;
