#pragma once
#include <string>


#if defined(__ANDROID__)
#include <jni.h>
#include <android_native_app_glue.h>
#endif


extern struct android_app* __state;

class Advert{
public:
	enum JAVA_FUNC{
		LOAD_BANNER,
		LOAD_INTERSTITIAL,
		SHOW_AD,
		AD_SHOWING
		
	};

	static bool CallJava(JAVA_FUNC func)
	{
		switch (func){
		case LOAD_BANNER:
			return doJavaThingy("loadBanner");
			break;
		case LOAD_INTERSTITIAL:
			return doJavaThingy("loadInterstitial");
			break;
		case SHOW_AD:
			return doJavaThingy("showAd");
			break;
		case AD_SHOWING:
		//	return doJavaThingy("adShowing", true);
			return false;
			break;
		}

		return false;
	}

	static void isClosed() {

	}
private:
	static bool doJavaThingy(std::string javaFunc, bool hasReturn = false)
	{
		bool returnValue = false;
#if defined(__ANDROID__)
		// Get the android application's activity.

		ANativeActivity* activity = __state->activity;

		JavaVM* jvm = __state->activity->vm;

		JNIEnv* env = NULL;

		jvm->GetEnv((void **)&env, JNI_VERSION_1_6);

		jint res = jvm->AttachCurrentThread(&env, NULL);

		if (res == JNI_ERR)

		{

			// Failed to retrieve JVM environment

			return returnValue;

		}

		jclass clazz = env->GetObjectClass(activity->clazz);

		jmethodID methodID = env->GetMethodID(clazz, javaFunc.c_str(), "()V");
		if (hasReturn)
			env->CallVoidMethod(activity->clazz, methodID);
		else {
			jboolean jreturn = env->CallBooleanMethod(activity->clazz, methodID);
			returnValue = (bool)jreturn;
		}
		jvm->DetachCurrentThread();
		#endif
		return returnValue;
	}
};