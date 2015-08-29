#include "WrapperManager.h"
#include "cocos2d.h"

#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
#import "IOSWrapper.h"
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include "JniWrapper.h"
#endif


WrapperManager* WrapperManager::_instanceWrapperManager = NULL;


WrapperManager::WrapperManager()
{
}

WrapperManager* WrapperManager::getInstance()
{
	if (! _instanceWrapperManager){
		_instanceWrapperManager = new WrapperManager();
	}
	return _instanceWrapperManager;
}

void WrapperManager::openUrl(const char *Url)
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    IOSWrapper::openUrl(Url);
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    JniWrapper::openUrl(Url);
#else
    CCLOG("----->%s, Url is %s", __FUNCTION__, Url);
#endif
    
}
