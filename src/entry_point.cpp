/**
Attention!
This file has Oxygine initialization stuff.
If you just started you don't need to understand it exactly you could check it later.
You could start from example.cpp and example.h it has main functions being called from there
*/
#include "core/oxygine.h"
#include "Stage.h"
#include "DebugActor.h"

#include "first.h"


using namespace oxygine;


//called each frame
int mainloop()
{
	update();
	//update our stage
	//update all actors. Actor::update would be called also for all children
	getStage()->update();
	
	if (core::beginRendering())
	{		
		Color clearColor(0, 0, 0, 255);
		Rect viewport(Point(0, 0), core::getDisplaySize());
		//render all actors. Actor::render would be called also for all children
		getStage()->render(clearColor, viewport);

		core::swapDisplayBuffers();
	}

	//update internal components
	//all input events would be passed to Stage::instance.handleEvent
	//if done is true then User requests quit from app.
	bool done = core::update();

	return done ? 1 : 0;
}

//it is application entry point
void run()
{
	ObjectBase::__startTracingLeaks();

	//initialize Oxygine's internal stuff
	core::init_desc desc;

#if OXYGINE_SDL || OXYGINE_EMSCRIPTEN
	//we could setup initial window size on SDL builds
	desc.w = 800;
	desc.h = 480;
	//marmalade settings could be changed from emulator's menu
#endif


	preinit();
	core::init(&desc);


	//create Stage. Stage is a root node
	Stage::instance = new Stage(true);
	Point size = core::getDisplaySize();
	getStage()->setSize(size);

	//DebugActor is a helper actor node. It shows FPS, memory usage and other useful stuff
	//DebugActor::show();
		
	//initialize this example stuff. see example.cpp
	init();

#ifdef EMSCRIPTEN
	/*
	if you build for Emscripten mainloop would be called automatically outside.
	see emscripten_set_main_loop below
	*/	
	return;
#endif


	//here is main game loop
	while (1)
	{
		int done = mainloop();
		if (done)
			break;
	}
	//user wants to leave application...

	//lets dump all created objects into log
	//all created and not freed resources would be displayed
	ObjectBase::dumpCreatedObjects();

	//lets cleanup everything right now and call ObjectBase::dumpObjects() again
	//we need to free all allocated resources and delete all created actors
	//all actors/sprites are smart pointer objects and actually you don't need it remove them by hands
	//but now we want delete it by hands

	//check example.cpp
	destroy();


	//renderer.cleanup();

	/**releases all internal components and Stage*/
	core::release();

	//dump list should be empty now
	//we deleted everything and could be sure that there aren't any memory leaks
	ObjectBase::dumpCreatedObjects();

	ObjectBase::__stopTracingLeaks();
	//end
}

#ifdef __S3E__
int main(int argc, char* argv[])
{
	run();
	return 0;
}
#endif


#ifdef OXYGINE_SDL
#ifdef __MINGW32__
int WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
	run();
	return 0;
}
#else
#include "SDL_main.h"
extern "C"
{
	int main(int argc, char* argv[])
	{
		run();
		return 0;
	}
};
#endif
#endif

#ifdef EMSCRIPTEN
#include <emscripten.h>

void one(){ mainloop(); }

int main(int argc, char* argv[])
{
	run();
	emscripten_set_main_loop(one, 0, 0);
	return 0;
}
#endif
