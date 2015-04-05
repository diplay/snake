#include "Ads.h"
#include "oxygine-framework.h"
#ifdef __ANDROID__
	#include "core/android/jniHelper.h"
	#include "core/android/jniUtils.h"
#endif

using namespace oxygine;

void showAd()
{
#ifdef __ANDROID__
	log::messageln("trying to show ad");
	JNIEnv *env = jniGetEnv();
	jobject activity = jniGetMainActivity();
	jclass mainclass = env->FindClass("org/oxygine/snake/MainActivity");
	jmethodID showAd = env->GetMethodID(mainclass, "showAd", "()V");
	env->CallVoidMethod(activity, showAd);
#endif
}

void loadAd()
{
#ifdef __ANDROID__
	log::messageln("trying to load ad");
	JNIEnv *env = jniGetEnv();
	jobject activity = jniGetMainActivity();
	jclass mainclass = env->FindClass("org/oxygine/snake/MainActivity");
	jmethodID loadAd = env->GetMethodID(mainclass, "loadAd", "()V");
	env->CallVoidMethod(activity, loadAd);
#endif
}
