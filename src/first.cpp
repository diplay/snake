#include "oxygine-framework.h"
#include "Game.h"

using namespace oxygine;

Resources resources;

void preinit(){}

void init()
{
	resources.loadXML("res.xml");
	Game* g = new Game();
	//and add it to Stage as child
	g->showMenu();
}


//called each frame from entry_point.cpp
void update()
{
}

void destroy()
{
}
