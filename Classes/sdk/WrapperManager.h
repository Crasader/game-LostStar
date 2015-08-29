
#pragma once

#include "cocos2d.h"
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include <jni.h>
#endif

class WrapperManager
{
public:
	static WrapperManager* getInstance();
private:
	WrapperManager();
	static WrapperManager* _instanceWrapperManager;

    
public:
    void openUrl(const char* Url);
    
};