#include "BaseContentWrapLayer.h"
#include "CommonDefine.h"
#include "BaseScense.h"
#include "ScrollController.h"

BaseContentWrapLayer::BaseContentWrapLayer(BaseScense* scene)
{
	init();
	mWrapper = NULL;
	setWapper(CCLayer::create());
	//add by jj
	//屏蔽缩放
	mFixedScaling = true;

	mCurrentInitCenterScale = 1.0f;
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	mCurrentInitCenterScalePoint = ccp(winSize.width/2,winSize.height/2);
	mCurrentInitPos = CCPointZero;
	mCurrentFixScale = 1.0f;
	wrapedLayer = NULL;
	mBaseScene = scene;
	mVCalculate = NULL;

	createVCalculate();

	mScrollControler = NULL;
	ScrollController* pS = new ScrollController();
	pS->autorelease();
	setScrollControlerr(pS);

	setTouchEnabled(true);
}
BaseContentWrapLayer::~BaseContentWrapLayer()
{
	delete mVCalculate;
}

void BaseContentWrapLayer::createVCalculate()
{
	mVCalculate = new VelocityCalculate();
}

void BaseContentWrapLayer::setWapper(CCLayer* pWrapper)
{
	if (!pWrapper)
	{
		return;
	}
	mWrapper = pWrapper;
	addChild(mWrapper);
}

void BaseContentWrapLayer::setScrollControlerr(ScrollController* pScrollController)
{
	if (!pScrollController)
	{
		return;
	}
	mScrollControler = pScrollController;
	addChild(pScrollController);
}

void BaseContentWrapLayer::ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent)
{
	if (!mBaseScene->getCollisionEnable())
	{
		return;
	}

	CCTouch* touch = (CCTouch*)(*(pTouches->begin()));
    if(touch->getID() != 0 || pTouches->count() > 1)
        return;
    
	mScrollControler->stopScrolling();
	mVCalculate->clearRecord();
	mLastTouchPos = touch->getLocation();
}
void BaseContentWrapLayer::ccTouchesMoved (CCSet *pTouches, CCEvent *pEvent)
{
	if (!mBaseScene->getCollisionEnable())
	{
		return;
	}

	if (mFixedScaling)
	{
		return;
	}
	
	CCTouch* touch = (CCTouch*)(*(pTouches->begin()));
    if(touch->getID() != 0 || pTouches->count() > 1)
        return;
    
	CCPoint currentPos = touch->getLocation();
	mVCalculate->addTouchMoveRecord(currentPos);

	float x_offset = currentPos.x - mLastTouchPos.x;
	float y_offset = currentPos.y - mLastTouchPos.y;

	setPositionX(getPositionX() + x_offset);
	setPositionY(getPositionY() + y_offset);

	mLastTouchPos = currentPos;

	float min_x;
	float min_y;
	float max_x;
	float max_y;
	getBoarderPos(min_x,min_y,max_x,max_y);

	if (getPositionX() < min_x)
	{
		setPositionX(min_x);
	}
	if (getPositionX() > max_x)
	{
		setPositionX(max_x);
	}

	if (getPositionY() < min_y)
	{
		setPositionY(min_y);
	}
	if (getPositionY() > max_y)
	{
		setPositionY(max_y);
	}
	

}
void BaseContentWrapLayer::ccTouchesEnded(cocos2d::CCSet* touches, cocos2d::CCEvent* event)
{

	if (!mBaseScene->getCollisionEnable())
	{
		return;
	}

	CCTouch* touch = (CCTouch*)(*(touches->begin()));
    if(touch->getID() != 0 || touches->count() > 1)
        return;
    
	if (mFixedScaling)
		return;


	ScrollConfig config;
	config.decelerate = 0.95f;
	getBoarderPos(config.minPosX,config.minPosY,config.maxPosX,config.maxPosY);
	config.scrollSpeed = mVCalculate->getCurrentInstantaneousSpeed();
	config.toScroll = this;
	mScrollControler->scrollWithNoRebound(config);
}

void BaseContentWrapLayer::getBoarderPos(float& minX,float& minY,float& maxX,float& maxY)
{
	CCSize winsize = CCDirector::sharedDirector()->getWinSize();
	float x_offset = (winsize.width - 1024)/2.0f;

	maxX = (getContentSize().width * getScaleX() - winsize.width )/2;
	if (getScaleX() > 1.0f)
	{
		maxX -= x_offset;
	}
	minX = winsize.width/2 - getContentSize().width * getScaleX()/2 + x_offset;
	maxY = (getContentSize().height * getScaleY() - winsize.height )/2;
	minY = winsize.height/2 - getContentSize().height * getScaleY()/2;
}

BaseContentWrapLayer* BaseContentWrapLayer::createLayer(BaseScense* scene)
{
	BaseContentWrapLayer* layer = new BaseContentWrapLayer(scene);
	layer->autorelease();
	return layer;
}
BaseContentLayer* BaseContentWrapLayer::getBaseContentLayer()
{
	return wrapedLayer;
}

void BaseContentWrapLayer::setBaseContentLayer(BaseContentLayer* layer,int zOrder)
{
	CCAssert(layer!=NULL,"");

// 	if (wrapedLayer != NULL)
// 	{
// 		wrapedLayer->removeAllChildrenWithCleanup(true);
// 	}
	wrapedLayer = layer;
//	mWrapper->addChild(wrapedLayer,zOrder);
}

