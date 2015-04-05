#include "Game.h"

#ifdef EMSCRIPTEN
	#include <emscripten.h>
#endif

using namespace oxygine;

Resources resources;
SoundPlayer splayer;
Game* g;
int SIZE;
Vector2 CORRECTION;

void preinit()
{
}

void init()
{
	SIZE = getStage()->getWidth() / 40;
	Vector2 sides(SIZE*40, SIZE*((int)getStage()->getHeight() / SIZE));
	CORRECTION = (getStage()->getSize() - sides) / 2;

	SoundSystem::instance = SoundSystem::create();
	SoundSystem::instance->init(4);
	SoundPlayer::initialize();
	splayer.setResources(&resources);
	splayer.setVolume(0.5);

	resources.loadXML("res.xml");
	float fontScale = 25 / getStage()->getHeight() * resources.getResFont("invaders")->getFont()->getSize();
	resources.getResFont("invaders")->getFont()->setScale(fontScale);

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

	g = new Game();
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
	delete g;
	splayer.stop();
	SoundSystem::instance->stop();
	resources.free();
	SoundSystem::instance->release();
}
