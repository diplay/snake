#include "oxygine-framework.h"
#include <list>

using namespace oxygine;

class Menu : public Actor
{
private:
	std::list<spTextField> items;
public:
	TextStyle style;
	Menu();
	void addItem(std::string text, EventCallback onClick);
	void addItem(std::string text);
};

typedef oxygine::intrusive_ptr<Menu> spMenu;
