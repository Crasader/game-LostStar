#include "WaitTimeToClickLayer.h"
#include "cocos2d.h"
#include "ResManager.h"
#include "Music.h"
#include "CommonDefine.h"
#include "AchievementManager.h"
#include "VersionControl.h"

USING_NS_CC;

#define Tag_Timer 789898
#define Tag_Timer_Boarder 799999


WaitTimeToClickLayer::WaitTimeToClickLayer(void)
{
	mSoundId = 0;
	isDone = true;
	mScreenSize = CCDirector::sharedDirector()->getWinSize();
}

void WaitTimeToClickLayer::viewWaitingAnimation()
{
#if NEED_GAME_CD_TIME == 0
	// 2014-04-22修改目前版本不需要了
	return;
#endif


	isDone = false;
	// 提前载入，防止卡顿
	CCTextureCache::sharedTextureCache()->addImage(ResManager::getManager()->getSharedFilePath("timer_wave.png").c_str());

	// 外壳
	CCSprite* clockBoarder = CCSprite::create(ResManager::getManager()->getSharedFilePath("timer_1.png").c_str());
	addChild(clockBoarder, 128 ,Tag_Timer_Boarder);
	clockBoarder->setPosition(CCPointMake(mScreenSize.width/2, 690));

	// 内芯
	CCSprite* timerSprite = CCSprite::create(ResManager::getManager()->getSharedFilePath("timer_2.png").c_str());
	timerSprite->setOpacity(255);
	CCProgressTimer* timer = CCProgressTimer::create(timerSprite);
	timer->setType(kCCProgressTimerTypeRadial);
	timer->setPercentage(0);
	timer->setPosition(ccp(30,29));
	clockBoarder->addChild(timer,0,Tag_Timer);

	mSoundId = Music::sharedMusicPlayer()->playEffectSound("dida_L1_5.wav");

	timer->runAction(CCSequence::create(
		CCProgressTo::create(TouchDestroyTimeGap,100.0f),
		CCCallFuncN::create(this,SEL_CallFuncN(&WaitTimeToClickLayer::waitingDone)),
		NULL
		));

}
void WaitTimeToClickLayer::viewWaitingPrompt()
{
#if NEED_GAME_CD_TIME == 0
	// 2014-04-22修改目前版本不需要了
	return;
#endif



	int tempTag = 123321;
	CCNode* boarder = getChildByTag(Tag_Timer_Boarder);
	if (boarder != NULL) {

		if (boarder->getChildByTag(tempTag) != NULL)
		{// 还在动画，直接返回
			return;
		}

		AchievementManager::getInstance()->alarmClockBi();

		Music::sharedMusicPlayer()->playEffectSound("dudu.wav");

		float timeCost0 = 0.05f;
		float timeCost1 = 0.05f;
		float timeCost2 = 0.03f;
		float timeCost3 = 0.025f;
		float timeCost4 = 0.015f;
		CCFiniteTimeAction* rotate0 = CCSequence::create(CCRotateTo::create(timeCost0,30),CCRotateTo::create(timeCost0,0),CCRotateTo::create(timeCost0,-30),CCRotateTo::create(timeCost0,0),NULL);
		CCFiniteTimeAction* rotate1 = CCSequence::create(CCRotateTo::create(timeCost1,30),CCRotateTo::create(timeCost1,0),CCRotateTo::create(timeCost1,-30),CCRotateTo::create(timeCost1,0),NULL);
		CCFiniteTimeAction* rotate2 = CCSequence::create(CCRotateTo::create(timeCost2,20),CCRotateTo::create(timeCost2,0),CCRotateTo::create(timeCost2,-20),CCRotateTo::create(timeCost2,0),NULL);
		CCFiniteTimeAction* rotate3 = CCSequence::create(CCRotateTo::create(timeCost3,10),CCRotateTo::create(timeCost3,0),CCRotateTo::create(timeCost3,-10),CCRotateTo::create(timeCost3,0),NULL);
		CCFiniteTimeAction* rotate4 = CCSequence::create(CCRotateTo::create(timeCost4,5),CCRotateTo::create(timeCost4,0),CCRotateTo::create(timeCost4,-5),CCRotateTo::create(timeCost4,0),NULL);

		boarder->runAction(CCSequence::create(rotate0,rotate1,rotate2,rotate3,rotate4,NULL));

		float delay = 0.2f;
		for (int i=0;i<4;i++)
		{
			CCSprite* wave = CCSprite::create(ResManager::getManager()->getSharedFilePath("timer_wave.png").c_str());
			boarder->addChild(wave,0,tempTag);
			wave->setPosition(ccp(boarder->getContentSize().width/2,boarder->getContentSize().height/2));
			wave->setScale(0.0f);

			// 整体变透明
			wave->setOpacity(200);
			// 旋转
			wave->runAction(CCSequence::create(CCDelayTime::create(delay*i),CCRepeat::create(CCRotateBy::create(3.0f,360),-1),NULL));
			// 变透明
			wave->runAction(CCSequence::create(CCDelayTime::create(delay*i),CCFadeTo::create(0.8f,0),NULL));
			// 放大
			wave->runAction(CCSequence::create(CCDelayTime::create(delay*i),CCScaleTo::create(1.0f,1.5f),CCCallFuncN::create(this,SEL_CallFuncN(&WaitTimeToClickLayer::removeNode)),NULL));
		}
	}
}
void WaitTimeToClickLayer::waitingDone(CCNode* null_obj)
{
	isDone = true;
	setWaitingSoundPause(true);

	CCNode* timerBoarder = getChildByTag(Tag_Timer_Boarder);
	if (timerBoarder == NULL)
		return;
	removeChild(timerBoarder, true);
	Music::sharedMusicPlayer()->playEffectSound("ding.wav");
}
void WaitTimeToClickLayer::setWaitingSoundPause(bool pause)
{
	CCNode* boarder = getChildByTag(Tag_Timer_Boarder);
	CCProgressTimer* timer = (boarder != NULL ? (CCProgressTimer*)boarder->getChildByTag(Tag_Timer) : NULL);
	if (pause)
	{
		Music::sharedMusicPlayer()->pauseEffectSound(mSoundId);
	}else
	{
		if (timer == NULL) {
			return;
		}
		Music::sharedMusicPlayer()->resumeEffectiveSound(mSoundId);
	}

}

bool WaitTimeToClickLayer::isWaitingDone()
{
	return isDone;
}

void WaitTimeToClickLayer::setPause(bool pause /*= true*/)
{
	setWaitingSoundPause(pause);
	CCNode* timerBoarder = getChildByTag(Tag_Timer_Boarder);
	if (timerBoarder != NULL && timerBoarder->getChildByTag(Tag_Timer) != NULL)
	{
		if (pause)
		{
			timerBoarder->getChildByTag(Tag_Timer)->pauseSchedulerAndActions();
		}else
		{
			timerBoarder->getChildByTag(Tag_Timer)->resumeSchedulerAndActions();
		}
	}
}

void WaitTimeToClickLayer::removeNode(CCNode* node)
{
	if (node != NULL)
	{
		node->removeFromParent();
	}
}

//void WaitTimeToClickLayer::frezey()
//{
//	setPause(true);
//}

WaitTimeToClickLayer::~WaitTimeToClickLayer(void)
{
	setWaitingSoundPause(true);
}
