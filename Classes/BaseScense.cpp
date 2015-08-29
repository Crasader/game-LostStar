//
//  BaseScense.cpp
//  demo-box2d
//
//  Created by Davidloeng loeng on 12-6-22.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#include <iostream>
#include "BaseScense.h"
#include "GameScenseDefine.h"
#include "CongratulationLayer.h"
#include "GameLayerFactory.h"
#include "GameInforEditer.h"
#include "MainFrameLayer.h"
#include "LevelManager.h"
#include "Runtime.h"
#include "BackgroudLayerFactory.h"
#include "ShadowLayer.h"
#include <cmath>
#include "AchievementManager.h"
#include "GuideLayerFactory.h"
#include "BaseContentWrapLayer.h"
#include "WaitTimeToClickLayer.h"
#include "LFAlerts.h"
#include "Acount.h"
#include "net/NetworkWaitingLayer.h"
//#include "VersionControl.h"
//#include "CurtainSideLayer.h"

static BaseScense* s_scene = NULL;


BaseScense::BaseScense(int iSceneIndex)
{
	mSceneIndex = iSceneIndex;
	s_scene = this;
	m_bCollisionEnable = false;
	mContentLayerWapper = NULL;
	mMenuLayer = NULL;
	mContentLayer = NULL;
	needUnlockAction = false;
	mBackgroundLayer = NULL;
	notificationLayer = NULL;
	guideLayer = NULL;

	initAllGameLayerNeedLayers();
}

BaseScense::~BaseScense()
{
	//	mBackgroundLayer->release();
//	CC_SAFE_RELEASE(mContentLayer);
//	CC_SAFE_RELEASE(mMenuLayer);

	CCNotificationCenter::sharedNotificationCenter()->removeAllObservers(this);
}


BaseScense* BaseScense::getCurrentScene()
{
	return s_scene;
}

void BaseScense::initAllGameLayerNeedLayers()
{
	mContentLayerWapper = BaseContentWrapLayer::createLayer(this);
	setContentLayerWapper(mContentLayerWapper);

	mMenuLayer = new MenuLayer(mSceneIndex);
	mMenuLayer->autorelease();
	mMenuLayer->setBaseScene(this);
	addChild(mMenuLayer, 129);

	GameLayerFactory* layerFactory = GameLayerFactory::getInstance();
	std::pair<int, int> info = layerFactory->getLevelFormat(mSceneIndex);
	seasonId = info.first;
	sectionId = info.second;
	
	//添加底部星星评价
	const ConfigSectionInfo* curLevelinfo = GameLayerFactory::getInstance()->getConfigSectionInfo(seasonId, sectionId);
	StarState* pBar = StarState::create(curLevelinfo->iPefect_Gold, curLevelinfo->iBest_Gold);
	pBar->setPosition(ccp(0, -g_iTaiJieHight));
	mContentLayerWapper->addChild(pBar, 10001, Tag_StarStateBar);

	mMenuLayer->setStarState(pBar);

	mBackgroundLayer = BackgroudLayerFactory::createBgLayer(seasonId, sectionId);

	notificationLayer = AchievementNotification::create();
	addChild(notificationLayer, 128,Tag_Layer_Notification); // 放在最上层
 
	guideLayer = GuideLayerFactory::getFactory()->getGuideLayer(seasonId,sectionId);
	guideLayer->setBaseScene(this);
	setGuideLayer(guideLayer);
	guideLayer->setTouchEnabled(false);
	guideLayer->setVisible(false);

	ShadowLayer* shadowLayer = new ShadowLayer();
	shadowLayer->autorelease();

	WaitTimeToClickLayer* waitLayer = new WaitTimeToClickLayer();
	waitLayer->autorelease();
	addChild(waitLayer,1,Tag_Layer_WatingLayer);

	CCLayer* bgCover = CCLayer::create();
	mContentLayerWapper->getWrapper()->addChild(bgCover,-1,Tag_Layer_Background_Cover);
	
	mContentLayerWapper->getWrapper()->addChild(shadowLayer,0,Tag_Layer_shadow);
	mContentLayerWapper->addChild(mBackgroundLayer,-1,Tag_Layer_Background);
}


