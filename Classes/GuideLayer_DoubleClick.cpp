#include "GuideLayer_DoubleClick.h"
#include "font.h"
#include "LFStrings.h"
#include "LFToast.h"
#include "ServerTime.h"
#include "LFLayer.h"
#include "GuideLayer_Scale.h"
#include "BaseScense.h"
#include "History.h"

// 父类把手指和点击效果都准备好的，但是都是全透明的，到时子类用就是了
GuideLayer_DoubleClick::GuideLayer_DoubleClick(int seasonId, int sectionId):BaseGuideLayer(seasonId,sectionId)
{
// 	doubleClickGuided = false;
// 	mBaseContentLayerTouchEff = false;
// 	lastClickDownTime.time = 0;
// 	lastPos = ccp(-100,-100);
// 	CCPoint dest = ccp(315,mWinSize.height - 137);
// 	lable = CCLabelTTF::create(LFStrings::getValue("tryDoubleClicke").c_str(),fontStr_katong,30.0f,CCSizeMake(0,0),kCCTextAlignmentCenter);
// 	addChild(lable); 
// 	lable->setColor(ccc3(0,0,0));
// 	mask = CCSprite::create("guide_doubleclick.png");
// 	addChild(mask,-1);
// 	mask->setPosition(ccp(1024/2,768/2));
// 	// 不mask要了
// 	mask->setVisible(false);
// 
// 	lable->setPosition(ccp(dest.x + 3 + 6,dest.y + 23 + 13 - 77 - 18));
// 	lable->setOpacity(0);
// 	lable->runAction(CCSequence::create(
// 		CCDelayTime::create(0.2f),
// 		CCFadeTo::create(1.0f,255),
// 		CCDelayTime::create(5.0f),
// 		CCFadeTo::create(1.0f,0),
// 		NULL));
// 
// 	// 下面的不要了
// 	mBaseContentLayerTouchEff = true;
// 	return;
// 
// 	mFinger->stopAllActions();
// 	mFingerLight->stopAllActions();
// 
// 	CCPoint endPos = ccp(279,571);
// 	CCPoint starPos = ccp(415,411);
// 	mFingerLight->setPosition(endPos);
// 
// 	mFinger->setPosition(starPos);
// 	mFingerLight->setVisible(false);
// 
// 	mFinger->runAction(CCSequence::create(
// 		CCDelayTime::create(0.2f),
// 		CCFadeTo::create(0.4f,255),
// 		NULL
// 		));
// 	mFinger->runAction(CCSequence::create(
// 		CCDelayTime::create(0.2f),
// 		CCRepeat::create(
// 			CCSequence::create(
// 				CCSpawn::create(
// 					CCFadeTo::create(0.3f,255),
// 					CCMoveTo::create(0.5f,ccpAdd(endPos,ccp(Finger_Pos_Offset_X,Finger_Pos_Offset_Y))),
// 					NULL
// 				),
// 				CCCallFuncN::create(this,SEL_CallFuncN(&GuideLayer_DoubleClick::fingerPosReady)),
// 				CCDelayTime::create(1.2f),
// 				CCFadeTo::create(0.01f,0),
// 				CCMoveTo::create(0.01f,starPos),
// 				CCDelayTime::create(0.5f),
// 				NULL),
// 		-1),
// 	NULL));

	//add by jj
	//屏蔽缩放教学
// 	if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32 && !CCUserDefault::sharedUserDefault()->getBoolForKey(Scale_Teach_Is_Done))
// 	{
// 		GuideLayer_Scale* pLayer = GuideLayer_Scale::create();
// 		this->addChild(pLayer, 10);
// 	}
// 	else
	{
		BaseScense::getCurrentScene()->setCollisionEnable(true);
	}
}

void GuideLayer_DoubleClick::fingerPosReady(CCNode* finger)
{
// 	mFingerLight->runAction(CCSequence::create(
// 		CCShow::create(),
// 		CCFadeTo::create(0.2f,255),
// 		CCFadeTo::create(0.2f,100),
// 		CCFadeTo::create(0.2f,255),
// 		CCFadeTo::create(0.2f,100),
// 		CCFadeTo::create(0.2f,255),
// 		CCFadeTo::create(0.2f,100),
// 		CCHide::create(),
// 		NULL));
}

