
#include "JniWrapper.h"
#include "sdk/FXPlugin/FXPluginManager.h"

#define CLASS_NAME  "org/FXUtils/JniWrapper"

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include "ResManager.h"


JniWrapper::JniWrapper()
{
}

JniWrapper::~JniWrapper()
{
}

void JniWrapper::openUrl(const char* url)
{
	JniMethodInfo t;
	if (JniHelper::getStaticMethodInfo(t, CLASS_NAME, "openUrl", "(Ljava/lang/String;)V")) 
	{
		jstring jUrl = t.env->NewStringUTF(url); 
		t.env->CallStaticBooleanMethod(t.classID, t.methodID, jUrl);
		t.env->DeleteLocalRef(jUrl);
	}
}


// JNIEXPORT void JNICALL Java_com_camellia_starfall_JniWrapper_login(JNIEnv *env, jobject thiz)
// {
// 
// }

 JNIEXPORT jint JNICALL Java_org_FXUtils_JniWrapper_getCurSettingLanguageTypeNative(JNIEnv *env, jobject thiz)
 {
	return (jint)ResManager::getManager()->getLanguageType();
 }
#endif