void BaseContentWrapLayer::setWrapperScaleWithFixPoint(float scale,CCPoint glPoint,bool animation /*= true*/)
{
	setWrapperScaleWithFixPoint(scale,glPoint,(animation?0.5f:-1));
}

void BaseContentWrapLayer::setWrapperScaleWithFixPoint(float scale,CCPoint glPoint,float animation_time /* = 0.5f */)
{
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	if (scale <= 1.0f)
	{
		scale = 1.0f;
		glPoint = CCPointZero;
		mCurrentFixScale = scale;
	}
	mCurrentFixScale = scale;

	CCPoint wrapperNativePoint = convertToNodeSpace(glPoint);
	CCPoint bgNativePoint = mBaseScene->getBackgroudLayer()->convertToNodeSpace(glPoint);

	// 这里有个Bug，就是初始缩放和这里缩放是不同步的
	CCPoint vec2Center = ccpSub(ccp(winSize.width/2,winSize.height/2),wrapperNativePoint);
	vec2Center = ccpMult(vec2Center,mCurrentFixScale-1.0f);


	CCPoint vec2CenterForBg = ccpSub(ccp(winSize.width/2,winSize.height/2),bgNativePoint);
	vec2CenterForBg = ccpMult(vec2CenterForBg,mCurrentFixScale-1.0f);

	// 背景也要同步缩放
	stopAllActions();
	if (animation_time > 0)
	{
		mFixedScaling = true;
		runAction(CCSequence::create(
			CCScaleTo::create(animation_time,scale),
			CCCallFuncN::create(this,SEL_CallFuncN(&BaseContentWrapLayer::scalingEnd)),
			NULL));
		runAction(CCMoveTo::create(animation_time,vec2Center));
	}else
	{
		setScale(scale);
		setPosition(vec2Center);
	}
}

void BaseContentWrapLayer::setWrapperInitScaleWithCenterPoint(float scale,CCPoint newNativeCenterPosIn1024_768,bool annimation /* = true */)
{
	CCAssert(annimation == false,"还不支持");


	CCSize winSize = CCDirector::sharedDirector()->getWinSize();

//	scale = 1.5f;
//	newNativeCenterPosIn1024_768.y = scale*(newNativeCenterPosIn1024_768.y - winSize.height/2+350.0f);
//	CCLog("newNativeCenterPosIn1024_768 pos is ------------------>%f, %f", newNativeCenterPosIn1024_768.x, newNativeCenterPosIn1024_768.y);

	// 需要把1024 768 的坐标，放大到当前的窗口坐标大小
	CCPoint newNativeCenterPos = ccp(newNativeCenterPosIn1024_768.x / 1024 * winSize.width,newNativeCenterPosIn1024_768.y/768 * winSize.height);
	
	if (scale <= 1.0f)
	{
		scale = 1.0f;
		newNativeCenterPos = ccp(winSize.width/2,winSize.height/2);
	}

	mCurrentInitCenterScalePoint = newNativeCenterPos;


	if (newNativeCenterPos.x < winSize.width/2/scale)
	{
		CCLog("Waring : newNativeCenterPos.x is too small [%f < %f], we will see the black boarder!" ,newNativeCenterPos.x , winSize.width/2/scale);
	}
	//if (newNativeCenterPos.x > winSize.width/2 + winSize.width/2/scale)
	if (newNativeCenterPos.x > winSize.width - winSize.width/2/scale)
	{
		CCLog("Waring : newNativeCenterPos.x is too big [%f > %f] , we will see the black boarder!" ,newNativeCenterPos.x , winSize.width - winSize.width/2/scale);
	}
	if (newNativeCenterPos.y < winSize.height/2/scale)
	{
		CCLog("Waring : newNativeCenterPos.y is too small [%f < %f] , we will see the black boarder!" ,newNativeCenterPos.y , winSize.height/2/scale);
	}
	//if (newNativeCenterPos.y > winSize.height/2 + winSize.height/2/scale)
	if (newNativeCenterPos.y > winSize.height - winSize.height/2/scale)
	{
		CCLog("Waring : newNativeCenterPos.y is too big [%f > %f] , we will see the black boarder!" ,newNativeCenterPos.y , winSize.height - winSize.height/2/scale);
	}

	mCurrentInitCenterScale = scale;

	CCPoint vec2Center = ccpSub(ccp(winSize.width/2,winSize.height/2),newNativeCenterPos);
	vec2Center = ccpMult(vec2Center, scale);

	mCurrentInitPos = vec2Center;

	stopAllActions();
	if (annimation)
	{
		mWrapper->runAction(CCScaleTo::create(0.5f,scale));
		mWrapper->runAction(CCMoveTo::create(0.5f,vec2Center));
	}else
	{
		mWrapper->setScale(scale);
		mWrapper->setPosition(vec2Center);
	}
}

void BaseContentWrapLayer::scalingEnd(CCNode* selfNode)
{
	mFixedScaling = false;
	CCNotificationCenter::sharedNotificationCenter()->postNotification(MSG_FIXED_SCALE_END,this);
}