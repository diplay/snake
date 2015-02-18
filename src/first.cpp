#include "oxygine-framework.h"
#include "Scene.h"

using namespace oxygine;

Resources resources;

void preinit(){}

void init()
{
	resources.loadXML("data/res.xml");
	spScene actor = new Scene;

	//and add it to Stage as child
	getStage()->addChild(actor);
}


//called each frame from entry_point.cpp
void update()
{
}

void destroy()
{
}