WaitTimeToClickLayer* BaseScense::getWaitingLayer()
{
	return (WaitTimeToClickLayer*)getChildByTag(Tag_Layer_WatingLayer);
}

CCLayer* BaseScense::getBackgroudCoverLayer()
{
	return (CCLayer*)mContentLayerWapper->getWrapper()->getChildByTag(Tag_Layer_Background_Cover);
}

void BaseScense::setMenuLayer(MenuLayer* pMenuLayer)
{
	if (!pMenuLayer)
	{
		return;
	}
	mMenuLayer = pMenuLayer;
	mMenuLayer->setBaseScene(this);
	addChild(mMenuLayer, 129);
}

void BaseScense::setBackgroudLayer(BaseBackgroundLayer* pLayer)
{
	mBackgroundLayer = pLayer;
}

CCLayer* BaseScense::getBackgroudLayer()
{
	return mBackgroundLayer;
}

BaseContentWrapLayer* BaseScense::getBaseContentWrapLayer()
{
	return mContentLayerWapper;
}

BaseGuideLayer* BaseScense::getGuideLayer()
{
	return guideLayer;
}

void BaseScense::bombed(int currentBombPower, int maxPower)
{
    mBackgroundLayer->bombed(currentBombPower, maxPower);
}

void BaseScense::freezyBgLayer()
{
//    mBackgroundLayer->freezyLayer();
    //避免素材路径已经切换了，但是还有schedule还没有取消。
    //也就避免了对话框弹出。
    mBackgroundLayer->unscheduleAllSelectors();
}

void BaseScense::removeChildByTag(int tag, bool cleanup)
{
	// 现在wrapper中找移除的
	mContentLayerWapper->getWrapper()->removeChildByTag(tag,cleanup);
	CCScene::removeChildByTag(tag,cleanup);
}

void BaseScense::setContentLayerWapper(BaseContentWrapLayer* contentLayerWapper, bool bAdd/* = true*/)
{
	if (!contentLayerWapper)
	{
		return;
	}
	mContentLayerWapper = contentLayerWapper;
	if (bAdd)
	{
		addChild(mContentLayerWapper, 0, Tag_Layer_BaseContetWapper);
	}
}

void BaseScense::setGuideLayer(BaseGuideLayer* pBaseGuideLayer)
{
	if (!guideLayer)
	{
		return;
	}
	guideLayer = pBaseGuideLayer;
	mContentLayerWapper->getWrapper()->addChild(guideLayer, 1002);
}

void BaseScense::setContentLayer(BaseContentLayer* contentLayer)
{
	mContentLayer = contentLayer;
	mContentLayerWapper->setBaseContentLayer(mContentLayer);
    
    mMenuLayer->setBaseContentLayer(mContentLayer);
    
    mBackgroundLayer->setBaseContentLayer(mContentLayer);

	guideLayer->setBaseContentLayer(mContentLayer);
//	guideLayer->startGuide();
}

