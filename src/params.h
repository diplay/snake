//#define SIZE 32
#define DURATION 100

#if !defined(EMSCRIPTEN) && !defined(__ANDROID_API__)
	#define PC_VERSION
#endif

#include "oxygine-framework.h"

extern int SIZE;
extern oxygine::Vector2 CORRECTION;
