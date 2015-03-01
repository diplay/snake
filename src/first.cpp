#include "oxygine-framework.h"
#include "Game.h"
#ifdef EMSCRIPTEN
#include <emscripten.h>
#endif

using namespace oxygine;

Resources resources;
SoundPlayer splayer;

void preinit(){}

void init()
{
	SoundSystem::instance = SoundSystem::create();
	SoundSystem::instance->init(4);
	SoundPlayer::initialize();
	splayer.setResources(&resources);
	splayer.setVolume(0.5);

	resources.loadXML("res.xml");
	resources.getResFont("invaders")->getFont()->setScale(1.5);

	for (int i = 0; i < resources.getCount(); ++i)
	{
		ResSound *rs = dynamic_cast<ResSound*>(resources.get(i));
		if (!rs)
			continue;

#ifdef EMSCRIPTEN
		EM_ASM_ARGS({
			var p = Preloading("hello");
			p.add(Pointer_stringify($0));
			p.start();
		}, rs->getPath().c_str());
#endif
	}

	Game* g = new Game();
	g->showMenu();
}


//called each frame from entry_point.cpp
void update()
{
	SoundSystem::instance->update();
	splayer.update();
}

void destroy()
{
	splayer.stop();
	SoundSystem::instance->stop();
	resources.free();
	SoundSystem::instance->release();
}