void BaseScense::congratulation(){
    //current layer sleep
//	int oldScore = GameInforEditer::getGameInfor()->getTotalFreeScore();
    mContentLayer->freezeLayer();

	GameLayerFactory* layerFactory = GameLayerFactory::getInstance();
	std::pair<int, int> info1 = layerFactory->getLevelFormat(mSceneIndex);
	int seasonId = info1.first;
	int sectionId = info1.second;

	int nextSeasonId = layerFactory->getNextSeasonId(seasonId,sectionId);
	int nextSectionId = layerFactory->getNextSectionId(seasonId,sectionId);

	GameScore gamescoreinfo = Runtime::getRuntime()->getGameScore();
	int oldScore = 0, oldStar = 0;
	SectionInfor* pInfo = GameInforEditer::getGameInfor()->getSectionInfor(seasonId, sectionId);
	if (pInfo)
	{
		oldScore = pInfo->iRankingScore;
		oldStar = pInfo->iStarNum;
	}

	ccColor4B mr;
	mr.r=0;mr.g=0;mr.b=0;mr.a=160;
	CCLayerColor* color = CCLayerColor::create(mr);
	BaseScense::getCurrentScene()->addChild(color, 100, 100+100);

	int score = SectionInfor::calculateRankScore(gamescoreinfo.timeCost, gamescoreinfo.effectiveClickedNum, gamescoreinfo.clickStar, gamescoreinfo.isUsedSuperTool);
	Acount* player = Acount::createByid(score, gamescoreinfo.clickStar, oldScore, oldStar);
	player->m_playBeyondAnimate = true;//成绩刷新就要播放超越动画
	addChild(player, 127, 100);
	player->setPositionX(player->getContentSize().width);

	player->runAction(CCSequence::create(
		CCEaseExponentialOut::create(CCMoveBy::create(0.4f, ccp(-player->getContentSize().width, 0.0f))),
		//CCEaseOut::create(CCMoveBy::create(0.5f, ccp(-player->getContentSize().width, 0.0f)), 0.25f),
		NULL));

//     CongratulationLayer* layer = new CongratulationLayer(mSceneIndex, oldScore); 	
//     addChild(layer, 1024);
//     layer->release();
 	if (nextSeasonId != seasonId)//该进入下一章了,那么Next的按钮就到选关界面
 	{
// 		layer->setNextButtonToSelector();

 		if (needUnlockAction)
 		{
 			//设置需要解锁的Season的Id
 			LevelManager::getManager()->setNeedUnLockSeason(nextSeasonId);
 		}else
 		{
 			LevelManager::getManager()->setNeedScrollToSeason(nextSeasonId);
 		}
	}
 
	//保存关卡数据到内存,确定是否要解锁动画
	sectionFinished(seasonId,sectionId,nextSeasonId,nextSectionId);
	GameScore runtime = Runtime::getRuntime()->getGameScore();
	SectionInfor* info = GameInforEditer::getGameInfor()->getSectionInfor(seasonId, sectionId);
	if (info)
	{
		if (!info->isSuccessed)
		{
			info->fFirstCost = runtime.timeCost;
		}
		//保存更优的数据
		if (info->isBetterThanMe(runtime.timeCost, runtime.effectiveClickedNum, runtime.clickStar, runtime.isUsedSuperTool))
		{
			info->fTimeCost = runtime.timeCost;
			info->effectiveClickNum = runtime.effectiveClickedNum;
			info->iStarNum = runtime.clickStar;
			info->iRankingScore = SectionInfor::calculateRankScore(runtime.timeCost, runtime.effectiveClickedNum, runtime.clickStar, runtime.isUsedSuperTool);
			//保存数据
			GameInforEditer::getGameInfor()->saveSessonInforList();
			//有更优的数据的时候，需要刷新内存中的数据
			//因为结算界面的排行榜是从内存中的数据获取的，不然会导致每次得分更高的时候看到的都是第一次得分变化成最高的得分
			SocialInfoManager::getInstance()->setSelfSectionScore(seasonId, sectionId, info->iRankingScore, info->iStarNum);

#if RECORD_ASYNC == 1
			if (runtime.isUsedSuperTool) //使用了超能道具的没有解法数据，所以直接上传
			{
				CCLOG("upload section score");
				vector<std::pair<int,int> > sectionList;
				sectionList.push_back(std::make_pair(LevelManager::getManager()->getLastSeasonId(), LevelManager::getManager()->getLastSectionId()));
				CmdHelper::getInstance()->uploadTaskSolution(sectionList);
			}
			else//没有使用超能道具，等解法数据保存完后，一起上传
			{
				int sectionIdx = GameLayerFactory::getInstance()->getSectionIndex(LevelManager::getManager()->getLastSeasonId(), LevelManager::getManager()->getLastSectionId());
				Recorder::pfuncRecordCallback pRecordCallback = [](bool bSuccess, int sectionIdx, string error){
					CCLOG("async solution end upload solution data");
					vector<std::pair<int,int> > sectionList;
					sectionList.push_back(GameLayerFactory::getInstance()->getLevelFormat(sectionIdx));
					CmdHelper::getInstance()->uploadTaskSolution(sectionList);
				};
				CCLOG("async solution begin save record data");
				Recorder::getInstance()->setOneSectionRecordAsync(sectionIdx, BaseContentLayer::shareBaseContentLayer()->getAllFramesData(), pRecordCallback);
			}
#else
			if (Recorder::getInstance()->setOneSectionRecord(GameLayerFactory::getInstance()->getSectionIndex(
				LevelManager::getManager()->getLastSeasonId(), LevelManager::getManager()->getLastSectionId()), 
				BaseContentLayer::shareBaseContentLayer()->getAllFramesData())){
					CCLog("record OK");
			} else {
				CCLog("record Error");
			}
			vector<std::pair<int,int> > sectionList;
			sectionList.push_back(std::make_pair(LevelManager::getManager()->getLastSeasonId(), LevelManager::getManager()->getLastSectionId()));
			CmdHelper::getInstance()->uploadTaskSolution(sectionList);
#endif

		}
	}
 	//GameInforEditer::getGameInfor()->saveSessonInforList();

	AchievementManager::getInstance()->congratulationed();

	//analytics
	//记录玩家开始一关游戏 --- end (Success)
	const char* pEventInfo = CCString::createWithFormat("%02d-%02d", LevelManager::getManager()->getLastSeasonId(), LevelManager::getManager()->getLastSectionId())->getCString();
	CmdHelper::getInstance()->getAnalyticsInstance()->onCompleted(pEventInfo);
}

