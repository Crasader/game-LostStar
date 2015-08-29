#include "CrossPlatformSizeManager.h"

CrossPlatformSizeManager* CrossPlatformSizeManager::manager;

CrossPlatformSizeManager::CrossPlatformSizeManager()
{
	//CrossPlatform_defaultWinSize = CCDirector::sharedDirector()->getWinSize();

	//CrossPlatform_defaultWinSize = CCSizeMake(480,320);
	//CCEGLView *view = &CCEGLView::sharedOpenGLView();
	//CrossPlatform_currentWinSize = view->getFrameSize();

	CrossPlatform_defaultWinSize = CCDirector::sharedDirector()->getWinSize();
	CrossPlatform_currentWinSize = CCDirector::sharedDirector()->getWinSize();
	factor = getScaleFactor();

	CCLog("%s%f%s%f%s","CrossPlatformSizeManager--->CrossPlatform_defaultWinSize=(",CrossPlatform_defaultWinSize.width,",",CrossPlatform_defaultWinSize.height,")");
	CCLog("%s%f%s%f%s","CrossPlatformSizeManager--->CrossPlatform_currentWinSize=(",CrossPlatform_currentWinSize.width,",",CrossPlatform_currentWinSize.height,")");
	CCLog("%s%f","CrossPlatformSizeManager--->get the screen scale factor:",factor);
}

float CrossPlatformSizeManager::getScaleFactor()
{
	float factor_width = getCurrentWinSize().width/CrossPlatform_defaultWinSize.width;
	float factor_height = getCurrentWinSize().height/CrossPlatform_defaultWinSize.height;
	return factor_width>factor_height?factor_height:factor_width;
}
CrossPlatformSizeManager* CrossPlatformSizeManager::getManager()
{
	if (manager == NULL)
	{
		manager = new CrossPlatformSizeManager();
	}
	return manager;
}

CCSize CrossPlatformSizeManager::getCurrentWinSize()
{
	return CrossPlatform_currentWinSize;
}
CCSize CrossPlatformSizeManager::makeSize(CCSize size)
{
	return CCSizeMake(size.width*factor,size.height*factor);
}
CCPoint CrossPlatformSizeManager::makePoint(CCPoint point)
{
	return CCPointMake(point.x * factor,point.y * factor);
}
CCSprite* CrossPlatformSizeManager::makeSprite(CCSprite* sprite)
{
	//CCSize rectBefore = sprite->getContentSize();
	//CCSize boxBefore = sprite->boundingBox().size;
	//CCLog("%s%f%s%f","CrossPlatformSizeManager::makeSprite-->contentBefore width=",rectBefore.width," height=",rectBefore.height);
	//CCLog("%s%f%s%f","CrossPlatformSizeManager::makeSprite-->boxBefore width=",boxBefore.width," height=",boxBefore.height);
	sprite->setScale(factor);
	//rectBefore = sprite->getContentSize();
	//boxBefore = sprite->boundingBox().size;
	//CCLog("%s%f%s%f","CrossPlatformSizeManager::makeSprite-->contentNow width=",rectBefore.width," height=",rectBefore.height);
	//CCLog("%s%f%s%f","CrossPlatformSizeManager::makeSprite-->boxNow width=",boxBefore.width," height=",boxBefore.height);
	return sprite;
}
CCNode* CrossPlatformSizeManager::makeNode(CCNode* node)
{
	node->setScale(factor);
	return node;
}
//获得大小转换的因子;
float CrossPlatformSizeManager::getFactor()
{
	return factor;
}
