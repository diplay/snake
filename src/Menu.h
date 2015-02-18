#include "oxygine-framework.h"
#include <list>

using namespace oxygine;

class Menu : public Actor
{
private:
	std::list<spTextField> items;
	TextStyle style;
public:
	Menu();
	void addItem(std::string text, EventCallback onClick);
};