void BaseScense::sectionFinished(int seasonId,int sectionId,int nextSeasonId,int nextSectionId)
{
	GameScore gameScore = Runtime::getRuntime()->getGameScore();
    //保存本关数据
	{//保存本关数据
		SeasonInfor* seasonInfor = GameInforEditer::getGameInfor()->getSeasonInfor(seasonId);
		SectionInfor* sectionInfor = GameInforEditer::getGameInfor()->getSectionInfor(seasonId,sectionId);
		if (seasonInfor == NULL)
		{
			seasonInfor = new SeasonInfor(seasonId,true,false);//玩过，而且没有锁
			GameInforEditer::getGameInfor()->addSeasonInfor(seasonInfor);
		}else
		{
			seasonInfor->locked = false;
			seasonInfor->isPlayed = true;
		}

		if (sectionInfor == NULL)
		{
			sectionInfor = new SectionInfor(seasonId,sectionId,true,false);
			sectionInfor->iStarNum = max(gameScore.getStarNum(),sectionInfor->iStarNum);
			sectionInfor->isSuccessed = true; // 成功过了
			seasonInfor->addSectionInfor(sectionInfor);
		}else
		{
			sectionInfor->iStarNum = max(gameScore.getStarNum(),sectionInfor->iStarNum);
			sectionInfor->isPlayed = true;
			sectionInfor->locked = false;
			sectionInfor->isSuccessed = true;
		}

		//设置勋章,如果数量有增加就重新设置
		{
			GameScore gameScore = Runtime::getRuntime()->getGameScore();
			if (sectionInfor->keyItemMedalNum < gameScore.keyItemMedalNum)
				sectionInfor->keyItemMedalNum = gameScore.keyItemMedalNum;
			if (sectionInfor->keyTableMedalNum < gameScore.keyTableMedalNum)
				sectionInfor->keyTableMedalNum = gameScore.keyTableMedalNum;
		}

        //保存season的数据
        seasonInfor->resetScoreByMySections();
	}

	{//开启下一关：如果是游戏的最后一关，那么就直接交给wholeGameFinished，如果是本章的最后一关，就交给wholeSeasonFinished处理
		if ( nextSeasonId == -1 )
		{//下一关的章就无效了，那么就是最后一关;
			wholeGameFinished();//游戏完成通关
		}else if (nextSeasonId != seasonId)
		{//一章玩完了
			wholeSeasonFinished(seasonId);
		}
		else
		{//开启本章的下一关
			CCAssert(nextSeasonId == seasonId,"ERROR:BaseScense::sectionFinished---->open next section nextSeasonId should equel seasonId !");
			SectionInfor* sectionInfor = GameInforEditer::getGameInfor()->getSectionInfor(seasonId,nextSectionId);
			if (sectionInfor == NULL)
			{
				sectionInfor = new SectionInfor(seasonId,nextSectionId,false,false);
				GameInforEditer::getGameInfor()->getSeasonInfor(seasonId)->addSectionInfor(sectionInfor);
			}else
			{
				sectionInfor->locked = false;
			}
			// 开始计时
			GameInforEditer::getGameInfor()->startSectionTimeCounter(nextSeasonId,nextSectionId);
		}
	}
	
}