void GuideLayer_DoubleClick::onEnter()
{
 	BaseGuideLayer::onEnter();
// 
// 	// 下面的不要了
// 	return;
// 
// 	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this,-127,true);
}
void GuideLayer_DoubleClick::onExit()
{
	BaseGuideLayer::onExit();
//	CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
}

// 必须重写，因为父类调用了setTouchEnabled
// 就会把现在的触摸禁掉了
void GuideLayer_DoubleClick::pause()
{
	//setTouchEnabled(false);
//	SpriteHelper::pauseNodeAndChildren(this);
}
void GuideLayer_DoubleClick::resume()
{
	//setTouchEnabled(true);
//	SpriteHelper::resumeNodeAndChildren(this);
}

bool GuideLayer_DoubleClick::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
// 	CCPoint glPoint = pTouch->getLocation();
// 
// 	// mask外面，可以点击
// 	if (!mask->boundingBox().containsPoint(mask->convertToNodeSpace(glPoint)))
// 	{
// 		return false;
// 	}
// 
// 	if (doubleClickGuided)
// 	{
// 		return false; // 
// 	}
// 
// 
// 	bool doubleClicked = false;
// 	if (lastClickDownTime.time > 0)
// 	{//代表之前已经点过一次了
// 		timeb temp = currentTime();
// 		if ( (temp.time - lastClickDownTime.time)*1000 + temp.millitm - lastClickDownTime.millitm <= CLICK_Trigger*1000)
// 		{
// 			if ( abs(lastPos.x - glPoint.x) <= DOUBLE_CLICK_DIS && abs(lastPos.y - glPoint.y) <= DOUBLE_CLICK_DIS)
// 			{
// 				doubleClicked = true;
// 				doubleClickDown(glPoint);
// 				lastClickDownTime.time = 0;
// 			}
// 		}
// 	}
// 	lastPos = glPoint;
// 
// 	if (!doubleClicked)
// 	{
// 		lastClickDownTime = currentTime();
// 	}

	return true; // 屏蔽
}
void GuideLayer_DoubleClick::doubleClickDown(CCPoint glPoint)
{
// 	CCPoint maskPoint = mask->convertToNodeSpace(glPoint);
// 	if (maskPoint.x > 119 && maskPoint.y > 542 &&
// 		maskPoint.x < 540 && maskPoint.y < 600
// 		)
// 	{
// 		mFinger->stopAllActions();
// 		mFinger->setVisible(false);
// 		mFingerLight->setVisible(false);
// 		mFingerLight->setPosition(ccp(-100,-100));
// 
// 		doubleClickGuided = true;
// 		mBaseContentLayer->postLfDoubleClickDown(glPoint);
// 		scaleBigRunning();
// 	}
}
void GuideLayer_DoubleClick::lfClick(CCPoint glPoint)
{
}

void GuideLayer_DoubleClick::scaleBigRunning()
{
// 	lable->stopAllActions();
// 	lable->runAction(CCFadeTo::create(0.2f,0));
// 	mask->setVisible(false);
// 	schedule(schedule_selector(GuideLayer_DoubleClick::showCanDrag),1.0f);
}

void GuideLayer_DoubleClick::showCanDrag(float time)
{
// 	mBaseContentLayerTouchEff = true; // 游戏里面可以点击了
// 	unschedule(schedule_selector(GuideLayer_DoubleClick::showCanDrag));
// 	lable->setString(LFStrings::getValue("youcandrag").c_str());
// 	lable->setVisible(true);
// 	lable->runAction(CCSequence::create(
// 		CCFadeTo::create(0.5f,255),
// 		CCDelayTime::create(5.0f),
// 		CCFadeTo::create(0.5f,0),NULL));
}

void GuideLayer_DoubleClick::baseContentLayerEffctiveClicked(CCNode* node)
{
}