void BaseScense::wholeGameFinished()
{//成功
	int id = GameLayerFactory::getInstance()->getConfigSeasonNum();
	AchievementManager::getInstance()->seasonFinished(id);
}

void BaseScense::wholeSeasonFinished(int seasonId)
{
	// 解锁
	if (GameInforEditer::getGameInfor()->canUnclock(seasonId + 1))
	{
		GameInforEditer::getGameInfor()->unLockSecretStageBySeason(seasonId);
		GameInforEditer::getGameInfor()->unLockSeasonForce(seasonId + 1);
		needUnlockAction = true;

		// 开始计时,需要吗？？
		GameInforEditer::getGameInfor()->startSectionTimeCounter(seasonId + 1,1);
	}
	
	{//保存当前章节的数据

	}

	AchievementManager::getInstance()->seasonFinished(seasonId);
}

void BaseScense::failed(){
    //current layer sleep
    mContentLayer->freezeLayer();

	if (LFAlert_FreeTimeOut_InGameFaild::isTimeToShowBeforePlay(seasonId,sectionId))
	{
		LFAlert_FreeTimeOut_InGameFaild::showAlert(seasonId,sectionId)->
			addAutoRemoveObserver(this,SEL_CallFuncO(&BaseScense::buySectionSuc),BUY_ONE_PLAY_TIME_SUC);
//			addAutoRemoveObserver(this,SEL_CallFuncO(&BaseScense::buySectionWithMoney),MSG_PP_SectionTimeLimiteRsp);
		return;
	}

	if (mContentLayer->getArmature())
	{
		mContentLayer->getArmature()->getAnimation()->play(g_ThiefAnimation_siwang.c_str());
        Music::sharedMusicPlayer()->playEffectSound("lost.wav");
		return;
	}

	LevelManager::getManager()->retry(false);

	//4. 20140720 scf 新需求 --- 
// 	//1.红星坠落失败
// 	//2.重新开始  
// 	AdManager::getManager()->showFuilureAd();
//	AdManager::getManager()->showAdByTimeDelay();


	//analytics
	//记录玩家开始一关游戏 --- end (faild)
	const char* pInfo = CCString::createWithFormat("%02d-%02d", LevelManager::getManager()->getLastSeasonId(), LevelManager::getManager()->getLastSectionId())->getCString();
	CmdHelper::getInstance()->getAnalyticsInstance()->onFailed(pInfo, "star fall");
}

void BaseScense::buySectionSuc(CCObject* obj)
{
	LevelManager::getManager()->retry(false);
}
void BaseScense::buySectionWithMoney(CCObject* payRspWrapper)
{
// 	PPCmd_BuySectionTimeLimitRsp* rsp = (PPCmd_BuySectionTimeLimitRsp*)((PayRspWrapper*)payRspWrapper)->data;
// 	if (rsp->rspCode == PP_Success && rsp->seasonId == seasonId && rsp->sectionId == sectionId)
// 	{
// 		buySectionSuc(NULL);
// 	}
}







