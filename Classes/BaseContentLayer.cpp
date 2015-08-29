//
//  BaseContentLayer.cpp
//  demo-box2d
//
//  Created by Davidloeng loeng on 12-6-26.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//
//
#include <iostream>
#include "BaseContentLayer.h"
#include "BaseScense.h"
#include "ResManager.h"
#include <cmath>
#include "Runtime.h"
#include "SpriteHelper.h"
#include "AWTextureFilter.h"
#include "CustomeSprite.h"
#include "ShadowLayer.h"
#include "Bomb.h"
#include "VRope.h"
#include "TouchTrailLayer.h"
#include "Music.h"
#include "MenuLayer.h"
#include <typeinfo>
#include <vector>
#include "BackgroudLayerFactory.h"
#include "AchievementManager.h"
#include "GameLayerFactory.h"
#include "MyContactFilter.h"
#include "LFBox2dHelper.h"
#include "LevelManager.h"
#include "WaitTimeToClickLayer.h"
#include "BaseContentWrapLayer.h"
#include "SafeBoarder.h"
#include "LFAlerts.h"
#include "BaseSprite.h"
#include "VersionControl.h"
#include "cocos-ext.h"
#include "font.h"
#include "JuqingDaoJuManager.h"
#include "CollisionDetection.h"
#include "History.h"
#include "GuideLayer_JuqingDaoju.h"
#include "animatepacker/AnimatePacker.h"
#include "Recorder.h"
#include "UserDataManager.h"
#include "Guankamsg.h"
#include "AppMacros.h"
#include "Goluetu.h"
#include "GuideLayerFactory.h"
#include "GuideLayer_ClickFinger.h"

USING_NS_CC_EXT;
float MAX_MOVE_DISTANCE_FOR_CLICK = 30.0f; 

const static float s_fMoveSpeed = 500.0f;			//每秒移动的像素

extern const float g_PlaySpeedScale = 1.0f;

extern const int g_iTest_Record = 0;			//是否打开测试播放

const static int s_iMaxRecordTime = 18000;	//最大的录制时间帧 5分钟

#define Record		1
#define USE_SCALE9SPRITE_TRACK				1
#define SHOW_GUANKAMSG_LAYER				0
#define USE_JUMP_ARMATURE					1

#define GaussianBlurRadiusForSpriteDisappear 10
#define TABLE_HL_TAG 2324
#define TABLE_FL_TAG 2325
#define REDSTAR_MASK_TAG 2326//需要特效的红心表面的一层与红心重合的Sprite的Tag
#define REDSTAR_FL_TAG 2327
#define TIME_DEC_EFFECTIVE_TAG 287635

#define Tag_Forever_Rotate	27635

#define Tag_Particle 1111

#define LONG_TIME	10.0f
#define USE_MANUAL_FRAGMENT false
float PTM_RATIO = PTM_RATIO_INIT; // 默认值

static BaseContentLayer* curInstance = NULL;
static BaseContentLayer* preInstance = NULL;
std::vector<string> g_NeedBlurFileNameVec;

//创建一个粒子系统，并加入到item中，也返回这个粒子系统的指针
CCParticleSystem* createFogParticle(CCSprite* item,const char* fileName)
{
	CCParticleGalaxy* par = new CCParticleGalaxy();
	par->autorelease();
	int particalNum = 400;
// 	if (getMachinePerformanceLevel() <= 0)
// 	{
// 		particalNum = 200;
// 	}
	
	par->initWithTotalParticles(particalNum);
	par->setTexture( CCTextureCache::sharedTextureCache()->addImage(fileName));
	par->setAutoRemoveOnFinish(true);
	par->setEmitterMode(kCCParticleModeGravity);
	par->setPositionType(kCCPositionTypeGrouped);
	par->setGravity(CCPointZero);
	ccColor4F startColor;
	startColor.a = 0.009f;
	startColor.r = 211.0f/255.0f / 2.0f;//0.414
	startColor.g = 252.0f/255.0f / 2.0f;//0.494
	startColor.b = 253.0f/255.0f / 2.0f;//0.496

	ccColor4F endColor;
	endColor.a = 0.04f;
	endColor.r = 211.0f/255.0f / 2.0f;//0.414
	endColor.g = 252.0f/255.0f / 2.0f;//0.494
	endColor.b = 253.0f/255.0f / 2.0f;//0.496

	par->setStartColor(startColor);
	par->setEndColor(endColor);
	par->setStartSize(7.0f);
	par->setStartSizeVar(3.0f);
	par->setEndSize(20.0f);
	par->setEndSizeVar(5.0f);
	par->setTotalParticles(particalNum);
	par->setAngle(90.0f);
	par->setAngleVar(180.0f);
	par->setLife(2.0f);
	par->setLifeVar(0.8f);
	par->setSpeed(8.0f);
	par->setSpeedVar(1.8f);
	par->setTangentialAccel(0.0f);
	par->setTangentialAccelVar(0.0f);
	par->setRadialAccel(2.0f);
	par->setRadialAccelVar(0.0f);
	par->setDuration(-1);

	par->setPosition(ccp(item->getContentSize().width/2,item->getContentSize().height/2));
	par->setPosVar(CCPointZero);
	item->addChild(par,0,Tag_Particle);

	//防止崩溃
	if (CC_TARGET_PLATFORM != CC_PLATFORM_IOS)
		par->retain();

	return par;
}
//创建一个粒子系统，并加入到item中，也返回这个粒子系统的指针
CCParticleSystem* createFog24Particle(CCSprite* item,const char* fileName)
{
	CCParticleGalaxy* par = new CCParticleGalaxy();
	par->autorelease();
	int particalNum = 100;
// 	if (getMachinePerformanceLevel() <= 0)
// 	{
// 		particalNum /= 2;
// 	}
	par->initWithTotalParticles(particalNum);
	par->setTexture( CCTextureCache::sharedTextureCache()->addImage(fileName));
	par->setAutoRemoveOnFinish(true);
	par->setEmitterMode(kCCParticleModeGravity);
	par->setPositionType(kCCPositionTypeGrouped);
	par->setGravity(CCPointZero);
	ccColor4F startColor;
	startColor.a = 0.009f;
	startColor.r = 211.0f/255.0f / 2.0f;//0.414
	startColor.g = 252.0f/255.0f / 2.0f;//0.494
	startColor.b = 253.0f/255.0f / 2.0f;//0.496

	ccColor4F endColor;
	endColor.a = 0.04f;
	endColor.r = 211.0f/255.0f / 2.0f;//0.414
	endColor.g = 252.0f/255.0f / 2.0f;//0.494
	endColor.b = 253.0f/255.0f / 2.0f;//0.496

	par->setStartColor(startColor);
	par->setEndColor(endColor);
	par->setStartSize(7.0f);
	par->setStartSizeVar(3.0f);
	par->setEndSize(20.0f);
	par->setEndSizeVar(5.0f);
	par->setTotalParticles(particalNum);
	par->setAngle(90.0f);
	par->setAngleVar(180.0f);
	par->setLife(0.5f);
	par->setLifeVar(0.2f);
	par->setSpeed(2.0f);
	par->setSpeedVar(1.0f);
	par->setTangentialAccel(0.0f);
	par->setTangentialAccelVar(0.0f);
	par->setRadialAccel(0.0f);
	par->setRadialAccelVar(0.0f);
	par->setDuration(-1);

	par->setPosition(ccp(item->getContentSize().width/2,item->getContentSize().height/2));
	par->setPosVar(CCPointZero);
	item->addChild(par,0,Tag_Particle);

	//par->setScale(0.5f);
	//防止崩溃
	if (CC_TARGET_PLATFORM != CC_PLATFORM_IOS)
		par->retain();

	return par;
}

BaseContentLayer::BaseContentLayer(BaseScense* scense, playMode curPlayMode/* = playMode_Normal*/, std::vector< std::vector<record> >* allRecordData/* = NULL*/)
	: springBedAction(NULL)
	, m_pSuccessRedSar(NULL)
	, m_pSuccessPlatform(NULL)
{
	preInstance = curInstance;
	curInstance = this;
	m_bSuccessful = false;
	GameScenseDefine::SharedGameScenseDefine()->getAllBaseContentLayerVec()->push_back(this);
	if (allRecordData)
	{
		m_AllFramesData = *allRecordData;
	}
	m_iCurPlayMode = curPlayMode;
	m_bAnimationOver = false;
	m_llLastTouchBeginTime = 0;
	m_pArmature = NULL;

	m_curSeasonID = LevelManager::getManager()->getLastSeasonId();
	m_curSectionID = LevelManager::getManager()->getLastSectionId();

	if (m_iCurPlayMode == playMode_Normal)
	{
#if	Record != 0
		setRecord(true);
#else
		setRecord(false);
#endif
	}
	else
	{
		setRecord(false);
	}
	
	mIsLastClickedItem = false;
	setTouchEnabled( true );
	setAccelerometerEnabled( true );
	mScreenSize = CCDirector::sharedDirector()->getWinSize();
	boilerNum = 0;

	mSafeBoarder = NULL;
	mSuperTool = NULL;
	mUseSuperTool = false;
	mCreateSuperToolWhenScaleBack = false;
    mEnabledDoubleClick = false;

    
//		ResManager* resMgr = ResManager::getManager();
	// 图册
	{
//		CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("fragements.plist");
//		CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("table_shadow.plist");
		CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("table.plist");
		CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("physicalItem.plist");
//		CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("physicalItem_shadow.plist");
	}
    int bedFileNum = 5;
	string bedFiles[] = {
		"spring_1.png",
		"spring_2.png",
		"spring_3.png",
		"spring_2.png",
		"spring_1.png"};
    springBedAction = (CCFiniteTimeAction*)SpriteHelper::createAction_pList(bedFiles, bedFileNum, 0.02f,false);
    springBedAction->retain();
    
    mRedstarContactTable = false;
    mPositionRedstarFromTable = b2Vec2(0,0);
    mRedstarContactTableTime = 0;
	initRedStartPos = CCPointZero;
	lastCreateTablePos = CCPointZero;
	lastCreateBombPos = CCPointZero;
	timerPos = CCPointZero;
    
    redStarFadeTime = 0.7f;
    redStatDelayTime = 0.7f;
    
	contactedTable = NULL;
	mSpriteToDisappear = NULL;
	//guide = NULL;
    //mCanDestroyBoxNow = true;
    
    mFailedWhenRedStarFall = true;
    needRedStarBeginEff = true;
    mRedstarExist = false;
    timeAscing = false;
    
    setLayerFreezy(false);

    mActiveBoiler = NULL;
    mToucheMovePointPrev.x = -1;
    mToucheMovePointPrev.y = -1;

	//
	mScense = scense;
	// Define the gravity vector.
	b2Vec2 gravity;
	gravity.Set(0.0f, -10.0f);

	// Do we want to let bodies sleep?
	bool doSleep = true;

	// Construct a world object, which will hold and simulate the rigid bodies.
	mWorld = new b2World(gravity);
	mWorld->SetAllowSleeping(doSleep);    
	mWorld->SetContinuousPhysics(true);

	//
	mContactListener = new ContactListener(this);
	mWorld->SetContactListener(mContactListener);

	mContactFilter = new MyContactFilter();
	mWorld->SetContactFilter(mContactFilter);
	// 3个bacthNode
	{
		motorTrackBatchNode = NULL;
		track = NULL;
		mRopeSpriteBatchNode = NULL;

		ResManager* resMgr = ResManager::getManager();
		//rope texture  rope.png
		mRopeSpriteBatchNode = CCSpriteBatchNode::create(resMgr->getSharedFilePath(g_gameinPath+"rope.png").c_str());
		addChild(mRopeSpriteBatchNode);
#if USE_SCALE9SPRITE_TRACK == 0
 		string tableTrackFile = SpriteHelper::splicingNum_String("table_no_S%d_track_middle.png", 1);//mScense->getCurrentSeasonId());
 		track = CCSpriteBatchNode::create(resMgr->getSharedFilePath(g_gameinPath+tableTrackFile).c_str());
 		track->setPosition(CCPointZero);
 		//BackgroudLayerFactory::getCurrentBgLayer()->addChild(track);
 		mScense->getBackgroudCoverLayer()->addChild(track);
 
 		string motorTrackFile = SpriteHelper::splicingNum_String("turn_anchor_S%d_track_middle.png", 1);// mScense->getCurrentSeasonId());
 		motorTrackBatchNode = CCSpriteBatchNode::create(resMgr->getSharedFilePath(g_gameinPath+motorTrackFile).c_str());
 		motorTrackBatchNode->setPosition(CCPointZero);

		//BackgroudLayerFactory::getCurrentBgLayer()->addChild(motorTrackBatchNode);
		mScense->getBackgroudCoverLayer()->addChild(motorTrackBatchNode);
#endif
	}

	mLastBoxDestroyTime = 0;
	redStar_StarPar = NULL;
	redStar_FirePar = NULL;
	//离子系统,星星,安卓的不要这个粒子系统
	{
		if (CC_TARGET_PLATFORM != CC_PLATFORM_ANDROID)
		{
			//if (getMachinePerformanceLevel() > 0)
			{
				redStar_StarPar = createRedStarStarPar(ccp(25,25));
				addChild(redStar_StarPar, 10);
				redStar_StarPar->setVisible(false);
			}
		}
	}

	// 离子系统2，安卓的不要这个粒子系统
	{
		if (CC_TARGET_PLATFORM != CC_PLATFORM_ANDROID)
		{
//			if (getMachinePerformanceLevel() > 0)
			{
				redStar_FirePar = createRedStarFirePar(ccp(20,30));
				addChild(redStar_FirePar);		
				redStar_FirePar->setVisible(false);
			}
		}
	}

	{
		touchTrailLayer = TouchTrailLayer::create();
		mScense->addChild(touchTrailLayer,100);
	}

	ShadowLayer::sharedShadowLayer()->startToUpdateShadows(mWorld);

	BombCache::getCache();//创建缓存

	mRopes.clear();
	motorVec.clear();
	//strobeVec.clear();
	emVec.clear();
	//boilerVec.clear();
	toDestoryVec.clear();
	mTimerDecList.clear();

	//add by jiangjie
	m_iRecordTotalCount = 0;
	m_iCurRecordFrame = 0;
	m_iPlayFrame = 0;
	m_fTickTotalTime = 0.0f;
	m_iEffectLevel = 1;
	m_iPlayEffectTimes = 0;
	m_bPlayEffect = true;
	m_fDiamondPreviousPosition = CCPointZero;
	setPlayRecord(false);
	m_bShowRanodomArmature = false;

	mScense->setCollisionEnable(false);
	m_curLevelinfo = GameLayerFactory::getInstance()->getConfigSectionInfo(m_curSeasonID, m_curSectionID);
	if (m_iCurPlayMode == playMode_PlayGameRecord)
	{
		m_bAnimationOver = true;
		mScense->getMenuLayer()->setVisible(false);
		if (g_iTest_Record != 0)
		{	
			Runtime::getRuntime()->clearData();
		}
		setLayerFreezy(true);
		showGameImmediately();
		return;
	}
	else if (m_iCurPlayMode == playMode_Normal)
	{
		initRandomArmatureVec();

		Runtime::getRuntime()->clearData();

		AchievementManager::getInstance()->clearData(m_curSeasonID, m_curSectionID);
		AchievementManager::getInstance()->setWorld(mWorld);

		//先隐藏菜单层和游戏层，动画播放完了才显示,并且屏蔽物理相关
		this->setVisible(false);
		mScense->getMenuLayer()->setVisible(false);
		//获取齿轮隐藏
		mScense->getBackgroudCoverLayer()->setVisible(false);

		unschedule(schedule_selector(BaseContentLayer::longTimeNoOpCallback));
		scheduleOnce(schedule_selector(BaseContentLayer::longTimeNoOpCallback),LONG_TIME);
		CCNotificationCenter::sharedNotificationCenter()->addObserver(this,SEL_CallFuncO(&BaseContentLayer::useSuperToolClicked),MSG_USE_SUPERTOOL,NULL);
		CCNotificationCenter::sharedNotificationCenter()->addObserver(this,SEL_CallFuncO(&BaseContentLayer::fixScaleBackEnd),MSG_FIXED_SCALE_END,NULL);

		Music::sharedMusicPlayer()->stopAllEffect();
		Music::sharedMusicPlayer()->playPlayMusic();

		//添加台阶
		addTaiJie(ccp(mScreenSize.width/2, -g_iTaiJieHight/2));

		mScense->getBaseContentWrapLayer()->setPositionY(0);

		if (!LevelManager::getManager()->getShowLoadingAnimation() || g_iTest_Record != 0)
		{
			showGameImmediately();
		}
		else
		{
			mScense->getBackgroudLayer()->setPositionY(mScense->getBackgroudLayer()->getPositionY()-g_iTaiJieHight);

			m_pArmature = CCArmature::create("star_r");
			if (m_curLevelinfo->iRole_in == Role_Pos_Right)
			{
				m_pArmature->setPosition(ccp(mScreenSize.width+m_pArmature->getContentSize().width, g_iTaiJieHight));
				m_pArmature->getAnimation()->play(g_ThiefAnimation_paozuo.c_str());
			}
			else
			{
				m_pArmature->setPosition(ccp(-m_pArmature->getContentSize().width, g_iTaiJieHight));
				m_pArmature->getAnimation()->play(g_ThiefAnimation_paoyou.c_str());
			}
			m_pArmature->getAnimation()->setMovementEventCallFunc(this, movementEvent_selector(BaseContentLayer::MovementEventCallFunc));
			mScense->getBaseContentWrapLayer()->addChild(m_pArmature, 2000);
			m_pArmature->setVisible(false);

			this->layerUp();
		}
	}
	//end
}

void BaseContentLayer::onEnter()
{
	LFLayer::onEnter();
	schedule(schedule_selector(BaseContentLayer::tick) );
	schedule(schedule_selector(BaseContentLayer::timeToFlowlightAction),TIME_FLOWLIGHT_ACTION);
	schedule(schedule_selector(BaseContentLayer::preBomb),1);
}

void BaseContentLayer::initRandomArmatureVec()
{
	m_randomArmatureVec.clear();
	m_tempRandomArmatureVec.clear();
	m_bShowRanodomArmature = true;

	m_randomArmatureVec.push_back(g_ThiefAnimation_shenlanyao);

	m_tempRandomArmatureVec = m_randomArmatureVec;
}

void BaseContentLayer::SaveData()
{
	if (m_iCurPlayMode != playMode_Normal || mUseSuperTool  || m_iCurRecordFrame > s_iMaxRecordTime)
	{
		return;
	}
	
	if (Recorder::getInstance()->setOneSectionRecord(GameLayerFactory::getInstance()->getSectionIndex(
		m_curSeasonID, m_curSectionID), &m_AllFramesData))
	{
		CCLOG("record OK");
	}
	else
	{
		CCLOG("record Error");
	}
}

void BaseContentLayer::starPlay()
{
	//有抛物体时，播放的时候要早点执行抛物，不然会导致播放过程中途出现物体
	schedule(schedule_selector(BaseContentLayer::begineThrowPointSchedule), 0.2f);
	this->unschedule(schedule_selector(BaseContentLayer::playRecord));
	this->schedule(schedule_selector(BaseContentLayer::playRecord));
}

void BaseContentLayer::playRecord(float dt)
{
	if (m_AllFramesData.size() <= 0)
	{
		return;
	}
	if (m_iPlayFrame >= 3)
	{
		if (!m_bPlayRecord)
		{
			return;
		}
	}
	++m_iPlayFrame;
	if (m_iPlayFrame >= m_AllFramesData.size())
	{
		this->unschedule(schedule_selector(BaseContentLayer::playRecord));
		if (m_iCurPlayMode == playMode_PlayGameRecord)
		{
			CCNotificationCenter::sharedNotificationCenter()->postNotification(MSG_GAME_CLICKIS_Z, NULL);
			if (g_iTest_Record != 0)
			{
				LevelManager::getManager()->setShowLoadingAnimation(false);
				LevelManager::getManager()->nextSection();
			}
			else
			{
				CCNode* pParent = this->getParent()->getParent()->getParent()->getParent()->getParent();

				this->getParent()->getParent()->getParent()->getParent()->removeFromParent();
				m_iPlayFrame = 0;

				if (pParent->getTag() == gflayertag)
				{
					((Goluetu*)pParent->getParent())->addplaygf();
				}
				else
				{
					CCLayer* pPlayRecordLayer = GameScenseDefine::SharedGameScenseDefine()->playRecord(m_curSeasonID, m_curSectionID, &m_AllFramesData, false);
					pPlayRecordLayer->setPosition(ccp(CCDirector::sharedDirector()->getWinSize().width/2, CCDirector::sharedDirector()->getWinSize().height / 2));
					pParent->addChild(pPlayRecordLayer, 1, Tag_Layer_PlayRecord);
				}
			}
		}
		return;
	}

	//用于更新绳子
	for(size_t i=0;i<mRopes.size();i++) {
		mRopes[i]->update(dt);
	}

	//遍历所有需要记录的元素，并执行对应的事件
	for (size_t i = 0; i < m_AllFramesData.at(m_iPlayFrame - 1).size(); i++)
	{
		RecordSprite* pCurSprite = NULL;
		//继续当前帧的下一个元素匹配时，先移除已经删除记录的元素
		removeFromRecordSpriteVec(m_WillBeRemoveRecordSpriteVec);

		//当前帧 当前已经记录的元素m_AllNeedRecordSpriteVec中只有一个元素与之匹配
		//所以找到之后就跳出m_AllNeedRecordSpriteVec这个循环，继续当前帧的下一个元素的查找匹配
		for (size_t k = 0; k < m_AllNeedRecordSpriteVec.size(); k++)
		{
			pCurSprite = (RecordSprite*)(m_AllNeedRecordSpriteVec.at(k));
			if (pCurSprite->m_SaveData.id == m_AllFramesData.at(m_iPlayFrame-1).at(i).id)
			{
				//为了适配所有的分辨率，记录的时候需要减去一个 偏移（屏幕的宽度的一半减去设计分辨率的一半），这样才能保证这个位置是一个最原始的位置，和分辨率无关的位置
				//当播放的时候，加上这个偏移，因为屏幕分辨率不同，这样每次加上的偏移就不同，就保证了在不同分辨率下显示的位置是一致的
				pCurSprite->setRotation(m_AllFramesData.at(m_iPlayFrame-1).at(i).rotate);
				int iTag = pCurSprite->getTag();
				if (iTag == Tag_DarkBomb_bombExplosion)
				{
				}
				else
				if (iTag == Tag_In_DarkBomb)
				{
					pCurSprite->setPositionX(m_AllFramesData.at(m_iPlayFrame-1).at(i).posX );
					pCurSprite->setPositionY(m_AllFramesData.at(m_iPlayFrame-1).at(i).posY );
				}
				else
				{
					pCurSprite->setPositionX(m_AllFramesData.at(m_iPlayFrame-1).at(i).posX + mScreenSize.width/2 - designResolutionSize.width/2);
					pCurSprite->setPositionY(m_AllFramesData.at(m_iPlayFrame-1).at(i).posY + mScreenSize.height/2 - designResolutionSize.height/2);
				}


				//用于更新星星上面的粒子特效位置
				if (pCurSprite->getTag() == Tag_Box_RedStar)
				{
					//设置粒子系统特效位置
					if (redStar_StarPar)
					{
						redStar_StarPar->setPosition(pCurSprite->getPosition());
					}
					if (redStar_FirePar)
					{
						redStar_FirePar->setPosition(pCurSprite->getPosition());
					}
				}
				else if (pCurSprite->getTag() == Tag_Box_Strobe)
				{
					if (m_AllFramesData.at(m_iPlayFrame-1).at(i).eventId == eventID_Strobe_Off)
					{
						addPlayRecordFinger(pCurSprite);
						((Strobe*)pCurSprite)->playDoorAnimation(false);
					}
					else if (m_AllFramesData.at(m_iPlayFrame-1).at(i).eventId == eventID_Strobe_On)
					{
						addPlayRecordFinger(pCurSprite);
						((Strobe*)pCurSprite)->playDoorAnimation(true);
					}
				}

				//其他的东西根据事件来区分，相同事件的根据Tag来区分
				if (m_AllFramesData.at(m_iPlayFrame-1).at(i).eventId == eventID_BoilerStart)
				{
					if (pCurSprite->getTag() == Tag_Box_Boiler)
					{
						CCSprite* pFinger = (CCSprite*)this->getChildByTag(2424);
						if (!pFinger)
						{
							pFinger = CCSprite::create(ResManager::getManager()->getFilePath(g_gameinPath+"finger.png").c_str());
							this->addChild(pFinger);
							pFinger->setPosition(ccpAdd(pCurSprite->getPosition(), ccp(-10, 0)));
							pFinger->setAnchorPoint(ccp(0.0f, 1.0f));
							pFinger->setScale(0.5f);
							pFinger->setTag(2424);
						}
						((Boiler*)pCurSprite)->fire();
					}
				}
				else if (m_AllFramesData.at(m_iPlayFrame-1).at(i).eventId == eventID_BoilerEnd)
				{
					if (pCurSprite->getTag() == Tag_Box_Boiler)
					{
						CCSprite* pFinger = (CCSprite*)this->getChildByTag(2424);
						if (pFinger)
						{
							pFinger->runAction(CCSequence::create(CCDelayTime::create(0.2f), CCFadeTo::create(0.2f, 0), CCRemoveSelf::create(), NULL));
						}
						((Boiler*)pCurSprite)->stop();
					}
				}
				else if (m_AllFramesData.at(m_iPlayFrame-1).at(i).eventId == eventID_CutRopeAction)
				{
					if (pCurSprite->getTag() == Tag_Box_RopeBatchSprite)
					{
						int xoffset = 40;
						CCSprite* pFinger = CCSprite::create(ResManager::getManager()->getFilePath(g_gameinPath+"finger.png").c_str());
						addChild(pFinger);
						pFinger->setPosition(ccp(pCurSprite->getPositionX() - xoffset, pCurSprite->getPositionY()));
						pFinger->setAnchorPoint(ccp(0.0f, 1.0f));
						pFinger->setScale(0.5f);
						pFinger->runAction(CCSequence::create(
							CCMoveBy::create(0.3f, ccp(xoffset*2, 0)),
							CCDelayTime::create(0.2f), 
							CCFadeTo::create(0.2f, 0),
							CCRemoveSelf::create(), NULL));
					}
				}
				else if (m_AllFramesData.at(m_iPlayFrame-1).at(i).eventId >= eventID_CutRopeStart)
				{
					//切断绳子的时候需要检查一遍绳子的锚点，让它执行渐隐动画
					VRope* newRope = NULL;
					((VRope*)pCurSprite)->createNewRope(&newRope, m_AllFramesData.at(m_iPlayFrame-1).at(i).eventId-eventID_CutRopeStart);
					vector<VRope*> newropes;
					newropes.push_back(newRope);
					addToRopes(newropes);
					((VRope*)pCurSprite)->setBodyA(NULL);

					Music::sharedMusicPlayer()->playEffectSound("cut_rope_sound.wav");

					removeFromRecordSpriteVec(m_WillBeRemoveRecordSpriteVec);
					bool bFind = false;
					for (size_t j = 0; j < m_AllFramesData.at(m_iPlayFrame - 1).size(); j++)
					{
						if (bFind)
						{
							break;
						}
						for (size_t m = 0; m < m_AllNeedRecordSpriteVec.size(); m++)
						{
							RecordSprite* temp = m_AllNeedRecordSpriteVec.at(m);
							if (temp->getTag() == Tag_Box_Rope_Anchor)
							{
								if (temp->m_SaveData.id == m_AllFramesData.at(m_iPlayFrame - 1).at(j).id)
								{
									if (m_AllFramesData.at(m_iPlayFrame - 1).at(j).eventId == eventID_Remove)
									{
										//单独的动画统一先删除原有的，然后新添加sprite来执行动画
										CCSprite* pNode = CCSprite::createWithTexture(temp->getTexture(), temp->getTextureRect());
										pNode->setPosition(temp->getPosition());
										temp->getParent()->addChild(pNode, temp->getZOrder());
										pNode->runAction(CCSequence::create(CCFadeTo::create(2.0f, 0), CCRemoveSelf::create(), NULL));

										temp->removeFromParent();
										bFind = true;
										break;
									}
								}
							}
						}
					}
				}
				else if (m_AllFramesData.at(m_iPlayFrame-1).at(i).eventId == eventID_TouchBomb_DarkWhole)
				{
					if (pCurSprite->getTag() == Tag_Box_Bomb_DarkWhole)
					{
						addPlayRecordFinger(pCurSprite);

						removeRopeWhenPlayRecord(m_iPlayFrame);

						for (size_t j = 0; j < m_AllFramesData.at(m_iPlayFrame - 1).size(); j++)
						{
							for (size_t m = 0; m < m_AllNeedRecordSpriteVec.size(); m++)
							{
								RecordSprite* temp = m_AllNeedRecordSpriteVec.at(m);
								if (temp->m_SaveData.id == m_AllFramesData.at(m_iPlayFrame - 1).at(j).id)
								{
									if (m_AllFramesData.at(m_iPlayFrame - 1).at(j).eventId == eventID_AddOutRangeItem)
									{
										((DarkHoleBomb*)pCurSprite)->addOutRangeItem(temp);
									}
									else if(m_AllFramesData.at(m_iPlayFrame - 1).at(j).eventId == eventID_setToBomb)
									{
										((DarkHoleBomb*)pCurSprite)->setToBomb(temp);
									}
								}
							}
						}				
						((DarkHoleBomb*)pCurSprite)->playBombAnimation();
					}
				}
				else if (m_AllFramesData.at(m_iPlayFrame-1).at(i).eventId == eventID_Touch)
				{
					if (pCurSprite->getTag() == Tag_Box_Bomb ||//一般的炸弹
						pCurSprite->getTag() == Tag_Box_Bomb_Hard ||//不可摔碎的炸弹
						pCurSprite->getTag() == Tag_Box_Bomb_Cirecle)//圆形炸弹
					{
						removeRopeWhenPlayRecord(m_iPlayFrame);
						int size = 4;
						//爆炸效果！
						if (pCurSprite->getContentSize().width < 48)
						{
							size = 1;
							pCurSprite->setScale(0.5f);
						}

						CCArmature* pArmature = CCArmature::create("boom");
						pArmature->setPosition(pCurSprite->getPosition());
						pCurSprite->getParent()->addChild(pArmature);
						pArmature->getAnimation()->setMovementEventCallFunc(this, movementEvent_selector(BaseContentLayer::MovementEventCallFunc));
						pArmature->getAnimation()->play("boom");
						pArmature->setScale(pCurSprite->getScale());

						playBombEffectBySize(size);

						addPlayRecordFinger(pCurSprite);
						//
						((CustomeSprite*)pCurSprite)->removeShadowIfPosible();
						pCurSprite->removeFromParent();
					}
					else if (pCurSprite->getTag() == Tag_Box_can_Destroied)
					{
						addPlayRecordFinger(pCurSprite);

						removeRopeWhenPlayRecord(m_iPlayFrame);
						((CustomeSprite*)pCurSprite)->fragmentBomb();
						((CustomeSprite*)pCurSprite)->removeShadowIfPosible();
						pCurSprite->removeFromParent();
					}		
					else if (pCurSprite->getTag() == Tag_Timer_Dec_Bomb)
					{
						Music::sharedMusicPlayer()->playEffectSound("timer_bomb_sound.wav");

						TimerDec* timerDec = (TimerDec*)pCurSprite->getChildByTag(Tag_Box_Frost);
						timerDec->retain();
						timerDec->removeFromParentAndCleanup(false);
						addChild(timerDec, 2, timerDec->getTag());
						timerDec->release();

						timerDec->setPosition(pCurSprite->getPosition());
						timerDec->setVisible(true);
						timerDec->action();

						timerDec->setScale(0.1f);
						timerDec->runAction(CCScaleTo::create(0.2f, 1.0f));
						timerDec->runAction(CCFadeIn::create(0.2f));

						mTimerDecList.insert(mTimerDecList.end(), timerDec);

						addPlayRecordFinger(pCurSprite);

						removeRopeWhenPlayRecord(m_iPlayFrame);

						pCurSprite->removeFromParent();
 						removeFromRecordSpriteVec(m_WillBeRemoveRecordSpriteVec);
					}
					else if (pCurSprite->getTag() == Tag_Timer_Asc_Bomb)
					{//如果是时间加速的炸弹
						timeAscing = true;

						CCSprite* effect = new CCSprite();
						effect->init();
						addChild(effect);
						effect->release();

						CCDelayTime* delay = CCDelayTime::create(Time_Dec_Gap);

						CCAction* action = SpriteHelper::createAction(delay,
							SEL_CallFuncO(&BaseContentLayer::timerAscActionEnd),
							effect);
						effect->runAction(action);

						removeRopeWhenPlayRecord(m_iPlayFrame);

						pCurSprite->removeFromParent();
						removeFromRecordSpriteVec(m_WillBeRemoveRecordSpriteVec);
					}
				}
				else if (m_AllFramesData.at(m_iPlayFrame-1).at(i).eventId == eventID_Remove)
				{
					if (pCurSprite->getTag() == Tag_Box_Rope)
					{
						//绳子被删除的时候，会删掉很多绳子元素，所以这里重新执行一遍当前帧的东西
						//不能break，因为可能后面的元素还有东西没有处理，就只能重新执行一遍当前帧的事件
						//执行过的事件是不会再执行的，因为执行过的元素已经从数组里面删除了
						((VRope*)pCurSprite)->setBodyA(NULL);
						((VRope*)pCurSprite)->setOpacity(1);
						((VRope*)pCurSprite)->update(dt);
					}
					else if (pCurSprite->getTag() == Tag_Box_RopeBatchSprite)
					{
						pCurSprite->setVisible(false);
					}
					else if (pCurSprite->getTag() == Tag_Box_Rope_Anchor)
					{
						pCurSprite->setVisible(false);
					}
					else if (pCurSprite->getTag() == Tag_Box_RedStar)
					{
						mRedstarExist = false;
						pCurSprite->removeFromParent();
					}
					else if (pCurSprite->getTag() == Tag_Box_Frost)
					{
						timerDecActionEnd(pCurSprite);
					}
					else
					{
						addToWillBeRemoveRecordSpriteVec(pCurSprite);
						pCurSprite->removeFromParent();
					}
					//移除后，需要从m_AllNeedRecordSpriteVec里面移除
					//不然会导致像冰冻炸弹，黑洞等这类由几个记录精灵组成的元素在移除后，循环访问时，野指针崩溃
					removeFromRecordSpriteVec(m_WillBeRemoveRecordSpriteVec);
				}
				else if (m_AllFramesData.at(m_iPlayFrame-1).at(i).eventId == eventID_Motor_Off)
				{
					addPlayRecordFinger(pCurSprite);
					((MotorSprite*)pCurSprite)->stopRotation();
					Music::sharedMusicPlayer()->playEffectSound("motor_sound.wav");
				}
				else if (m_AllFramesData.at(m_iPlayFrame-1).at(i).eventId == eventID_Motor_On)
				{
					addPlayRecordFinger(pCurSprite);
					((MotorSprite*)pCurSprite)->remuseRotation();
					Music::sharedMusicPlayer()->playEffectSound("motor_sound.wav");
				}
				else if (m_AllFramesData.at(m_iPlayFrame - 1).at(i).eventId == eventID_SpringBedAction)
				{
					pCurSprite->stopAllActions();

					Music::sharedMusicPlayer()->playEffectSound("spring_bed_collision.wav");

					CCFiniteTimeAction* frameAction = (CCFiniteTimeAction*)springBedAction->copy()->autorelease();

					CCAction* action = SpriteHelper::createAction(frameAction,
						SEL_CallFuncO(&BaseContentLayer::springBedActionEnd),
						pCurSprite);
					pCurSprite->runAction(action);
				}
				break;
			}
		}
	}
}

void BaseContentLayer::addPlayRecordFinger(CCNode* pNode)
{
	CCSprite* pFinger = CCSprite::create(ResManager::getManager()->getFilePath(g_gameinPath+"finger.png").c_str());
	pNode->getParent()->addChild(pFinger);
	pFinger->setPosition(ccpAdd(pNode->getPosition(), ccp(-12, 5)));
	pFinger->setAnchorPoint(ccp(0.0f, 1.0f));
	pFinger->setScale(0.5f);
	pFinger->runAction(CCSequence::create(CCDelayTime::create(0.2f), CCFadeTo::create(0.2f, 0), CCRemoveSelf::create(), NULL));
}

void BaseContentLayer::removeRopeWhenPlayRecord(int frameIndex)
{
	//移除当前帧所有事件为 eventID_Remove 的绳子和锚点
	removeFromRecordSpriteVec(m_WillBeRemoveRecordSpriteVec);
	for (size_t i = 0; i < m_AllFramesData.at(frameIndex - 1).size(); i++)
	{
		for (size_t j = 0; j < m_AllNeedRecordSpriteVec.size(); j++)
		{
			RecordSprite* temp = m_AllNeedRecordSpriteVec.at(j);
			if (temp->m_SaveData.id == m_AllFramesData.at(frameIndex - 1).at(i).id)
			{
				if (m_AllFramesData.at(frameIndex - 1).at(i).eventId == eventID_Remove)
				{
					if (temp->getTag() == Tag_Box_Rope)
					{
						((VRope*)temp)->setBodyA(NULL);
						((VRope*)temp)->setOpacity(1);
						((VRope*)temp)->update(CCDirector::sharedDirector()->getAnimationInterval());
						removeFromRecordSpriteVec(m_WillBeRemoveRecordSpriteVec);
						j = -1;
						continue;
					}
					else if (temp->getTag() == Tag_Box_Rope_Anchor)
					{
						temp->removeFromParent();
	 					removeFromRecordSpriteVec(m_WillBeRemoveRecordSpriteVec);
						j = -1;
						continue;
					}
				}
			}
		}
	}				
}

void BaseContentLayer::removeFromRecordSpriteVec(vector<RecordSprite*>& willRemoveFromVec)
{
	for (size_t i = 0; i < willRemoveFromVec.size(); i++)
	{
		for (size_t j = 0; j < m_AllNeedRecordSpriteVec.size(); j++)
		{
			if (willRemoveFromVec.at(i) == m_AllNeedRecordSpriteVec.at(j))
			{
				m_AllNeedRecordSpriteVec.erase(m_AllNeedRecordSpriteVec.begin()+j);
				break;
			}
		}
	}
	willRemoveFromVec.clear();
}

void BaseContentLayer::layerUpOver()
{
	m_pArmature->setVisible(true);

	CCPoint toPoint = ccp(mScreenSize.width/2, m_pArmature->getPositionY());

	if (m_curLevelinfo->iRole_in == Role_Pos_Right)
	{
		toPoint.x = mScreenSize.width - m_pArmature->getContentSize().width;
	}
	else
	{
		toPoint.x = m_pArmature->getContentSize().width;
	}

	float time = fabs(toPoint.x - m_pArmature->getPositionX())/s_fMoveSpeed;

	m_pArmature->runAction(CCSequence::create(
		CCMoveTo::create(time, toPoint),
		CCCallFuncN::create(this, callfuncN_selector(BaseContentLayer::playZuoXia)),
		NULL));
}

void BaseContentLayer::layerUp()
{
	float time = 0.8f;
	//背景层上移
	mScense->getBackgroudLayer()->runAction(CCSequence::create(
		CCMoveTo::create(time, ccp(mScense->getBackgroudLayer()->getPositionX(), mScense->getBackgroudLayer()->getPositionY()+g_iTaiJieHight)), 
		CCCallFunc::create(this, callfunc_selector(BaseContentLayer::layerUpOver)),
		NULL));

	//台阶上移
	if (CCNode* pNode = mScense->getBaseContentWrapLayer()->getWrapper()->getChildByTag(Tag_TaiJieStart))
	{
		CCPoint point = mScense->getBaseContentWrapLayer()->getWrapper()->convertToNodeSpace(ccp(pNode->getPositionX(), g_iTaiJieHight/2));
		point = ccp(0, point.y - pNode->getPositionY());
		pNode->runAction(CCSequence::create(
			CCMoveBy::create(time, point), 
			NULL));
	}
	
	//底部星星评价栏上移
	mScense->getBaseContentWrapLayer()->getChildByTag(Tag_StarStateBar)->runAction(CCSequence::create(
		CCMoveBy::create(time, ccp(0, g_iTaiJieHight)),
		NULL));
}

void BaseContentLayer::allItemFadeTo(CCNode* pNode, float time, int fade)
{
	if (!pNode)
	{
		return;
	}
	//得到所有的元素（木块，星星）
	CCArray* children = pNode->getChildren();
	if (!children)
	{
		return;
	}
	int num = children->count();
	for (int i=0;i<num;i++)
	{  
		CCSprite* temp = dynamic_cast<CCSprite*>(children->objectAtIndex(i));
		if (!temp)
		{
			continue;
		}
		if (time <= 0.001f)
		{
			temp->setOpacity(fade);
			continue;
		}
		temp->runAction(
			CCSequence::create(
				CCFadeTo::create(time, fade),
				NULL)
			);
		allItemFadeTo(temp, time, fade);
	}
}

void BaseContentLayer::addTaiJie(CCPoint point /*=CCPointZero*/)
{
	CCSprite* pTaiJie = NULL;
	if (pTaiJie = (CCSprite*)mScense->getBaseContentWrapLayer()->getWrapper()->getChildByTag(Tag_TaiJieStart))
	{
		pTaiJie->removeFromParent();
	}
	
	string taiJie = CCString::createWithFormat("background_s%d_1.png", m_curSeasonID)->getCString();

	pTaiJie = CCSprite::create(taiJie.c_str());
	if (point.equals(CCPointZero))
	{
		pTaiJie->setPosition(ccp(mScreenSize.width/2, pTaiJie->getContentSize().height/2));
	}
	else
	{
		pTaiJie->setPosition(point);
	}
	pTaiJie->setScale(1/mScense->getBaseContentWrapLayer()->getCurrentInitCenterScale());
	pTaiJie->setPosition(mScense->getBaseContentWrapLayer()->getWrapper()->convertToNodeSpace(pTaiJie->getPosition()));
	mScense->getBaseContentWrapLayer()->getWrapper()->addChild(pTaiJie, 1000, Tag_TaiJieStart);

	if (m_iCurPlayMode == playMode_Normal)
		addGuanKaName(pTaiJie);
}

void BaseContentLayer::setInfoVisible(bool e)
{
	if (CCSprite* pTaiJie = (CCSprite*)mScense->getBaseContentWrapLayer()->getWrapper()->getChildByTag(Tag_TaiJieStart))
	{
		pTaiJie->setVisible(e);
	}

	if (mScense->getBaseContentWrapLayer()->getChildByTag(Tag_StarStateBar))
	{
		mScense->getBaseContentWrapLayer()->getChildByTag(Tag_StarStateBar)->setVisible(e);
	}

	if (mSuperTool)
	{
		mSuperTool->clear();
	}

	if (mSafeBoarder)
	{
		mSafeBoarder->getCoverLayer()->setVisible(e);
	}
}

void BaseContentLayer::addGuanKaName(CCNode* pParent)
{
	CCLabelTTF* pGunka = (CCLabelTTF*)pParent->getChildByTag(Tag_GuankaLabel);
	if (pGunka)
	{
		pGunka->removeFromParent();
	}
	pGunka = CCLabelTTF::create("0", fontStr_KlavikaBold, 32);
	pParent->addChild(pGunka, 1003, Tag_GuankaLabel);
	pGunka->setString(CCString::createWithFormat(LFStrings::getValue("seasonstring").c_str(), m_curSeasonID, m_curSectionID)->getCString());

	if (m_curSectionID >= 10)
	{
		pGunka->setPosition(ccp(pParent->getContentSize().width/2 - pGunka->getContentSize().width/2 - 35, pParent->getContentSize().height/2 - 10));
	}
	else
	{
		pGunka->setPosition(ccp(pParent->getContentSize().width/2 - pGunka->getContentSize().width/2 - 30, pParent->getContentSize().height/2 - 10));
	}
}

void BaseContentLayer::showGameImmediately()
{
	m_bAnimationOver = true;
	//重新初始化台阶
	addTaiJie();
	if (m_iCurPlayMode != playMode_Normal)
	{
		if (g_iTest_Record != 0)
		{
			reallyStart(NULL);
		}
		return;
	}

	//重置底部评价栏Y坐标
	if (StarState* pBar = (StarState*)mScense->getBaseContentWrapLayer()->getChildByTag(Tag_StarStateBar))
	{
		pBar->stopAllActions();
		pBar->setPositionY(0);
	}
	else
	{
		//添加底部星星评价
		pBar = StarState::create(m_curLevelinfo->iPefect_Gold, m_curLevelinfo->iBest_Gold);
		pBar->setPosition(ccp(0, 0));
		mScense->getBaseContentWrapLayer()->addChild(pBar, 10001, Tag_StarStateBar);
	}

	//重置背景Y坐标
	mScense->getBackgroudLayer()->stopAllActions();
	mScense->getBackgroudLayer()->setPositionY(0);
	
	//改变角色的动画为坐下
	if (m_pArmature)
	{
		m_pArmature->getAnimation()->play(g_ThiefAnimation_zuo.c_str());
		m_pArmature->setPosition(ccp(m_pArmature->getContentSize().width, g_iTaiJieHight));
	}
	else
	{
		m_pArmature = CCArmature::create("star_r");
		m_pArmature->getAnimation()->play(g_ThiefAnimation_zuo.c_str());
		m_pArmature->getAnimation()->setMovementEventCallFunc(this, movementEvent_selector(BaseContentLayer::MovementEventCallFunc));
		mScense->getBaseContentWrapLayer()->addChild(m_pArmature, 2000);
		if (m_curLevelinfo->iRole_in == Role_Pos_Right)
		{
			m_pArmature->setPosition(ccp(mScreenSize.width - m_pArmature->getContentSize().width, g_iTaiJieHight));
		}
		else
		{
			m_pArmature->setPosition(ccp(m_pArmature->getContentSize().width, g_iTaiJieHight));
		}
	}

	if (g_iTest_Record == 0)
	{
		//点击了下一关，那么就显示关卡信息
		if (LevelManager::getManager()->getNext())
		{
			if (m_iCurPlayMode == playMode_Normal)
			{
                //1-3单独处理，开始不开启物理，点start的时候才开始，就不会导致倾斜的问题
                //4-5也需要单独处理，不然会导致小球点击了start后就跳动的bug
                //4-19，防止圆形炸弹自动滚落到屏幕外
				if ((m_curSeasonID == 4 && m_curSectionID == 5)
					|| (m_curSeasonID == 4 && m_curSectionID == 19)
#if Version_Free != 0
					|| (m_curSeasonID == 1 && m_curSectionID == 10)
#else
					|| (m_curSeasonID == 1 && m_curSectionID == 3)
#endif
					)
				{
					setLayerFreezyTrue();
				}
				showBaseContentLayerInAnimation();
			}
		}
		else
		{
			//不是结算界面点击的下一关，那么就直接开始
			reallyStart(NULL);
		}
	}
}

void BaseContentLayer::setLayerFreezyTrue()
{
    setLayerFreezy(true);
}

Guankamsg* BaseContentLayer::showGuanKaMsgLayer()
{
	//显示了关卡信息的时候，就调用暂停函数，停止所有的元素动画与动作，直到点了开始按钮

    setLayerFreezy(true);
    mScense->getMenuLayer()->pause();

	ccColor4B mr;
	mr.r=0;mr.g=0;mr.b=0;mr.a=160;
	CCLayerColor* color = CCLayerColor::create(mr);
	BaseScense::getCurrentScene()->addChild(color, 100, Guangkalayer_tag+100);

	Guankamsg* p = Guankamsg::createByid(m_curSeasonID, m_curSectionID);
	BaseScense::getCurrentScene()->addChild(p, 1000, Guangkalayer_tag);

	return p;
}

void BaseContentLayer::reallyStart(CCNode* pNode)
{
	m_bAnimationOver = true;

	if (redStar_FirePar)
	{
		redStar_FirePar->setVisible(true);
	}
	if (redStar_StarPar)
	{
		redStar_StarPar->setVisible(true);
	}

	if (m_iCurPlayMode == playMode_Normal)
	{
		mScense->getBackgroudLayer()->setPositionY(0);
		{
			mScense->getBackgroudCoverLayer()->setVisible(true);
			mScense->getBackgroudLayer()->setPositionY(0);
			mScense->getMenuLayer()->setVisible(true);
			this->setVisible(true);
			//测试播放的时候，就不调用这些函数
			if (g_iTest_Record == 0)
			{
				setLayerFreezy(false);
				mScense->setCollisionEnable(true);
				mScense->getMenuLayer()->resume();
				mScense->getMenuLayer()->showGame();
			}
			schedule(schedule_selector(BaseContentLayer::begineThrowPointSchedule), 2);
			if (BaseGuideLayer* pGuideLayer = (BaseGuideLayer*)mScense->getGuideLayer())
			{
				pGuideLayer->setTouchEnabled(true);
				pGuideLayer->setVisible(true);
				pGuideLayer->startGuide();
			}

			Runtime::getRuntime()->clearData();
		}
        
        //analytics
        //记录玩家开始一关游戏 --- begin
        const char* pInfo = CCString::createWithFormat("%02d-%02d", LevelManager::getManager()->getLastSeasonId(), LevelManager::getManager()->getLastSectionId())->getCString();
        CmdHelper::getInstance()->getAnalyticsInstance()->onBegin(pInfo);
	}
}

void BaseContentLayer::showBaseContentLayerInAnimation()
{
    mScense->getBackgroudCoverLayer()->setVisible(true);
	mScense->getBackgroudLayer()->setPositionY(0);
	this->setVisible(true);
    
	this->setPositionX(mScreenSize.width);
    
    mScense->getBackgroudCoverLayer()->setPositionX(this->getPositionX());
    
    float time = 0.3f;
    if (LevelManager::getManager()->getNext())
    {
        time = 0.3f;
    }
    scheduleOnce(schedule_selector(BaseContentLayer::startAnimation), time);
}

void BaseContentLayer::startAnimation(float dt)
{
	CCPoint point = ccp(-mScreenSize.width, 0.0f);
	float time = 0.4f;

	allItemFadeTo(this, 0.0f, 0);
	allItemFadeTo(this, time, 255);

#if SHOW_GUANKAMSG_LAYER != 0
	Guankamsg* p = showGuanKaMsgLayer();
	p->setPositionX(mScreenSize.width);

	p->runAction(CCSequence::create(
		CCEaseExponentialOut::create(CCMoveBy::create(time, point)),
		NULL));
#endif

    mScense->getBackgroudCoverLayer()->runAction(CCSequence::create(
                                                                    CCEaseExponentialOut::create(CCMoveBy::create(time, point)),
                                                                    NULL));

    this->runAction(CCSequence::create(
                                       CCEaseExponentialOut::create(CCMoveBy::create(time, point)),
                                       CCCallFunc::create(this, callfunc_selector(BaseContentLayer::BaseContentLayerInAnimationOver)),
                                       NULL));

}

void BaseContentLayer::BaseContentLayerInAnimationOver()
{
    LevelManager::getManager()->setNext(false);

#if SHOW_GUANKAMSG_LAYER == 0
	reallyStart(NULL);
#endif
}

//
CCParticleSystem* BaseContentLayer::createRedStarFirePar(CCPoint posVar)
{
// 	if (getMachinePerformanceLevel() <= 0)
// 	{
// 		return NULL;
// 	}
	int num = abs( (posVar.x * posVar.y ) / (20*30) * 60 );
	ResManager* resMgr = ResManager::getManager();
	// 火焰
	CCParticleSystem* par = new CCParticleSun();
	par->initWithTotalParticles(num);
	par->setPosition(ccp(-300,900));
	par->setTexture( CCTextureCache::sharedTextureCache()->addImage(resMgr->getSharedFilePath(g_gameinPath+"fire.png").c_str()) );
	par->setEmitterMode(kCCParticleModeGravity);
	par->setPositionType(kCCPositionTypeFree);
	par->setGravity(ccp(0,5));
	par->setAngleVar(0.0f);
	par->setStartColor(ccc4f(0.5f,0.5f,0.5f,0.0f));
	par->setStartColorVar(ccc4f(0.0f,0.0f,0.0f,0.0f));
	par->setEndColor(ccc4f(0.5f,0.5f,0.5f,0.04f));
	par->setEndColorVar(ccc4f(0.0f,0.0f,0.0f,0.0f));
	par->setStartSize(10.0f);
	par->setStartSizeVar(5.0f);
	par->setEndSize(60.0f);
	par->setEndSizeVar(20.0f);
	par->setSpeed(1.0f);
	par->setPosVar(posVar);
	par->setLife(0.8f);
	par->setLifeVar(0.3f);

	par->autorelease();

	return par;
}
//
CCParticleSystem* BaseContentLayer::createRedStarStarPar(CCPoint posVar)
{
	// 	if (getMachinePerformanceLevel() <= 0)
	// 	{
	// 		return NULL;
	// 	}
	int num = abs( (posVar.x * posVar.y ) / (25*25) * 10 );

	ResManager* resMgr = ResManager::getManager();
	CCParticleSystem* par = new CCParticleFlower();
	par->initWithTotalParticles(num);
	par->setPosition(ccp(-300,900));
	par->setPositionType( kCCPositionTypeFree );
	par->setLife(1);
	par->setLifeVar(0.5f);
	par->setStartSize(10);
	par->setStartSizeVar(5);
	par->setEndSize(8);
	par->setEndSizeVar(15);

	par->setStartColor(ccc4f(1.0f,1.0f,1.0f,0.5f));
	par->setEndColor(ccc4f(1.0f,1.0f,1.0f,1.0f));
	par->setStartColorVar(ccc4f(0.0f,0.0f,0.0f,0.0f));
	par->setEndColorVar(ccc4f(0.0f,0.0f,0.0f,0.0f));

	par->setPosVar(posVar);
	par->setSpeed(6.0f);
	par->setSpeedVar(3.0f);
	par->setBlendAdditive(false);
	par->setRadialAccel(0.0f);
	par->setRadialAccelVar(0.0f);
	par->setTangentialAccel(0.0f);
	par->setTangentialAccelVar(0.0f);
	par->setTexture( CCTextureCache::sharedTextureCache()->addImage(resMgr->getSharedFilePath(g_gameinPath+"stars.png").c_str()) );

	par->autorelease();
	return par;
}
void BaseContentLayer::preBomb(float time)
{
	unschedule(schedule_selector(BaseContentLayer::preBomb));
	return;
	Bomb* bomb = BombCache::getCache()->getBomb();
	addChild(bomb);
	bomb->setPosition(ccp(-100,-400));
//	bomb->bomb();
	//bomb->setPosition(ccp(100,400));
}
void BaseContentLayer::setCCBladeVisible(bool visibility)
{
	touchTrailLayer->setVisible(visibility);
	touchTrailLayer->setTouchEnabled(visibility);
}
void BaseContentLayer::timeToFlowlightAction(float time)
{
    unschedule(schedule_selector(BaseContentLayer::timeToFlowlightAction));
    schedule(schedule_selector(BaseContentLayer::timeToFlowlightAction),
             TIME_FLOWLIGHT_ACTION+((float)TIME_FLOWLIGHT_ACTION)*CCRANDOM_0_1());
    
    
//    flowLightDoAction();
}

BaseContentLayer::~BaseContentLayer()
{
	m_bRecord = false;
	m_bPlayRecord = false;
	m_AllFramesData.clear();
	m_AllNeedRecordSpriteVec.clear();
//	if (m_iCurPlayMode == playMode_Normal && m_bRecord)
	{
		//rope
		for (size_t i = 0; i < mRopes.size(); ++i) {
			delete mRopes[i];
		}
	}
	mRopes.clear();
	CCNotificationCenter::sharedNotificationCenter()->removeObserver(this,MSG_FIXED_SCALE_END);
	// 超能道具
	CCNotificationCenter::sharedNotificationCenter()->removeObserver(this,MSG_USE_SUPERTOOL);

	springBedAction->release();
	m_iCurRecordFrame = 0;
	m_iPlayFrame = 0;
    mTimerDecList.clear();
    
    effectveTableVec.clear();
    toDestoryVec.clear();
    
    motorVec.clear();
    //boilerVec.clear();
    
    //strobeVec.clear();
    emVec.clear();
    
    CC_SAFE_DELETE(mContactListener);
	CC_SAFE_DELETE(mContactFilter);
	CC_SAFE_DELETE(mWorld);
    
    for (size_t i=0; i<mGameTableMovementDataVec.size(); ++i) {
        removeChild(mGameTableMovementDataVec[i].ghost, true);
		//ghost已经调用了autorelease，这儿就不用再release了
//        CC_SAFE_RELEASE(mGameTableMovementDataVec[i].ghost);
    }

	unschedule(schedule_selector(BaseContentLayer::tick));
	unschedule(schedule_selector(BaseContentLayer::begineThrowPointSchedule));
	unschedule(schedule_selector(BaseContentLayer::timeToFlowlightAction));
	unschedule(schedule_selector(BaseContentLayer::preBomb));
    
    /*
     for (int i=0; i<mSpriteVec.size(); ++i) {
     CCNode* node = (CCNode*)mSpriteVec[i];
     node->release();
     }
     */

	if(CCDirector::sharedDirector()->getRunningScene()){
		if(CCDirector::sharedDirector()->getRunningScene()->getChildByTag(Tag_SpecialItem_Copy))
			CCDirector::sharedDirector()->getRunningScene()->removeChildByTag(Tag_SpecialItem_Copy);
	}

	GameScenseDefine::SharedGameScenseDefine()->removeBaseContentLayer(this);
	
	if (GameScenseDefine::SharedGameScenseDefine()->getAllBaseContentLayerVec()->size() > 0)
	{
		curInstance = GameScenseDefine::SharedGameScenseDefine()->getAllBaseContentLayerVec()->at(GameScenseDefine::SharedGameScenseDefine()->getAllBaseContentLayerVec()->size()-1);
	}
	else
	{
		curInstance = NULL;
	}

	if (GameScenseDefine::SharedGameScenseDefine()->getAllBaseContentLayerVec()->size() > 1)
	{		
		preInstance = GameScenseDefine::SharedGameScenseDefine()->getAllBaseContentLayerVec()->at(GameScenseDefine::SharedGameScenseDefine()->getAllBaseContentLayerVec()->size()-2);
	}
	else
	{
		preInstance = NULL;
	}

	if (m_iCurPlayMode == playMode_PlayGameRecord)
	{
		mScense->setContentLayerWapper((BaseContentWrapLayer*)mScense->getChildByTag(Tag_Layer_BaseContetWapper), false);
	}

#if SUPER_TOOL_TYPE == SUPER_TOOL_TYPE_LIKECUTROPE
	
	removeSuperTool();
#endif

}

// fileName 是连带路径的
void BaseContentLayer::addBluredSpriteToMap(std::string fileName){

	//不需要了
#if 0
	if (CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(fileName.c_str()) == NULL)
	{
		bool canAdd = true;
		for(size_t i = 0;i<g_NeedBlurFileNameVec.size();i++)
			if (g_NeedBlurFileNameVec[i] == fileName)
				canAdd = false;
		if(canAdd)
			g_NeedBlurFileNameVec.insert(g_NeedBlurFileNameVec.end(),fileName);
		
		//还没有创建
		CCImage* spriteImage = new CCImage;
		spriteImage->initWithImageFile(fileName.c_str());

		CCTexture2DMutable* texture = new CCTexture2DMutable();
		if (!texture->initWithData(spriteImage->getData(), kCCTexture2DPixelFormat_RGBA8888, spriteImage->getWidth(), spriteImage->getHeight(), CCSizeMake(spriteImage->getWidth(), spriteImage->getHeight()))) {
			CCLog("Could not create texture");
			delete texture;
			texture = NULL;
		}
		texture->apply();
		AWTextureFilter txrFilter;
		if (g_IsRetina) {
			txrFilter.blur(texture, GaussianBlurRadiusForSpriteDisappear*2);
		}else{
			txrFilter.blur(texture, GaussianBlurRadiusForSpriteDisappear);
		}
		CCSize texSize = texture->getContentSize();
		CCRect texRec = CCRectMake(0,0,texSize.width,texSize.height);
		CCSpriteFrame* frame = CCSpriteFrame::createWithTexture(texture,texRec);
		CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFrame(frame,fileName.c_str());

		spriteImage->release();
		//texture->release();
	}
#endif
}
//
//void BaseContentLayer::setSectionGuide(SectionGuide* guide)
//{
//	this->guide = guide;
//}

BaseContentLayer* BaseContentLayer::shareBaseContentLayer()
{
    return curInstance;
}

void BaseContentLayer::setCurBaseContentLayer(BaseContentLayer* pBaseContentLayer)
{
	curInstance = pBaseContentLayer;
}

BaseContentLayer* BaseContentLayer::getPreBaseContentLayer()
{
	return preInstance;
};

void BaseContentLayer::setPreBaseContentLayer(BaseContentLayer* pBaseContentLayer)
{
	preInstance = pBaseContentLayer;
};

void BaseContentLayer::tick(ccTime dt)
{
    if (mLayerFreezed) {
        return;
    }

#if SUPER_TOOL_TYPE == SUPER_TOOL_TYPE_NO_PHYSICS
	if (mUseSuperTool)
	{
		return;
	}
#endif
	m_fTickTotalTime += dt;

	std::map<float, CCPoint>::iterator it;
	for (it = m_TouchMovedPointsMap.begin(); it != m_TouchMovedPointsMap.end();)
	{
		if (m_fTickTotalTime - it->first >= 0.1f)
		{
			m_TouchMovedPointsMap.erase(it++);
		}
		else
		{
			it++;
		}
	}

	Runtime::getRuntime()->gameLoop(dt);

	AchievementManager::getInstance()->tick(dt);

    //查看是否有fixture由于碰撞而大幅改变了速度从而需要播放音效
    {
        for (b2Body* b = mWorld->GetBodyList(); b; b = b->GetNext())
		{
			CustomeSprite* sprite = (CustomeSprite*)b->GetUserData();
            //只处理动态刚体
            if (b->GetType() != b2_dynamicBody) {
                continue;
            }
            if (sprite->mFixtureVelocityInfoVec.size() == 0) {
                b2Fixture* fixture = b->GetFixtureList();
                for (; fixture!=NULL; fixture=fixture->GetNext()){
                    FixtureVelocityInfo info;
                    info.fixture = fixture;
                    info.collisionImpulse = 0.0f;
                    info.linerVelocityBeforeContact = b2Vec2(0, 0);
                    info.fixtureCenterBeforeContack = b2Vec2(0, 0);
                    
                    sprite->mFixtureVelocityInfoVec.insert(sprite->mFixtureVelocityInfoVec.end(), info);
                }
            }
            //
            float volume = 0.0f;
			string volumeSuffix = "";
            for (size_t i=0; i<sprite->mFixtureVelocityInfoVec.size(); ++i){
                
                //计算当前fixture的线速度
                b2Vec2 linerVelocityCurrent;
                {
                    //
                    linerVelocityCurrent = b->GetLinearVelocity();
                    if (linerVelocityCurrent.y > 0.1) {
                        //CCLog("linerVelocityCurrent[%f,%f]", linerVelocityCurrent.x, linerVelocityCurrent.y);
                    }
                    
                    b2Vec2 center = sprite->mFixtureVelocityInfoVec[i].fixture->GetAABB(0).GetCenter();
                    if (!(center == b->GetPosition())) {
                        b2Vec2 disV = center - b->GetPosition();
                        float radius = sqrt(pow(disV.x, 2)+pow(disV.y, 2));
                        float speed = radius * b->GetAngularVelocity();
                        float xVelocity = - speed * disV.y/radius;
                        float yVelocity = speed * disV.x/radius;
                        linerVelocityCurrent.x += xVelocity;
                        linerVelocityCurrent.y += yVelocity;
                    }
                    
                }
                
                //update fixture info evary tick
                b2Vec2 linerVelocityBeforeContact = sprite->mFixtureVelocityInfoVec[i].linerVelocityBeforeContact;
                sprite->mFixtureVelocityInfoVec[i].linerVelocityBeforeContact = linerVelocityCurrent;
                b2Vec2 fixtureCenterBeforeContack = sprite->mFixtureVelocityInfoVec[i].fixtureCenterBeforeContack;
                sprite->mFixtureVelocityInfoVec[i].fixtureCenterBeforeContack = sprite->mFixtureVelocityInfoVec[i].fixture->GetAABB(0).GetCenter();
                
                //fixture是否是发生了足够冲量的碰撞
                if(sprite->mFixtureVelocityInfoVec[i].collisionImpulse < MinImpulseForCollisionEffectSound){
                    continue;
                }
                sprite->mFixtureVelocityInfoVec[i].collisionImpulse = 0.0f;
                
                //是否产生足够的位移
                b2Vec2 deltaP = fixtureCenterBeforeContack - sprite->mFixtureVelocityInfoVec[i].fixture->GetAABB(0).GetCenter();
                float deltaDistance = sqrt(pow(deltaP.x, 2) + pow(deltaP.y, 2));
                if(deltaDistance < MinDeltaSpeedForCollisionEffectSound/60/3){
                    continue;
                }
                
                //线速度改变产生的音量
                b2Vec2 deltaV = linerVelocityBeforeContact - linerVelocityCurrent;
                float deltaSpeed = sqrt(pow(deltaV.x, 2) + pow(deltaV.y, 2));

                //stub
                if (deltaSpeed >= MinDeltaSpeedForCollisionEffectSound) {
                    //CCLog("deltaSpeed=%f", deltaSpeed);
                }
                

				//
				float tmpVolume = 0.0f;
				if (deltaSpeed >= MinDeltaSpeedForCollisionEffectSound*6) {
					tmpVolume = 1.2f;
				}else if (deltaSpeed >= MinDeltaSpeedForCollisionEffectSound*5) {
					tmpVolume = 1.0f;
				}else if (deltaSpeed >= MinDeltaSpeedForCollisionEffectSound*4) {
					tmpVolume = 0.8f;
				}else if (deltaSpeed >= MinDeltaSpeedForCollisionEffectSound*3) {
					tmpVolume = 0.6f;
				}else if (deltaSpeed >= MinDeltaSpeedForCollisionEffectSound*2) {
					tmpVolume = 0.4f;
				}else if (deltaSpeed >= MinDeltaSpeedForCollisionEffectSound) {
					tmpVolume = 0.2f;
				}

				if (tmpVolume > volume ) {
					volume = tmpVolume;
				}
            }
            if (volume > 0) {
                char soundName[32];
                {
                    if (b->GetFixtureList()->GetRestitution() > 0) {
                        sprintf(soundName, "ball_bounce.wav");
                    }else{
                        float r = CCRANDOM_0_1();
                        int i = r/0.2;
                        sprintf(soundName, "wood collision a%d.wav", i+1);
                    }
                }

				string fileName = Music::sharedMusicPlayer()->getDefaultVolumeFile(soundName,volume);
                Music::sharedMusicPlayer()->playEffectSound(fileName.c_str());
            }
		}
    }
	//It is recommended that a fixed time step is used with Box2D for stability
	//of the simulation, however, we are using a variable time step here.
	//You need to make an informed choice, the following URL is useful
	//http://gafferongames.com/game-physics/fix-your-timestep/
	
	int velocityIterations = 8;
	int positionIterations = 4;
    
	// Instruct the world to perform a single step of simulation. It is
	// generally best to keep the time step and iterations fixed.
	//mWorld->Step(1.0f/60.0f, velocityIterations, positionIterations);
    
    if (timeAscing) {
        mWorld->Step(dt*2, velocityIterations, positionIterations);
    }else{
        mWorld->Step(dt, velocityIterations, positionIterations);
    }

    windBlowing();//刮风
    
	//Iterate over the bodies in the physics world
	for (b2Body* b = mWorld->GetBodyList(); b; /*b = b->GetNext()*/)
	{        
		if (b->GetUserData() != NULL) {
			//Synchronize the AtlasSprites position and rotation with the corresponding body
			CCSprite* myActor = (CCSprite*)b->GetUserData();
            
//            if (!g_IsRetina) {
//                //stub黑暗关卡
//                if (myActor->getTag() == Tag_Box_RedStar) {
//                    if (mLighting == NULL) {
//                        
//                        CCSize trueWinSize = CCDirector::sharedDirector()->getWinSize();
//                        float width = trueWinSize.width*3;
//                        float height = trueWinSize.height*3;
//                        mLighting = Lighting::renderTextureWithWidthAndHeight(width, height);
//                        CCPoint centerOffWin = ccp(trueWinSize.width/2, trueWinSize.height/2);
//                        CCLayer* null_layer = new CCLayer;
//                        mLighting->setObjectLayer(null_layer);
//                        mLighting->setPosition(myActor->getPosition());
//                        mScense->addChild(mLighting, 200);
//                        mLighting->addLight(ccp(width/2,height/2));
//                        mLighting->updateLighting();
//                    }
//                    
//                    mLighting->setPosition(myActor->getPosition());
//                }
//            }
            
            
            
            //时间减慢
            {
                list<TimerDec*>::iterator timer;
                for (timer = mTimerDecList.begin(); timer!=mTimerDecList.end(); timer++)
                {
                    TimerDec* t = (*timer);
                    
                    if (t->isInTimeDecArea(myActor)) {
                        
                        b->SetLinearDamping(5.0f);
                        b->SetAngularDamping(5.0);

						break;
                    }else{
						
						//如果开启了超能道具
						if(mUseSuperTool)
						{
							//修改所有物体的阻尼系数
							//if(myActor->getTag() == Tag_Box_RedStar)
							{
								b->SetLinearDamping(1.0);
								b->SetAngularDamping(1.0);
							}
						}
						else
						{
							b->SetLinearDamping(0.0f);
							b->SetAngularDamping(0.0f);
						}
						
                        
                    }
                }
                if (mTimerDecList.size()<=0) {
					//如果开启了超能道具
					if(mUseSuperTool)
					{
						//修改所有物体的阻尼系数
						//if(myActor->getTag() == Tag_Box_RedStar)
						{
							b->SetLinearDamping(1.0);
							b->SetAngularDamping(1.0);
						}
					}
					else
					{
						b->SetLinearDamping(0.0f);
						b->SetAngularDamping(0.0f);
					}
                }
            }
            
            if (myActor->getTag() == Tag_Box_RedStar)
			{
				//设置粒子系统特效位置
				if (redStar_StarPar!=NULL)
				{
					redStar_StarPar->setPosition(myActor->getPosition());
				}
				if (redStar_FirePar!= NULL)
				{
					redStar_FirePar->setPosition(myActor->getPosition());
				}

#if 0
				//根据速度动态改变粒子数量，速度越快粒子越多
				b2Vec2 curVelo = b->GetLinearVelocity();
				float veloPar =  sqrt(curVelo.x * curVelo.x + curVelo.y * curVelo.y);
				int baseParticleNum = 10;
				int particleNum = veloPar < 1.0f ? baseParticleNum : baseParticleNum * veloPar;
				redStar_StarPar->setTotalParticles(particleNum);
#endif

                if (myActor->getPosition().y < -200) {
                    if (mFailedWhenRedStarFall) {
                        mScense->failed();
                        return;
                    }else {
                        
                        b2Body* tmp = b;
                        b = b->GetNext();
                        
                        //将红心从效果队列中取出来
                        for (size_t i=0; i < effectveRedStarVec.size(); i++) {
                            if (effectveRedStarVec[i] == myActor) {
                                effectveRedStarVec[i] = NULL;
                            }
                        }
                        mWorld->DestroyBody(tmp);
                        ((CustomeSprite*)myActor)->removeShadowIfPosible();
                        removeChild(myActor, true);
                        
                        //清除红星标志
                        mRedstarExist = false;

						AchievementManager::getInstance()->setRedStar(NULL,NULL,false);
                        
                        continue;
                    }
                    
                }
				else if (myActor->getPosition().y > mScreenSize.height + 250) 
				{
					AchievementManager::getInstance()->redStarFlyOutOffScreenAndFailed();
                    mScense->failed();
                    return;
                }
				else if (myActor->getPosition().x > mScreenSize.width + 100 || myActor->getPosition().x < - 100) 
				{
					if (mFailedWhenRedStarFall)
						mScense->failed();
				}
				else if (mRedstarContactTable && b->GetPosition().y > (contactedTable->GetPosition().y+0.5))
				{
                    //红星接触到了桌面,而且红星在桌面上方
                    
                    //红星相对于桌面的位置是否出现加大位移
                    b2Vec2 shift = mPositionRedstarFromTable - (b->GetPosition() - contactedTable->GetPosition());
					AchievementManager::getInstance()->redStarOnTable(shift,dt);
                    //CCLog("shift=(%f, %f)", shift.x, shift.y);
                    if (abs(shift.x) <= 0.01 && abs(shift.y) <= 0.01) {
                        mRedstarContactTableTime += dt;
                        
						//AchievementManager::getInstance()->redStarOnTableStatic(dt);

                        if (mRedstarContactTableTime >= RedstarStayOnTable_TimeToSuc) {
                            
							{//设置成功在了关键台面的标记
								CustomeSprite* table = (CustomeSprite*)contactedTable->GetUserData();
								if (table->isKeyItem())
									Runtime::getRuntime()->setSuccessdOnKeyTable();
							}

							Music::sharedMusicPlayer()->playEffectSound("win.wav");

							Runtime::getRuntime()->genGameScore();

							// 给成就传递信息
							{
								CustomeSprite* table = (CustomeSprite*)contactedTable->GetUserData();
								AchievementManager::getInstance()->sucessed(myActor,table,contactedTable);
							}
							// 台面显示粒子效果
							{
								//if (getMachinePerformanceLevel() > 0)
								{
									//CustomeSprite* table = (CustomeSprite*)contactedTable->GetUserData();
									//CCParticleSystem* star = createRedStarStarPar(ccp(table->getContentSize().width/2,table->getContentSize().height/3));
									//CCParticleSystem* fire = createRedStarFirePar(ccp(table->getContentSize().width/2,table->getContentSize().height/3));
									//addChild(star);
									//addChild(fire);
									//star->setPosition(table->getPosition());
									//fire->setPosition(table->getPosition());
								}
								
							}
                            //显示桌面吸纳红星的特效,以及桌面的特效
                            {
								if (redStar_FirePar != NULL)
								{
									redStar_FirePar->setVisible(false);
								}
								if (redStar_StarPar != NULL)
								{
									redStar_StarPar->setVisible(false);
								}
								
                                freezeLayer();
								mScense->getMenuLayer()->setGameMenuEnable(false);
								mScense->getMenuLayer()->setVisible(false);
								//道具移除
								CCDirector::sharedDirector()->getRunningScene()->removeChildByTag(Tag_SpecialItem_Copy);

                                CCSprite* redStar = (CCSprite*)b->GetUserData();
                                CCSprite* table = (CCSprite*)contactedTable->GetUserData();
                                
								{
									redStar->stopAllActions();
									int redStarChildNum = redStar->getChildrenCount();
									CCArray* children = redStar->getChildren();
									for (int i=0;i<redStarChildNum;i++)
									{
										((CCNode*)children->objectAtIndex(i))->stopAllActions();
									}
								}
								((CustomeSprite*)redStar)->removeShadowIfPosible();
								
								showRedStarSuccess(redStar, table);

								if (m_iCurPlayMode == playMode_Normal && m_bRecord)
								{
									setRecord(false);
								}
//                                 CCFiniteTimeAction* redStarMoveToTableHole = CCSpawn::create(
// 									CCScaleTo::create(0.5f, 0.3f),
// 									CCRotateTo::create(0.5f, 0.0f),
// 									CCMoveTo::create(0.5f, ccpSub(table->getPosition(),ccp(0,11))),NULL);
// 								CCFiniteTimeAction* dropIntoWhole = CCSpawn::create(
// 									CCScaleTo::create(0.2f,0.1f),
// 									CCMoveBy::create(0.5f,ccp(0,-5)),
// 									CCTintTo::create(0.3f,5,5,5),
// 									CCFadeTo::create(0.3f,0),NULL);
// 
// 								CCFiniteTimeAction* getOutFormExit = CCSpawn::create(
// 									CCScaleTo::create(0.2f,0.5f),
// 									CCFadeTo::create(0.2f,255),
// 									CCTintTo::create(0.2f,255,255,255),NULL);
//
// 								{
// 									int redStarChildNum = redStar->getChildrenCount();
// 									CCArray* children = redStar->getChildren();
// 									for (int i=0;i<redStarChildNum;i++)
// 									{
// 										((CCNode*)children->objectAtIndex(i))->runAction(CCSequence::create(
// 											CCDelayTime::create(0.5f),	// 等待红星掉进到洞口
// 											CCDelayTime::create(0.2f),	// 等待
// 											//CCDelayTime::create(0.7f),	// 等待已经缩小到洞里面
// 
// 											CCDelayTime::create(0.2f),	//
// 											CCSpawn::create(CCTintTo::create(0.3f,5,5,5),
// 															CCFadeTo::create(0.3f,0),NULL),
// 											CCDelayTime::create(0.2f),	// 等待放大
// 
// 											CCSpawn::create(CCTintTo::create(0.2f,255,255,255),
// 															CCFadeTo::create(0.2f,255),NULL),
// 											NULL));
// 									}
// 								}
//
// 								redStar->runAction(CCSequence::create(
// 									redStarMoveToTableHole,	// 移动到洞里面
// 									CCDelayTime::create(0.2f),
// 									dropIntoWhole,
// 									CCDelayTime::create(0.2f),
// 									CCCallFuncN::create(this,SEL_CallFuncN(&BaseContentLayer::redStarGetInTheHole)), // 重置了红星的位置
// 									getOutFormExit,
// 									CCDelayTime::create(0.12f),
// 									CCCallFuncN::create(this,SEL_CallFuncN(&BaseContentLayer::endEffectiveEnd)),NULL));
                            }
                        }
                    }else {
                        //红星移动了，时间清零
                        mRedstarContactTableTime = 0;
						//AchievementManager::getInstance()->redStarOnTableMove();
                        mPositionRedstarFromTable = b->GetPosition() - contactedTable->GetPosition();
                    }
                }
            }
            else
			{//不是红星
                //滑轨的锚点在屏幕外面时不销毁
                if (myActor->getPosition().y < -200 && myActor->getTag()!=Tag_Anchor_SpringSlide) {
                    b2Body* tmp = b;
                    b = b->GetNext();
                    
                    //
                    checkRopesWhenBodyDestroied(tmp,true);
                    
                    mWorld->DestroyBody(tmp);
                    ((CustomeSprite*)myActor)->removeShadowIfPosible();
                    removeChild(myActor, true);
                    continue;
                }
            }
            
            //对于滑轨的锚点，不需要更新位置
            if (myActor->getTag() != Tag_Anchor_SpringSlide)
            {
                myActor->setPosition( CCPointMake( b->GetPosition().x * PTM_RATIO, b->GetPosition().y * PTM_RATIO) );
            }
			//马达的旋转我们自己控制
            if (myActor->getTag() != Tag_Anchor_Mator) {
                myActor->setRotation( -1 * CC_RADIANS_TO_DEGREES(b->GetAngle()) );
            }
            //
            b = b->GetNext();
		}
	}
    
    //销毁需要销毁的物体
    for (size_t i=0; i<toDestoryVec.size(); i++) {

		checkRopesWhenBodyDestroied(toDestoryVec[i],true);
		//b2Body* ropeAnchor = checkRopesWhenBodyDestroied(toDestoryVec[i]);
        
        b2Body* shapeAnchor = NULL;
        shapeAnchor  = checkAnchorWhenShapeDestoryed((CustomeSprite*)toDestoryVec[i]->GetUserData());
        
        
        if (toDestoryVec[i]->GetUserData() != NULL) {
            CCNode* node = (CCNode*)toDestoryVec[i]->GetUserData();
            toDestoryVec[i]->SetUserData(NULL);
            
            node->setVisible(false);
            if (node->getTag() == Tag_Box_RedStar) {
                redStarDestoried(node);
            }
            
            //
            ((CustomeSprite*)node)->removeShadowIfPosible();
            removeChild(node, true);
            
        }
        mWorld->DestroyBody(toDestoryVec[i]);

        //if (ropeAnchor!=NULL) {
        //    if (ropeAnchor->GetUserData()!=NULL) {
        //        ((CCNode*)ropeAnchor->GetUserData())->runAction(CCFadeTo::create(0.5f, 0));
        //    }
        //    ropeAnchor->SetUserData(NULL);
        //    mWorld->DestroyBody(ropeAnchor);
        //}
        
        if (shapeAnchor!=NULL) {
            if (shapeAnchor->GetUserData()!=NULL) {
                
                ((CCNode*)shapeAnchor->GetUserData())->removeFromParentAndCleanup(true);
                shapeAnchor->SetUserData(NULL);
                
                mWorld->DestroyBody(shapeAnchor);
            }
            
        }
    }
    toDestoryVec.clear();
    
    //bomb
    for (size_t i=0; i<mBombToExpolsionVec.size(); ++i) {
        bombExplosion(mBombToExpolsionVec[i]);
    }
    mBombToExpolsionVec.clear();
    
    //tablemove
    static int tablemoveCalcCount = 0;
    tablemoveCalcCount++;
    if (tablemoveCalcCount == 1) {
        tablemoveCalcCount = 0;
        
        for (size_t i=0; i<mGameTableMovementDataVec.size(); ++i) {
            CCPoint pNow = mGameTableMovementDataVec[i].ghost->getPosition();
            float xDelta = pNow.x - mGameTableMovementDataVec[i].ghostLastTickPosition.x;
            float yDelta = pNow.y - mGameTableMovementDataVec[i].ghostLastTickPosition.y;
            b2Vec2 Velocity = b2Vec2(-xDelta/PTM_RATIO/dt, yDelta/PTM_RATIO/dt);
            mGameTableMovementDataVec[i].body->SetLinearVelocity(Velocity);
            
            mGameTableMovementDataVec[i].ghostLastTickPosition = pNow;
        }
    }

    
    //几何体消失
	if (mSpriteToDisappear != NULL) {
        float scale = mSpriteToDisappear->getScale();
        if (scale == 1) {
            scale = 0.9f;
        }else{
            scale -= dt/2;
        }
		
		if (scale < 0) {
			scale = 0;
		}
		mSpriteToDisappear->setScale(scale);
        
        //计算下落时长
        mSpriteToDisappearFallTime += dt;

		CCPoint pos = mSpriteToDisappear->getPosition();
        //计算y坐标
        float g = 200.0f; //重力加速度系数
        float a = 0.3f; //开始抛起到顶点的持续时长
        pos.y += (-g*dt*mSpriteToDisappearFallTime + a*g*dt)/(mSpriteToDisappearFallTime + 1);
        //计算X坐标
        pos.x += mSpriteToDisappearSpeedX*dt*300/(mSpriteToDisappearFallTime+1);
        //
        mSpriteToDisappear->setPosition(pos);
        
        //计算旋转
        float angel = mSpriteToDisappear->getRotation();
        angel += mSpriteToDisaappearFallRotation*dt*2;
        mSpriteToDisappear->setRotation(angel);

		if (pos.y < 0 || scale <= 0) {
			removeChild(mSpriteToDisappear, true);
			mSpriteToDisappear = NULL;
		}
	}

	// +++ Update rope physics
	for(size_t i=0;i<mRopes.size();i++) {
		mRopes[i]->update(dt);
	}

    
    //锅炉
    if (mActiveBoiler != NULL) {
        boilerApplyForce(mActiveBoiler, dt);
    }

#if SUPER_TOOL_TYPE == SUPER_TOOL_TYPE_LIKECUTROPE
	if (mUseSuperTool && mSuperTool)
	{
		mSuperTool->blow_tick(dt);
	}
#endif
	specialToolCollide();
	
	updateRecordInfo();

#if 0
	if (m_bAnimationOver && m_pArmature && m_pArmature->getAnimation()->getCurrentMovementID() == g_ThiefAnimation_zuo)
	{
		removeFromRecordSpriteVec(m_WillBeRemoveRecordSpriteVec);
		for (int i = 0; i < m_AllNeedRecordSpriteVec.size(); i++)
		{
			RecordSprite* pRecordSprite = m_AllNeedRecordSpriteVec.at(i);
			if (pRecordSprite->getTag() == Tag_Box_RedStar
				|| pRecordSprite->getTag() == Tag_Box_can_Destroied
				|| pRecordSprite->getTag() == Tag_Box_cannot_Destroied
				|| pRecordSprite->getTag() == Tag_Box_Bomb
				|| pRecordSprite->getTag() == Tag_Box_Bomb_Cirecle
				|| pRecordSprite->getTag() == Tag_Box_Bomb_Hard
				|| pRecordSprite->getTag() == Tag_Box_Bomb_DarkWhole
				|| pRecordSprite->getTag() == Tag_Timer_Dec_Bomb
				|| pRecordSprite->getTag() == Tag_Timer_Asc_Bomb)
			{
				bool bPlayAnimation = true;
				for (int j = 0; j < m_CollisionWithRoleNode.size(); j++)
				{
					if (m_CollisionWithRoleNode.at(j) == pRecordSprite)
					{
						bPlayAnimation = false;
						break;
					}
				}

				if (!bPlayAnimation)
				{
					continue;
				}

				CCRect rect = m_pArmature->boundingBox();

				rect.size.width *= 0.5f;
				rect.size.height *= 0.5f;

				rect.origin.x = convertToNodeSpace(m_pArmature->getPosition()).x - rect.size.width/2;
				rect.origin.y = convertToNodeSpace(m_pArmature->getPosition()).y - rect.size.height/2;

				if (rect.intersectsRect(pRecordSprite->boundingBox()))
				{
					m_CollisionWithRoleNode.push_back(pRecordSprite);
					m_pArmature->getAnimation()->play(g_ThiefAnimation_jingxia.c_str());
					break;
				}
			}
		}
	}
#endif
}

void BaseContentLayer::updateRecordInfo()
{
	//记录元素的属性，统一放到这个函数里面记录
	removeFromRecordSpriteVec(m_WillBeRemoveRecordSpriteVec);
	if (m_iCurPlayMode == playMode_Normal && m_bRecord && !mUseSuperTool)
	{
		//只记录位置变化的元素
		if (m_AllFramesData.size() >= 1)
		{
			for (size_t i = 0; i < m_AllNeedRecordSpriteVec.size(); i++)
			{
				RecordSprite* pRecordSprite = m_AllNeedRecordSpriteVec.at(i);
				if (fabs(pRecordSprite->getPositionX() - (pRecordSprite->m_SaveData.posX + (mScreenSize.width/2 - designResolutionSize.width/2) )) > 0.0005f
					|| fabs(pRecordSprite->getPositionY() - (pRecordSprite->m_SaveData.posY + (mScreenSize.height/2 - designResolutionSize.height/2) )) > 0.0005f
					|| fabs(pRecordSprite->getRotation() - pRecordSprite->m_SaveData.rotate) > 0.0005f)
				{
					m_CurNeedRecordSpriteVec.push_back(pRecordSprite);
				}
			}
		}
		else
		{
			m_CurNeedRecordSpriteVec = m_AllNeedRecordSpriteVec;
		}

		if (m_CurNeedRecordSpriteVec.size() > 0 || m_CurFrameData.size() > 0)
		{
			++m_iCurRecordFrame;
		}

		for (size_t i = 0; i < m_CurNeedRecordSpriteVec.size(); i++)
		{
			RecordSprite* pRecordSprite = m_CurNeedRecordSpriteVec.at(i);
			int iTag = pRecordSprite->getTag();
			if (iTag == Tag_Mator_Turn_Anchor)
			{
				continue;
			}
			pRecordSprite->m_SaveData.posX = pRecordSprite->getPositionX();
			pRecordSprite->m_SaveData.posY = pRecordSprite->getPositionY();
			pRecordSprite->m_SaveData.rotate = pRecordSprite->getRotation();

			if (iTag != Tag_In_DarkBomb)
			{
				//为了适配所有的分辨率，记录的时候需要减去一个 偏移（屏幕的宽度的一半减去设计分辨率的一半），这样才能保证这个位置是一个最原始的位置，和分辨率无关的位置
				//当播放的时候，加上这个偏移，因为屏幕分辨率不同，这样每次加上的偏移就不同，就保证了在不同分辨率下显示的位置是一致的
				pRecordSprite->m_SaveData.posX = pRecordSprite->m_SaveData.posX - (mScreenSize.width/2 - designResolutionSize.width/2);
				pRecordSprite->m_SaveData.posY = pRecordSprite->m_SaveData.posY - (mScreenSize.height/2 - designResolutionSize.height/2);
			}
			m_CurFrameData.push_back(pRecordSprite->m_SaveData);

			//除了锅炉的开始事件外，记录完成后都要重置事件ID
			if (pRecordSprite->m_SaveData.eventId != eventID_BoilerStart)
			{
				pRecordSprite->m_SaveData.eventId = eventID_None;
			}
		}
		if (m_CurFrameData.size() > 0)
		{
//			CCLOG("------------------->%s, m_iCurRecordFrame is %d, m_CurFrameData count is %d", __FUNCTION__, m_iCurRecordFrame, m_CurFrameData.size());
			m_AllFramesData.push_back(m_CurFrameData);
		}
		
		m_CurFrameData.clear();
		m_CurNeedRecordSpriteVec.clear();
	}
}

void BaseContentLayer::redStarGetInTheHole(CCNode* redStar)
{
}
void BaseContentLayer::endEffectiveEnd(CCNode* redStar)
{
	mScense->congratulation();
  //  BaseContentLayer* layer = (BaseContentLayer*)baseContentLayer;
  //  try {
  //      
  //      if (layer->mActiveBoiler!=NULL) {
  //          Boiler* boiler = (Boiler*)(layer->mActiveBoiler)->GetUserData();
  //          boiler->stop();
  //          layer->mActiveBoiler = NULL;
  //      }
		//setCCBladeVisible(false);
  //      layer->mScense->congratulation();
  //  } catch (std::exception e) {
  //      return;
  //  }
}

void BaseContentLayer::showRedStarSuccess(CCNode* pRedStar, CCNode* table)
{
	m_bSuccessful = true;
	if (BaseGuideLayer* pGuideLayer = (BaseGuideLayer*)mScense->getGuideLayer())
	{
		pGuideLayer->removeFromParent();
	}

	if (mUseSuperTool || m_iCurRecordFrame > s_iMaxRecordTime)
	{
        std::vector< std::vector<record> >().swap(m_AllFramesData);
		m_AllFramesData.clear();
	}

	m_pSuccessRedSar   = pRedStar;
	m_pSuccessPlatform = table;
	// 	endEffectiveEnd(NULL);
	// 	return;

#if 0 //星星破碎动画
	CCArmature* pArmature = CCArmature::create("xingxing");
	pArmature->getAnimation()->play("suilie");
	pRedStar->getParent()->addChild(pArmature, pRedStar->getZOrder()+1);
	pArmature->setPosition(pRedStar->getPosition());
#endif


	//星星被吸收的拖影效果
	CCSize tableSize = table->getContentSize();
	CCPoint destPos  =  table->getPosition();
	float time = 0.6f;
	CCEaseOut* move = CCEaseOut::create(
		CCSpawn::create(
		CCMoveTo::create(time, destPos), 
		CCScaleTo::create(time, 0.2f, 0.2f),
		CCRotateBy::create(time, time * 2 * 360.0f), NULL)
		, 1.0f
		);
	pRedStar->runAction(CCSequence::create(move, CCCallFuncN::create(this, callfuncN_selector(BaseContentLayer::showTableFinishAnim)), NULL));

	for( int i = 0; i < 4; i++ )
	{
		CCEaseOut* move = CCEaseOut::create(
			CCSpawn::create(
			CCMoveTo::create(time, destPos), 
			CCScaleTo::create(time, 0.0f, 0.0f),
			CCRotateBy::create(time, time * 2 * 360.0f), NULL)
			, 1.0f
			);
		CCSprite* shadow = new CCSprite();
		shadow->initWithFile( ResManager::getManager()->getSharedFilePath(g_gameinPath+"star60.png").c_str() );
		shadow->setRotation(pRedStar->getRotation());
		pRedStar->getParent()->addChild(shadow);
		shadow->release();
		shadow->setPosition(pRedStar->getPosition());
		//shadow->setZOrder(pRedStar->getZOrder() - i - 1);
		shadow->setOpacity(100 - i * 20);
		shadow->runAction(CCSequence::create(CCDelayTime::create((i+1) * 0.07f), move, NULL));
	}

	if (mSafeBoarder)
	{
		mSafeBoarder->getCoverLayer()->removeFromParent();
	}

	removeSuperTool();

}



void BaseContentLayer::showTableFinishAnim(CCNode* pRedStar)
{
	CCArmature* pArmature = CCArmature::create("starfall");
	ccBlendFunc blend = {GL_SRC_ALPHA, GL_ONE};
	pArmature->setBlendFunc(blend);
	m_pSuccessPlatform->addChild(pArmature);
	CCSize size = m_pSuccessPlatform->getContentSize();
	pArmature->setPosition(ccp(size.width/2, size.height/2));
	pArmature->setOpacity(0xCC);
	CCArmatureAnimation* anim = pArmature->getAnimation();
	CCMovementData* moveData = anim->getAnimationData()->getMovement("movelight");
	//修改动画的起始帧（左右位置）为实际的平台左右位置
	moveData->getMovementBoneData("movelight_L")->getFrameData(0)->x = -size.width/2;
	moveData->getMovementBoneData("movelight_R")->getFrameData(0)->x   = size.width/2;
	anim->play("movelight");
	anim->setMovementEventCallFunc(this, movementEvent_selector(BaseContentLayer::showDiamond));
}

void BaseContentLayer::showDiamond(cocos2d::extension::CCArmature *pArmature, cocos2d::extension::MovementEventType movementType, const char *movementID)
{
	if(movementType == COMPLETE)
	{
		//钻石掉落动画
		CCPoint point = convertToWorldSpace(m_pSuccessRedSar->getPosition());
		CCSprite* pDiamond = CCSprite::create(ResManager::getManager()->getSharedFilePath(g_gameinPath + "youxizhong_xingxing.png").c_str());
		mScense->getBaseContentWrapLayer()->addChild(pDiamond, 1999, diamond_tag);
		pDiamond->setPosition(point);
		m_pSuccessRedSar->setVisible(false);
		point = ccp(0, g_iTaiJieHight+pDiamond->getContentSize().height/2);
		float time = fabs(pDiamond->getPositionY() - point.y)/s_fMoveSpeed;
		pDiamond->runAction(CCSequence::create(
			CCEaseBounceOut::create( CCMoveTo::create( time*1.5f, ccp( pDiamond->getPositionX(),  point.y ) )),
			CCCallFuncN::create(this, callfuncN_selector(BaseContentLayer::diamondMoveDone)),
			NULL));
		m_iEffectLevel = getEffectLevel(fabs(pDiamond->getPositionY() - point.y));
		m_bPlayEffect = true;
		m_iPlayEffectTimes = 0;
		m_fDiamondPreviousPosition = pDiamond->getPosition();
		this->schedule(schedule_selector(BaseContentLayer::checkDiamondPos));

		m_pSuccessRedSar   = NULL;
		m_pSuccessPlatform = NULL;
	}
	
}

int BaseContentLayer::getEffectLevel(float dis)
{
	int ret = 1;
	int max = 400;
	int de = 100;
	if (dis >= max)
	{
		ret = 5;
	}
	else if (dis >= max-de && dis < max)
	{
		ret = 4;
	}
	else if (dis >= max-2*de && dis < max-de)
	{
		ret = 3;
	}
	else if (dis >= max-3*de && dis < max-2*de)
	{
		ret = 2;
	}

	return 3;
}

void BaseContentLayer::checkDiamondPos(float dt)
{
	if (CCSprite* pDiamond = (CCSprite*)mScense->getBaseContentWrapLayer()->getChildByTag(diamond_tag))
	{
		if (pDiamond->getPositionY() > m_fDiamondPreviousPosition.y && m_bPlayEffect)
		{
			m_bPlayEffect = false;
			++m_iPlayEffectTimes;
//			Music::sharedMusicPlayer()->stopAllEffect();
			Music::sharedMusicPlayer()->playEffectSound(CCString::createWithFormat("ding%d.wav", m_iEffectLevel)->getCString());
		}
		else if (pDiamond->getPositionY() < m_fDiamondPreviousPosition.y && !m_bPlayEffect)
		{
			m_bPlayEffect = true;
		}
		m_fDiamondPreviousPosition = pDiamond->getPosition();
	}
}

void BaseContentLayer::diamondMoveDone(CCNode* pDiamond)
{
	if (m_fDiamondPreviousPosition.y - pDiamond->getPositionY() >= 2.0f || m_iPlayEffectTimes == 0)
	{	
		Music::sharedMusicPlayer()->playEffectSound(CCString::createWithFormat("ding%d.wav", m_iEffectLevel)->getCString());
	}
	this->unschedule(schedule_selector(BaseContentLayer::checkDiamondPos));
	int iCount = getChildrenCount();
	CCArray* pChildren = getChildren();
	for (int i = 0; i < iCount; i++)
	{
		CCNode* Node = (CCNode*)pChildren->objectAtIndex(i);
		Node->stopAllActions();
		Node->runAction(CCSequence::create(
			CCFadeTo::create(0.2f, 0.0f), 
			CCHide::create(),
			NULL));
	}

	//砖石不用渐隐
	pDiamond->stopAllActions();
	pDiamond->setVisible(true);

	pDiamond->runAction(CCSequence::create(
		CCDelayTime::create(0.3f),
		CCCallFuncN::create(this, callfuncN_selector(BaseContentLayer::runToDiamond)),
		NULL));

	//齿轮是加在BackgroudCoverLaye上的，所以隐藏掉该层就行
	mScense->getBackgroudCoverLayer()->runAction(CCSequence::create(
		 			CCFadeOut::create(0.2f),
		 			CCHide::create(),
		 			NULL));
}

void BaseContentLayer::runToDiamond(CCNode* pNode)
{
	CCPoint point = m_pArmature->getPosition();
	if (m_curLevelinfo->iRole_in == Role_Pos_Right)
	{
		m_pArmature->getAnimation()->play(g_ThiefAnimation_paozuo.c_str());
	}
	else
	{
		m_pArmature->getAnimation()->play(g_ThiefAnimation_paoyou.c_str());
	}


	float time = fabs(pNode->getPositionX() - point.x)/s_fMoveSpeed;
	m_pArmature->runAction(CCSequence::create(
		CCMoveTo::create(time, ccp(pNode->getPositionX(), m_pArmature->getPositionY())),
		CCCallFuncN::create(this, callfuncN_selector(BaseContentLayer::thiefMoveDone)),
		NULL));
}

void BaseContentLayer::thiefMoveDone(CCNode* pThief)
{
	CCArmature* pArmature = ((CCArmature*)pThief);
//	pArmature->getAnimation()->play(g_ThiefAnimation_jianqi.c_str());

	CCNode* node = this->getParent()->getParent()->getChildByTag(diamond_tag);
	if (node)
	{
		node->removeFromParent();
	}

	Music::sharedMusicPlayer()->playEffectSound("spirit_laugh.wav");

#if USE_JUMP_ARMATURE != 0
	pArmature = CCArmature::create("star_r1");
	pArmature->setPosition(pThief->getPosition());
	pThief->getParent()->addChild(pArmature);
	pArmature->getAnimation()->play("xiao2");
	pThief->setVisible(false);
#else
	pArmature->getAnimation()->play(g_ThiefAnimation_daxiao.c_str());
#endif

	pArmature->runAction(CCSequence::create(
		CCDelayTime::create(0.8f),
		CCCallFuncN::create(this, callfuncN_selector(BaseContentLayer::thiefLeaveOut)),
		NULL));
}

void BaseContentLayer::stopArmature()
{
	if (m_pArmature)
	{
		m_pArmature->getAnimation()->stop();
	}
}

void BaseContentLayer::playPaoArmature()
{
	if (m_curLevelinfo->iRole_in == Role_Pos_Right)
	{
		m_pArmature->getAnimation()->play(g_ThiefAnimation_paoyou.c_str());
	}
	else
	{
		m_pArmature->getAnimation()->play(g_ThiefAnimation_paozuo.c_str());
	}
}

void BaseContentLayer::thiefLeaveOut(CCNode* pThief)
{
//	m_pArmature->removeFromParent();
//	m_pArmature = NULL;

//	layerDown();

	showCongratulationLayer();
}

void BaseContentLayer::layerDown()
{
 	this->getParent()->getParent()->runAction(CCSequence::create(
		CCDelayTime::create(0.01f),
		CCMoveBy::create(0.3f, ccp(0, -g_iTaiJieHight)),
		CCCallFunc::create(this, callfunc_selector(BaseContentLayer::showCongratulationLayer)),
 		NULL));
}

void BaseContentLayer::showCongratulationLayer()
{
	endEffectiveEnd(NULL);
}

void BaseContentLayer::redStartEffectEnd(cocos2d::CCObject *effective)
{
    CCNode* _effectivie = (CCNode*)effective;
    
    _effectivie->getParent()->removeChild(_effectivie, true);
//    removeChild(_effectivie, true);
}

void BaseContentLayer::tableEffectiveEnd(CCObject *effectiveObj)
{
    CCNode* _effectivie = (CCNode*)effectiveObj;
    
    _effectivie->getParent()->removeChild(_effectivie, true);
}

void BaseContentLayer::addTableNick(CCNode* table)
{
//    CCSprite* nick = CCSprite::create(ResManager::getManager()->getSharedFilePath(g_gameinPath+"table_nick.png").c_str());
//    
//    CCSprite* nickHL = CCSprite::create(ResManager::getManager()->getSharedFilePath(g_gameinPath+"table_nick_HLight.png").c_str());
//    
//    CCSize nickSize = nick->getContentSize();
//    nick->addChild(nickHL);
//    nickHL->setPosition(ccp(nickSize.width/2,nickSize.height/2+1));
//    
//    CCDelayTime* delay = CCDelayTime::create(redStatDelayTime);
//    
//    CCFiniteTimeAction* fadeB = SpriteHelper::createFadeBetweenAction(255,0,redStarFadeTime,1);
//    
//    
//    CCAction* fade = SpriteHelper::createAction(fadeB,
//                                                SEL_CallFuncO(&BaseContentLayer::tableEffectiveEnd),
//                                                nickHL);
//    CCAction* action = CCSequence::create(delay,(CCFiniteTimeAction*)fade);
//    CCSize size = table->getContentSize();
//    
//    table->addChild(nick);
//    
//    nick->setPosition(ccp(size.width/2,size.height/2+1));
//    
//    nickHL->setOpacity(0);
//    nickHL->runAction(action);

}

void  BaseContentLayer::addTableHL(const char *fileName, cocos2d::CCNode *table)
{
    CCSize tableSize = table->getContentSize();
    //添加高光效果
    CCSprite* tableHL = CCSprite::create(ResManager::getManager()->getSharedFilePath(g_gameinPath+fileName).c_str());
    tableHL->setVisible(false);
    table->addChild(tableHL,1,TABLE_HL_TAG);
    tableHL->setPosition(ccp(tableSize.width/2,tableSize.height/2));
    
    //测试高亮
    {//Test
//        CCDelayTime* delay = CCDelayTime::create(1.0f);
//        CCFiniteTimeAction* fadeBetween = SpriteHelper::createFadeBetweenAction(255,0,0.5f, 1);
//        CCFiniteTimeAction* delayToFade = CCSequence::create(delay,fadeBetween,NULL);
//        
//        CCRepeatForever* re = CCRepeatForever::create((CCActionInterval*)delayToFade);
//        
//        tableHL->setVisible(true);
//        tableHL->setOpacity(0);
//        tableHL->runAction(re);
    }

}

void BaseContentLayer::addTableFlowLight(cocos2d::CCNode *table)
{
	//if (getMachinePerformanceLevel() > 0)
	{
#if 0
		CCSprite* flowLight = CCSprite::create(ResManager::getManager()->getSharedFilePath(g_gameinPath+"flowlight.png").c_str());
		CCSize flowLightSize = flowLight->getContentSize();
		CCSize tableSize = table->getContentSize();

		table->addChild(flowLight, 0, TABLE_FL_TAG);
		flowLight->setPosition(ccp(0,tableSize.height/2));
#endif
		runTableflowLightAction((CCSprite*)table, NULL);
	}
}
void BaseContentLayer::flowLightEnd(cocos2d::CCObject *light)
{
    CCNode* flowLight = (CCNode*)light;
    flowLight->stopAllActions();
    flowLight->setVisible(false);
}
void BaseContentLayer::addRedStarFlowLight(cocos2d::CCNode *redStar)
{
	//if (getMachinePerformanceLevel() > 0)
	{
		CustomeSprite* flowLight = CustomeSprite::create(ResManager::getManager()->getSharedFilePath(g_gameinPath+"flowlight.png").c_str());
		CustomeSprite* redStarMask = CustomeSprite::create(ResManager::getManager()->getSharedFilePath(g_gameinPath+"star60.png").c_str());
		flowLight->setPosition(ccp(0,39));//放在左上角
		flowLight->setVisible(true);
    
		CCSize redStarSize = redStar->getContentSize();
    
		redStarMask->setPosition(ccp(redStarSize.width/2,redStarSize.height/2));//重合
    
		redStar->addChild(redStarMask,0,REDSTAR_MASK_TAG);
    
		redStarMask->addChild(flowLight,0,REDSTAR_FL_TAG);
		redStarMask->setOpacity(0);
    
		float angle = getRedStarFlowLightRotateAngel(redStar);
		redStarMask->setRotation(-angle);
		runRedStarFlowLightAction(redStar);
	}
}
void BaseContentLayer::runRedStarFlowLightAction(cocos2d::CCNode *redStar)
{
    CCSprite* flowLight = (CCSprite*)redStar->getChildByTag(REDSTAR_MASK_TAG)->getChildByTag(REDSTAR_FL_TAG);
    flowLight->setPosition(ccp(0,39));//放在左上角
    flowLight->setVisible(true);
    
//    CCSprite* flowLight = CCSprite::create(ResManager::getManager()->getSharedFilePath(g_gameinPath+"flowlight.png").c_str());
//    flowLight->setPosition(ccp(0,39));//放在左上角
//    flowLight->setVisible(true);
//    redStar->addChild(flowLight);
    
    CCDelayTime* delay = CCDelayTime::create(redStatDelayTime);
    
    CCMoveTo* moveTo2 = CCMoveTo::create(0.7f, ccp(18,46));//移动到第2个点
    CCMoveTo* moveTo3 = CCMoveTo::create(0.7f, ccp(28,60));//移动到第3个点，最上面
//    CCMoveTo* moveTo4 = CCMoveTo::create(0.4f, ccp(37,46));//移动到第4个点
//    CCMoveTo* moveTo5 = CCMoveTo::create(0.4f, ccp(53,39));//移动到第4个点
    
    
    CCFiniteTimeAction* blinkB = CCSequence::create(CCFadeTo::create(0.3f, 255),CCDelayTime::create(0.8f),
                                                    CCFadeTo::create(0.3f, 0),NULL);
    
    CCFiniteTimeAction* move = CCSequence::create(moveTo2,moveTo3,NULL);
    
    CCSpawn* moveAndFade = CCSpawn::create(move, blinkB,NULL);
    
    
    CCAction* action = SpriteHelper::createAction(CCSequence::create(delay,moveAndFade,NULL),
                                                  SEL_CallFuncO(&BaseContentLayer::flowLightEnd), flowLight) ;
    
    flowLight->runAction(action);
    flowLight->setOpacity(0);

}

float BaseContentLayer::getRedStarFlowLightRotateAngel(cocos2d::CCNode *currentRedStar)
{
    float avalibelAngel[] = {0.0f,72.0f,144.0f,216.0f,288.0f,360.0f};
    int minIndex = 0;
    float currentAngle = currentRedStar->getRotation();
    
    //取余
    while (currentAngle>360.0f) {
        currentAngle -= 360.0f;
    }
    
    for (int i=0; i<sizeof(avalibelAngel)/sizeof(float); i++) {
        
        if ( abs(avalibelAngel[minIndex]-currentAngle) >  abs(avalibelAngel[i]-currentAngle) )
        {//有更小的值，那么就换 
            minIndex = i;
        }
        
    }
    
    return avalibelAngel[minIndex];
}

void BaseContentLayer::flowLightDoAction()
{
    
    for (size_t i=0; i<effectveTableVec.size(); i++) {
        
        CCNode* flowLight = effectveTableVec[i]->getChildByTag(TABLE_FL_TAG);
        
        runTableflowLightAction((CCSprite*)effectveTableVec[i], (CCSprite*)flowLight);
    }
    
    for (size_t i=0; i<effectveRedStarVec.size(); i++) {
        
        if (effectveRedStarVec[i] == NULL) {
            continue;
        }
        CCNode* redStarMask = effectveRedStarVec[i]->getChildByTag(REDSTAR_MASK_TAG);
        float angle = getRedStarFlowLightRotateAngel(effectveRedStarVec[i]);
        redStarMask->setRotation(-angle);
        runRedStarFlowLightAction(effectveRedStarVec[i]);
    }
    
}

void BaseContentLayer::runTableflowLightAction(CCSprite* table,CCSprite* flowLight)
{
#if 0
	CCSize tableSize = table->getContentSize();
	flowLight->setPosition(ccp(0,27));
    flowLight->setVisible(true);
    
    CCDelayTime* delay = CCDelayTime::create(redStatDelayTime);
    
    CCMoveBy* moveBy = CCMoveBy::create(1.4f, ccp(tableSize.width,0));
//    CCMoveTo* moveTo = CCMoveTo::create(1.4f, ccp(tableSize.width,tableSize.height/2));
    
    CCFiniteTimeAction* blinkB = CCSequence::create(CCFadeTo::create(0.3f, 255),CCDelayTime::create(0.8f),
                                                    CCFadeTo::create(0.3f, 0),NULL);
    
    CCSpawn* moveAndFade = CCSpawn::create(moveBy, blinkB,NULL);
    
    
    CCAction* action = SpriteHelper::createAction(CCSequence::create(delay,moveAndFade,NULL),
                                                  SEL_CallFuncO(&BaseContentLayer::flowLightEnd), flowLight) ;
    
    flowLight->runAction(action);
    flowLight->setOpacity(0);
#else
	CCArmature* pArmature = CCArmature::create("starfall");
	ccBlendFunc blend = {GL_SRC_ALPHA, GL_ONE};
	pArmature->setBlendFunc(blend);
	table->addChild(pArmature);
	CCSize size = table->getContentSize();
	pArmature->setPosition(ccp(size.width/2, size.height/2));
	pArmature->setOpacity(0xCC);
	CCArmatureAnimation* anim = pArmature->getAnimation();
	const char* animName = "light";
	CCMovementData* moveData = anim->getAnimationData()->getMovement(animName);
	CCMovementBoneData* boneData = moveData->getMovementBoneData(animName);
	//修改动画的起始帧（左右位置）为实际的平台左右位置
// 	float fact = (float)(size.width/2) / (float)(boneData->getFrameData(0)->x);
// 	if(fact < 1.0f)fact = 1.0f;
	boneData->getFrameData(0)->x = size.width/2 - 10;
	boneData->getFrameData(1)->x = size.width/4;
	boneData->getFrameData(2)->x = -size.width/4;
	boneData->getFrameData(3)->x = -size.width/2 + 10;
//	moveData->duration = (int)((float)moveData->duration * fact);
// 	for(int i = 0; i < 4; i++)
// 	{
// 		int frameID = boneData->getFrameData(i)->frameID;
// 		boneData->getFrameData(i)->frameID = (int)((float)frameID * fact);
// 	}
	anim->play(animName);
#endif
}

// 长时间没操作的回调
void BaseContentLayer::longTimeNoOpCallback(float t)
{
	m_bShowRanodomArmature = true;
	runNoOpAction();
}
// 执行长时间没操作的动画
void BaseContentLayer::runNoOpAction()
{
	if (m_pArmature && m_bShowRanodomArmature)
	{
		if (m_tempRandomArmatureVec.size() <= 0)
		{
			return;
		}
		int index = rand()%m_tempRandomArmatureVec.size();

		if (m_tempRandomArmatureVec.size() > 1)
		{
			while (true)
			{
				if (index != 0)
				{
					break;
				}
				index = rand()%m_tempRandomArmatureVec.size();
			}
		}

		m_pArmature->getAnimation()->play(m_tempRandomArmatureVec.at(index).c_str());
	}
}

// 停止长时间没操作的动画
void BaseContentLayer::stopNoOpAction()
{
    m_bShowRanodomArmature = false;
    unschedule(schedule_selector(BaseContentLayer::longTimeNoOpCallback));
	if (m_pArmature)
	{
//		m_pArmature->getAnimation()->stop();
	}
}

void BaseContentLayer::ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent)
{
	if (m_iCurPlayMode != playMode_Normal)
	{
		return;
	}

	if (!m_bAnimationOver)
	{
		if (time(NULL) - m_llLastTouchBeginTime < 2.0f)
		{
//			showGameImmediately();
		}
		m_llLastTouchBeginTime = time(NULL);
		return;
	}

	if (!mScense->getCollisionEnable())
	{
		return;
	}
	LFLayer::ccTouchesBegan(pTouches,pEvent);

    stopNoOpAction();

	if (mScense->getGuideLayer())
	{
		if (!mScense->getGuideLayer()->baseContentLayerTouchsBegin(pTouches,pEvent))
		{
			return;
		}
	}
    
    //2 point touches
    CCSetIterator begen = pTouches->begin();
	CCTouch* touch = (CCTouch*)(*begen);
    if(pTouches->count() >= 2){
        _pointA.x = touch->getLocation().x;
        _pointA.y = touch->getLocation().y;
        touch = (CCTouch*)(*(++begen));
        _pointB.x = touch->getLocation().x;
        _pointB.y = touch->getLocation().y;
        return;
    }else{
        if(touch->getID() == 0){
            _pointA.x = touch->getLocation().x;
            _pointA.y = touch->getLocation().y;
        }else if(touch->getID() == 1){
            _pointB.x = touch->getLocation().x;
            _pointB.y = touch->getLocation().y;
            return;
        }
    }

    isMoving = false;
    
    //重新计时开始，到时播放流光动画
    {
        unschedule(schedule_selector(BaseContentLayer::timeToFlowlightAction));
        schedule(schedule_selector(BaseContentLayer::timeToFlowlightAction),
                 TIME_FLOWLIGHT_ACTION+((float)TIME_FLOWLIGHT_ACTION)*CCRANDOM_0_1());
    }

	if (mLayerFreezed) {
		return;
	}
    
	mTouchStarPoint = touch->getLocation();
	mTouchStarPoint = convertToNodeSpace(mTouchStarPoint);
	// 锅炉点击开始
	{
		b2Body* temp = whichBodyContainThePoint(b2Vec2(mTouchStarPoint.x / PTM_RATIO , mTouchStarPoint.y / PTM_RATIO),true);
		if (temp && temp->GetUserData() != NULL && ((CCNode*)temp->GetUserData())->getTag() == Tag_Box_Boiler)
		{
			mIsLastClickedItem = true;

			mActiveBoiler = temp;

			Boiler* boiler = (Boiler*)mActiveBoiler->GetUserData();
			boiler->fire();

			recordEventID(boiler, eventID_BoilerStart);

			if (mScense->getGuideLayer())
				mScense->getGuideLayer()->baseContentLayerEffctiveClicked(boiler);
			{//记录有效点击的次数
				Runtime::getRuntime()->effectiveClick();
			}
			// 给成就传值
			{
				AchievementManager::getInstance()->effctiveClick(boiler,NULL);
			}
			//记录被使用的元素个数
			{
				if (!boiler->isUseedByUser())
					Runtime::getRuntime()->oneMoreItemUsed();	
				if (!boiler->isUseedByUser() && boiler->isKeyItem())
					Runtime::getRuntime()->oneMoreKeyItemUsed();				
			}
			boiler->setUseedByUser();
		}
		else
		{
			mIsLastClickedItem = false;
		}
	}

	if (mIsLastClickedItem)
	{
		scaleBack();
	}

#if SUPER_TOOL_TYPE == SUPER_TOOL_TYPE_LIKECUTROPE
	if (mUseSuperTool && mSuperTool)
	{
		mSuperTool->setBlowPos(b2Vec2(mTouchStarPoint.x / PTM_RATIO , mTouchStarPoint.y / PTM_RATIO));
		mSuperTool->start();
	}
#endif
}

float32 getExtensionValue(b2Body* currentBody);

void BaseContentLayer::ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent)
{
	if (!mScense->getCollisionEnable())
	{
		return;
	}

	LFLayer::ccTouchesMoved(pTouches,pEvent);

	if (mScense->getGuideLayer() && !mScense->getGuideLayer()->baseContentLayerTouchsMoved(pTouches,pEvent))
		return;

	if (mLayerFreezed)
		return;
    
    CCSetIterator begen = pTouches->begin();
	CCTouch* touch = (CCTouch*)(*begen);
    //2 points move
    if(pTouches->count() >= 2){
        if(mScense->getBaseContentWrapLayer()->isFixedScaling())
            return;
        CCPoint a = touch->getLocation();
        touch = (CCTouch*)(*(++begen));
        CCPoint b = touch->getLocation();
        float fara = sqrt((_pointB.x-_pointA.x)*(_pointB.x-_pointA.x)+(_pointB.y-_pointA.y)*(_pointB.y-_pointA.y));
        float farb = sqrt((b.x-a.x)*(b.x-a.x)+(b.y-a.y)*(b.y-a.y));

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
        CCPoint t = ccp(MIN(_pointA.x,_pointB.x)+fabs(_pointA.x-_pointB.x)/2.0f,
                        MIN(_pointA.y,_pointB.y)+fabs(_pointA.y-_pointB.y)/2.0f);
#else
		CCPoint t = ccp(fminf(_pointA.x,_pointB.x)+fabs(_pointA.x-_pointB.x)/2.0f,
			fminf(_pointA.y,_pointB.y)+fabs(_pointA.y-_pointB.y)/2.0f);
#endif
        if(fabs(fara - farb) < 30)
            return;
        bool sced = false;
        if(fara < farb){
            if(!mScense->getBaseContentWrapLayer()->haveFixedScaled()){
                lfDoubleClickDown(t);
                sced = true;
            }
        }else{
            if(mScense->getBaseContentWrapLayer()->haveFixedScaled()){
                lfDoubleClickDown(t);
                sced = true;
            }
        }
        if(sced){
            _pointA.x = a.x;
            _pointA.y = a.y;
            _pointB.x = b.x;
            _pointB.y = b.y;
        }
        return;
    }
    
	touch = (CCTouch*)(*begen);
    if(touch->getID() != 0 || pTouches->count() != 1){
        isMoving = false;
        return;
    }

	CCPoint location = touch->getLocation();
	location = convertToNodeSpace(location);

#if SUPER_TOOL_TYPE == SUPER_TOOL_TYPE_LIKECUTROPE
	if (mUseSuperTool && mSuperTool)
	{
		mSuperTool->setBlowPos(b2Vec2(location.x / PTM_RATIO , location.y / PTM_RATIO));
	}
#endif
	

	// 滑出锅炉的话，就停止点击
	{
		if (mActiveBoiler!=NULL)
		{
			bool isInBolier = LF::isPointInTheBody(b2Vec2(location.x/PTM_RATIO,location.y/PTM_RATIO),mActiveBoiler,getExtensionValue);
			if (!isInBolier)
			{
				Boiler* boiler = (Boiler*)mActiveBoiler->GetUserData();
				boiler->stop();
				recordEventID(boiler, eventID_BoilerEnd);
				mActiveBoiler = NULL;
			}
		}
	}

	if ( abs(location.x - mTouchStarPoint.x) > MAX_MOVE_DISTANCE_FOR_CLICK ||
		 abs(location.y - mTouchStarPoint.y) > MAX_MOVE_DISTANCE_FOR_CLICK
		)
	{
		isMoving = true;
	}

	//暂时修改绳子切割不灵敏的问题
	// 双击放大状态不能切绳子
    if (mToucheMovePointPrev.x >= 0 && !mScense->getBaseContentWrapLayer()->haveFixedScaled()) {
		bool isKeyItem = false;
		bool cut = false;
		std::map<float, CCPoint>::iterator it;
		std::map<float, CCPoint>::iterator it1;
		if (m_TouchMovedPointsMap.size() >= 2)
		{
			for (it = m_TouchMovedPointsMap.begin(); it != m_TouchMovedPointsMap.end(); ++it)
			{
				it1 = ++it;
				it--;
				if (it1 == m_TouchMovedPointsMap.end())
				{
					if (cutRopesWhenPointsThrough(it->second, location, isKeyItem))
					{
						cut = true;
					}
					break;
				}
				else
				{
					if (cutRopesWhenPointsThrough(it->second, it1->second, isKeyItem))
					{
						cut = true;
						break;
					}
				}
			}
		}
		else
		{
			if (cutRopesWhenPointsThrough(mToucheMovePointPrev, location, isKeyItem))
			{
				cut = true;
			}
		}
		
        //判断是否切割了绳子
       // bool cut = cutRopesWhenPointsThrough(ccp(mTouchStarPoint.x, mTouchStarPoint.y), location,isKeyItem);
        //记录有效点击
        if (cut)
        {
			scaleBack();
			if (mScense->getGuideLayer())
				mScense->getGuideLayer()->baseContentLayerEffctiveClicked(NULL);

            Runtime::getRuntime()->effectiveClick();
			// 给成就传值
			{
				AchievementManager::getInstance()->effctiveClick(NULL,NULL);
			}

			{
				Runtime::getRuntime()->oneMoreItemUsed();
				if (isKeyItem)
					Runtime::getRuntime()->oneMoreKeyItemUsed();
			}
        }
    }
    mToucheMovePointPrev = location;

	m_TouchMovedPointsMap.insert(std::pair<float, CCPoint>(m_fTickTotalTime, mToucheMovePointPrev));
}
void BaseContentLayer::ccTouchesEnded(CCSet* touches, CCEvent* event)
{
	if (!mScense->getCollisionEnable())
	{
		return;
	}

	LFLayer::ccTouchesEnded(touches,event);

	if (mScense->getGuideLayer() && !mScense->getGuideLayer()->baseContentLayerTouchsEnd(touches,event))
		return;

	bool needHandleBoxClick = true;
    //只要手指离开屏幕则锅炉都停止
    if (mActiveBoiler != NULL) {
        Boiler* boiler = (Boiler*)mActiveBoiler->GetUserData();
        boiler->stop();
		recordEventID(boiler, eventID_BoilerEnd);
		needHandleBoxClick = false;
    }
    
#if SUPER_TOOL_TYPE == SUPER_TOOL_TYPE_LIKECUTROPE
	if (mUseSuperTool && mSuperTool)
	{
		mSuperTool->stop();
	}
#endif
	
    mActiveBoiler = NULL;
    
    //只要手指离开屏幕则清除move
    mToucheMovePointPrev.x = -1;
    mToucheMovePointPrev.y = -1;

	mTouchStarPoint.x = -1;
	mTouchStarPoint.y = -1;
    
	if (isMoving)
	{
		return;
	}
    if (mLayerFreezed) {
        return;
    }
    
	{//记录点击次数：recored click num
		Runtime::getRuntime()->click();
	}

    // 重新计时
    stopNoOpAction();
    scheduleOnce(schedule_selector(BaseContentLayer::longTimeNoOpCallback),LONG_TIME);
    
	mIsLastClickedItem = false;
    

    CCSetIterator it;
	CCTouch* touch;
    for( it = touches->begin(); it != touches->end(); it++) 
	{
		touch = (CCTouch*)(*it);
        
		if(!touch)
			break;
		CCPoint location = touch->getLocation();
		location = convertToNodeSpace(location);
		location.x /= getScaleX();
		location.y /= getScaleY();

		b2Vec2 p(location.x/PTM_RATIO, location.y/PTM_RATIO); 
		b2Body* body = whichBodyContainThePoint(p,true);	

		//先看是否点击了闸门
		{
			if (body && body->GetUserData())
			{
				CustomeSprite* node =  (CustomeSprite*)body->GetUserData();//->getTag() == Tag_Box_Strobe

				Strobe* tempStrobe = NULL;
				if(node->getTag() == Tag_Box_Strobe)
					tempStrobe = (Strobe*)node;
				if (node->getTag() == Tag_Box_Strobe_Head)
					tempStrobe = (Strobe*)node->getData();
				
				if (tempStrobe)
				 {
					 mIsLastClickedItem = true;

					 tempStrobe->swicth();
					 if (mScense->getGuideLayer())
						mScense->getGuideLayer()->baseContentLayerEffctiveClicked(tempStrobe);
					 //记录有效点击
					 Runtime::getRuntime()->effectiveClick();
					 // 给成就传值
					 {
						 AchievementManager::getInstance()->effctiveClick(tempStrobe,NULL);
					 }
					 //记录被使用的元素个数
					 {
						 if (!tempStrobe->isUseedByUser())
							 Runtime::getRuntime()->oneMoreItemUsed();	
						 if (!tempStrobe->isUseedByUser() && tempStrobe->isKeyItem())
							 Runtime::getRuntime()->oneMoreKeyItemUsed();				
					 }
					 tempStrobe->setUseedByUser();	

					 //不在处理几何体的点击
					 needHandleBoxClick = false;
					 break;
				 }
			}
		}
        
		//先看是否点击了电磁炮
		{
			for (size_t i=0; i<emVec.size(); i++) {
				if (emVec[i]!=NULL && emVec[i]->pointInMe(location)) {

					mIsLastClickedItem = true;

					emVec[i]->swicth();
					if (mScense->getGuideLayer())
						mScense->getGuideLayer()->baseContentLayerEffctiveClicked(emVec[i]);
					//记录有效点击
					Runtime::getRuntime()->effectiveClick();
					// 给成就传值
					{
						AchievementManager::getInstance()->effctiveClick(emVec[i],NULL);
					}
					//记录被使用的元素个数
					{
						if (!emVec[i]->isUseedByUser())
							Runtime::getRuntime()->oneMoreItemUsed();	
						if (!emVec[i]->isUseedByUser() && emVec[i]->isKeyItem())
							Runtime::getRuntime()->oneMoreKeyItemUsed();				
					}
					emVec[i]->setUseedByUser();	


					//不在处理几何体的点击
					needHandleBoxClick = false;
					break;
				}
			}
		}
        
		if (!needHandleBoxClick) {
			break;
		}


		// 一般的几何体判断
		{
			if (body && body->GetUserData() != NULL)
			{
				CCNode* node = (CCNode*)body->GetUserData();
				int tag = node->getTag();
				
				//延后判断
				if (!mScense->getWaitingLayer()->isWaitingDone() && (
						tag == Tag_Box_can_Destroied || 
						tag == Tag_Box_Bomb ||//一般的炸弹
						tag == Tag_Box_Bomb_Hard ||//不可摔碎的炸弹
						tag == Tag_Box_Bomb_Cirecle ||
						tag == Tag_Box_Bomb_DarkWhole ||
						tag == Tag_Timer_Dec_Bomb ||
						tag == Tag_Timer_Asc_Bomb)

					) 
				{
					mScense->getWaitingLayer()->viewWaitingPrompt();
					return;
				}
			}
		}
        
		if(body != NULL){
			mIsLastClickedItem = true;
			scaleBack();

            CCNode* node = (CCNode*)body->GetUserData();
            //可以消除
            if (node->getTag() == Tag_Box_can_Destroied)
            {
				if (mScense->getGuideLayer())
					mScense->getGuideLayer()->baseContentLayerEffctiveClicked(node);

				{//记录有效点击的次数
					Runtime::getRuntime()->effectiveClick();
				}
				// 给成就传值
				{
					AchievementManager::getInstance()->effctiveClick(node,body);
				}
				//记录被使用的元素个数
				{
					CustomeSprite* item = (CustomeSprite*)node;
					if (!item->isUseedByUser())
						Runtime::getRuntime()->oneMoreItemUsed();	
					if (!item->isUseedByUser() && item->isKeyItem())
						Runtime::getRuntime()->oneMoreKeyItemUsed();	
					item->setUseedByUser();	
				}

                //删除几何体的转动锚点
                b2Body* anchor = checkAnchorWhenShapeDestoryed((CustomeSprite*)node);
                
                if (anchor!=NULL) {
                    
                    if(anchor->GetUserData() !=NULL)
                    {
                        ((CCNode*)anchor->GetUserData())->removeFromParentAndCleanup(true);
                    }
                    anchor->SetUserData(NULL);
                    mWorld->DestroyBody(anchor);
                }
                
                if (body->GetUserData() != NULL)
                {
                    CCNode* node = (CCNode*)body->GetUserData();
                    body->SetUserData(NULL);

					recordEventID(((CustomeSprite*)node), eventID_Touch);

					((CustomeSprite*)node)->fragmentBomb();

					((CustomeSprite*)node)->removeShadowIfPosible();
					 removeChild(node, true);
                }
                
                //检查绳索
                checkRopesWhenBodyDestroied(body,true);
                
                //销毁几何体必须放在销毁绳索后面，因为销毁body会自动销毁关节，而在绳索的析构函数中会去销毁关节
                mWorld->DestroyBody(body);
                                
                //显示计时器动画
				mScense->getWaitingLayer()->viewWaitingAnimation();
                
                return;
            }
            else if (node->getTag() == Tag_Box_Bomb ||//一般的炸弹
                      node->getTag() == Tag_Box_Bomb_Hard ||//不可摔碎的炸弹
                      node->getTag() == Tag_Box_Bomb_Cirecle)//圆形炸弹
            {
				if (mScense->getGuideLayer())
					mScense->getGuideLayer()->baseContentLayerEffctiveClicked(node);

                {//记录有效点击的次数
					Runtime::getRuntime()->effectiveClick();
				}
				// 给成就传值
				{
					AchievementManager::getInstance()->effctiveClick(node,body);
				}
				//记录被使用的元素个数
				{
					CustomeSprite* item = (CustomeSprite*)node;
					if (!item->isUseedByUser())
						Runtime::getRuntime()->oneMoreItemUsed();	
					if (!item->isUseedByUser() && item->isKeyItem())
						Runtime::getRuntime()->oneMoreKeyItemUsed();	
					item->setUseedByUser();	
				}

				recordEventID(((CustomeSprite*)node), eventID_Touch);

				//爆炸
				bombExplosion(body);

                //显示计时器动画
				mScense->getWaitingLayer()->viewWaitingAnimation();
            
                return;
            }
            else if (node->getTag() == Tag_Box_Bomb_DarkWhole)
            {
				if (mScense->getGuideLayer())
					mScense->getGuideLayer()->baseContentLayerEffctiveClicked(node);

                //记录有效点击
                Runtime::getRuntime()->effectiveClick();
				// 给成就传值
				{
					AchievementManager::getInstance()->effctiveClick(node,body);
				}
				//记录被使用的元素个数
				{
					CustomeSprite* item = (CustomeSprite*)node;
					if (!item->isUseedByUser())
						Runtime::getRuntime()->oneMoreItemUsed();	
					if (!item->isUseedByUser() && item->isKeyItem())
						Runtime::getRuntime()->oneMoreKeyItemUsed();	
					item->setUseedByUser();	
				}
                //检查绳索
                checkRopesWhenBodyDestroied(body,true);


				recordEventID((CustomeSprite*)node, eventID_TouchBomb_DarkWhole);
                DarkHoleBomb* dark = (DarkHoleBomb*)node;
                dark->bomb(mWorld);

                //显示计时器动画
				mScense->getWaitingLayer()->viewWaitingAnimation();
                
                return;
            }
            else if(node->getTag() == Tag_Timer_Dec_Bomb)
            {//如果是时间减速的炸弹
                
				Music::sharedMusicPlayer()->playEffectSound("timer_bomb_sound.wav");

				if (mScense->getGuideLayer())
					mScense->getGuideLayer()->baseContentLayerEffctiveClicked(node);

                {//记录有效点击的次数
					Runtime::getRuntime()->effectiveClick();
				}
				// 给成就传值
				{
					AchievementManager::getInstance()->effctiveClick(node,body);
				}
				//记录被使用的元素个数
				{
					CustomeSprite* item = (CustomeSprite*)node;
					if (!item->isUseedByUser())
						Runtime::getRuntime()->oneMoreItemUsed();	
					if (!item->isUseedByUser() && item->isKeyItem())
						Runtime::getRuntime()->oneMoreKeyItemUsed();	
					item->setUseedByUser();	
				}
				
                 TimerDec* timerDec = (TimerDec*)node->getChildByTag(Tag_Box_Frost);
				 timerDec->retain();
				 timerDec->removeFromParentAndCleanup(false);
				 addChild(timerDec, 2, timerDec->getTag());
				 timerDec->release();
				 timerDec->setPosition(node->getPosition());
                 timerDec->setVisible(true);
                 timerDec->action();
 
				 CCSequence* delay = CCSequence::create(CCDelayTime::create(Time_Dec_Gap), CCFadeOut::create(0.3f), NULL);

				 CCAction* action = SpriteHelper::createAction(delay,
					 SEL_CallFuncO(&BaseContentLayer::timerDecActionEnd),
					 timerDec);
                 
                timerDec->setScale(0.1f);
                timerDec->runAction(CCScaleTo::create(0.2f, 1.0f));
 				timerDec->runAction(CCFadeIn::create(0.2f));
                 
                timerDec->runAction(action);
               
                mTimerDecList.insert(mTimerDecList.end(), timerDec);

                recordEventID(((CustomeSprite*)node), eventID_Touch);
                //销毁减速装置
                {

					//检查绳索
					checkRopesWhenBodyDestroied(body,true);
					body->SetUserData(NULL);


                    ((CustomeSprite*)node)->removeShadowIfPosible();
					node->removeFromParent();
                    
                    mWorld->DestroyBody(body);
                }
                //显示计时器动画
				mScense->getWaitingLayer()->viewWaitingAnimation();
                
				break;
            }
            else if(node->getTag() == Tag_Timer_Asc_Bomb)
            {//如果是时间家速的炸弹
                
				if (mScense->getGuideLayer())
					mScense->getGuideLayer()->baseContentLayerEffctiveClicked(node);

                {//记录有效点击的次数
					Runtime::getRuntime()->effectiveClick();
				}
				// 给成就传值
				{
					AchievementManager::getInstance()->effctiveClick(node,body);
				}
				//记录被使用的元素个数
				{
					CustomeSprite* item = (CustomeSprite*)node;
					if (!item->isUseedByUser())
						Runtime::getRuntime()->oneMoreItemUsed();	
					if (!item->isUseedByUser() && item->isKeyItem())
						Runtime::getRuntime()->oneMoreKeyItemUsed();	
					item->setUseedByUser();	
				}
                
                timeAscing = true;
                
                CCSprite* effect = new CCSprite();
                effect->init();
                addChild(effect);
                effect->release();
                
                CCDelayTime* delay = CCDelayTime::create(Time_Dec_Gap);
                
                CCAction* action = SpriteHelper::createAction(delay,
                                                              SEL_CallFuncO(&BaseContentLayer::timerAscActionEnd),
                                                              effect);
                effect->runAction(action);
				recordEventID(((CustomeSprite*)node), eventID_Touch);
                //销毁加速装置
                {
                    //检查绳索
                    checkRopesWhenBodyDestroied(body,true);
                    body->SetUserData(NULL);
                    ((CustomeSprite*)node)->removeShadowIfPosible();
                    removeChild(node, true);
                    mWorld->DestroyBody(body);
                }
                
                //显示计时器动画
				mScense->getWaitingLayer()->viewWaitingAnimation();
                
				break;
            }
        }

		//add by jiangjie
		//修改马达的触摸优先级最低,当没有点击到可点击的元素时，才响应马达的点击事件
		// 马达的拾取
		{
			MotorSprite* temp = getClickedMotor(location);
			if (temp != NULL)
			{
				mIsLastClickedItem = true;

				temp->swith();                
				needHandleBoxClick = false;
				if (temp->handlable())
				{
					if (mScense->getGuideLayer())
						mScense->getGuideLayer()->baseContentLayerEffctiveClicked(temp);
					//记录有效点击
					Runtime::getRuntime()->effectiveClick();
					// 给成就传值
					{
						AchievementManager::getInstance()->effctiveClick(temp,NULL);
					}
					//记录被使用的元素个数
					{
						if (!temp->isUseedByUser())
							Runtime::getRuntime()->oneMoreItemUsed();	
						if (!temp->isUseedByUser() && temp->isKeyItem())
							Runtime::getRuntime()->oneMoreKeyItemUsed();				
					}
					temp->setUseedByUser();		
				}
			}
		}
		//end
	}
    
}

void BaseContentLayer::scaleBack()
{
	//if (mScense->getContentWrapper()->haveInitScaled())
	//{
	//	return;
	//}
	// 不自动缩小了
	//mScense->getContentWrapper()->setWrapperScaleWithFixPoint(1.0f,CCPointZero,true);
	//touchTrailLayer->setTouchEnabled(true); // 恢复显示割绳子的痕迹
}

void BaseContentLayer::postLfDoubleClickDown(CCPoint glPoint)
{
//	lfDoubleClickDown(glPoint);
}
void BaseContentLayer::lfDoubleClickDown(CCPoint glPoint)
{
	if (mLayerFreezed) {
		return;
	}

	if (mUseSuperTool)
	{
		return; // 超能道具，不准放大
	}
	

	if (mIsLastClickedItem)
	{
		return;
	}

	CCSprite* bg = ((BaseBackgroundLayer*)mScense->getBackgroudLayer())->getBackGroundPic();
	CCPoint posInBg = bg->convertToNodeSpace(glPoint);
	if (!mScense->getBaseContentWrapLayer()->haveFixedScaled())
	{
		if (posInBg.x < 0 || posInBg.y < 0 || posInBg.x > bg->getContentSize().width || posInBg.y > bg->getContentSize().height)
		{
			CCLog("Infor : because our bg file is 1024 * 768, current clicked pos in out of the bg file,ignore to scale big!");
			return;
		}
	}

	
	b2Body* temp = whichBodyContainThePoint(b2Vec2(mTouchStarPoint.x / PTM_RATIO , mTouchStarPoint.y / PTM_RATIO),true);
	if (temp && temp->GetUserData() != NULL)
	{
		return;
	}


	// 缩放
	const float scaleBig = 1.5f;
	if (mScense->getBaseContentWrapLayer()->haveFixedScaled())
	{
		touchTrailLayer->setTouchEnabled(true);
		mScense->getBaseContentWrapLayer()->setWrapperScaleWithFixPoint(1.0f,CCPointZero,true);
	}else
	{
		touchTrailLayer->setTouchEnabled(false); // 放大状态，不显示额绳子滑动的痕迹
		mScense->getBaseContentWrapLayer()->setWrapperScaleWithFixPoint(scaleBig,glPoint,true);
	}
}

MotorSprite* BaseContentLayer::getClickedMotor(CCPoint glPoint)
{
	MotorSprite* clicked = NULL;
	float distance_min = 1024;
	float current_dis ;
	//先看是否点击了马达，是的话就操纵马达
	for (size_t i=0; i<motorVec.size(); i++) {
		if (motorVec[i] != NULL && motorVec[i]->isClickMe(glPoint)) {
			current_dis = ccpDistance(glPoint,motorVec[i]->getPosition());
			if ( current_dis < distance_min )
			{
				distance_min = current_dis;
				clicked = motorVec[i];
			}
		}
	}
	return clicked;
}

float BaseContentLayer::minDistanceFromShape(b2Shape *shape, b2Vec2 point,b2Body* shapeBody)
{//只支持b2PolygonShape和b2CircleShape
    
    if (shape->GetType() == 2)//e_PolygonShape
    {//多边形
        b2PolygonShape* pShape = (b2PolygonShape*)shape;
        b2Vec2* vetexs = new b2Vec2[pShape->GetVertexCount()];
        for (int i=0; i<pShape->GetVertexCount(); i++) {
            
            vetexs[i] = shapeBody->GetWorldPoint(pShape->GetVertex(i));
            
//            vetexs[i] = pShape->GetVertex(i);
            
//            CCLog("vetexs[%d]=[%f,%f]",i,vetexs[i].x,vetexs[i].y);
        }
        float dis = minDistanceFromPolygon(vetexs, pShape->GetVertexCount(), point);
		delete vetexs;
        return dis;
    }
    else if(shape->GetType() == 0)//e_circle
    {//圆形
        b2CircleShape* cShape = (b2CircleShape*)shape;
       
        return distance(shapeBody->GetWorldCenter(), point)-cShape->m_radius;
    }
    
    return 9999999.9f;
}

float BaseContentLayer::minDistanceFromPolygon(b2Vec2 *vertex, int vertexCount, b2Vec2 point)
{
    float d = 99999999.9f;
    for (int i =0; i<vertexCount; i++)
    {
        float temp = minDistanceFromLineSegment(vertex[i], vertex[ (i+1)%vertexCount ], point);
        if (temp < d) {
            d = temp;
        }
    }
    return d;
}

float BaseContentLayer::distance(b2Vec2 p1, b2Vec2 p2)
{
    return sqrt( (p1.x-p2.x)*(p1.x-p2.x) + (p1.y-p2.y)*(p1.y-p2.y) );
}

float BaseContentLayer::minDistanceFromLineSegment(b2Vec2 a, b2Vec2 b, b2Vec2 p)
{
    b2Vec2 ab = b - a;
    
    b2Vec2 ac = p - a;
    
//    float f = ab * ac;
    float f = ab.x * ac.x + ab.y*ac.y;
    
    //钝角
    if (f<0) return distance(p, a);//C1处的点
    
//    float d = ab * ab;
    float d = ab.x*ab.x + ab.y+ab.y;//阴影长度
    
    if ( f>d) return distance(p, b);//C2处的点，d=f*cos（theta）
    
    f = f/d;
    
    b2Vec2 D = a + f *ab; // c在ab线段上的投影点
    
    return distance(p, D);
}
//在只销毁锚点的情况下调用，如果是锚点，那么就会去清理持有该锚点的Shape的锚点引用
//在清楚SHape时也可以调用，让他来清理这个shape对这个锚点的引用。
void BaseContentLayer::checkShapeWhenAnchorPointDestoryed(CustomeSprite *anchor)
{
    int tag = anchor->getTag();
    
    if (tag == Tag_Anchor_Mator)
    {
        MotorSprite* motor = (MotorSprite*)anchor;
        
        motor->setJoint(NULL);
        motor->unscheduleAllSelectors();

        motor->bundableShape->setData(NULL);//把持有anchor的Shape的锚点引用去掉

    }
    else if(tag == Tag_Anchor_SpringGear)
    {
        SpringGear* gear = (SpringGear*)anchor;
        gear->bunableBody = NULL;
//        gear->setWeldJoint(NULL);
        gear->setRotationJoint(NULL);
        gear->unscheduleAllSelectors();
        gear->bundableShape->setData(NULL);//把持有anchor的Shape的锚点引用去掉
    }
        
    else if(tag == Tag_Anchor_SpringSlide)
    {
        SpringSlideAnchor* slideAnchor = (SpringSlideAnchor*)anchor;
        slideAnchor->unscheduleAllSelectors();
        
        SpringSlide* slideCircle = (SpringSlide*)slideAnchor->bundableShape->getChildByTag(Tag_SpringSlide_Cricle);
        
        slideCircle->setPrismaticJoint(NULL);
        
        slideAnchor->bundableShape->removeAllChildrenWithCleanup(true);//移除shape上面的那个滚动标记
        
        slideAnchor->bundableShape->setData(NULL);//把持有anchor的Shape的锚点引用去掉
    }

}

//检查是否CustomeSprite带有转动，移动，滑动 锚点，有就把锚点元素保存的body清除，
//然后从需要这些锚点元素的数组中移除他们，返回的body可以用于从世界中删除。
//shapeItem->setData(sprite);把锚点的精灵传进入就行了，然后锚点精灵要保持这个锚点的body
b2Body* BaseContentLayer::checkAnchorWhenShapeDestoryed(CustomeSprite *shape)
{
    b2Body* anchor = NULL;
    
    CustomeSprite* shapeData = shape->getData();
    
    if (shapeData!=NULL)
    {//如果这个Shape右附带数据，那么就看是否带的是锚点
        
        checkShapeWhenAnchorPointDestoryed(shapeData);
        
        if (shapeData->getTag() == Tag_Anchor_SpringGear) {
            
            anchor = ((SpringGear*)shapeData)->getAnchorBody();
            
            ((SpringGear*)shapeData)->setAnchorBody(NULL);
            
        }else if(shapeData->getTag() == Tag_Anchor_Mator)
        {
            anchor = ((MotorSprite*)shapeData)->getAnchorBody();
            shapeMotorDestoried( (MotorSprite*)shapeData);
            ((MotorSprite*)shapeData)->setAnchorBody(NULL);
            
        }else if(shapeData->getTag() == Tag_Anchor_SpringSlide)
        {
            anchor = (shapeData)->getAnchorBody();
            (shapeData)->setAnchorBody(NULL);
        }
        
        shape->setData(NULL);
    }
    
    return  anchor;
}

void BaseContentLayer::timerAscActionEnd(cocos2d::CCObject *dec)
{
    CCSprite* t = (CCSprite*)dec;
    
    BaseContentLayer* layer = (BaseContentLayer*)t->getParent();
    
    t->removeFromParentAndCleanup(true);
    
    layer->timeAscing = false;

}

void BaseContentLayer::timerDecActionEnd(cocos2d::CCObject *dec)
{
    TimerDec* t = (TimerDec*)dec;
    
    if (t == NULL) {
        return;
    }
    
    BaseContentLayer* layer = (BaseContentLayer*)t->getParent();
    
    t->removeFromParentAndCleanup(true);
    if (layer == NULL) {
        return;
    }
    
    std::list<TimerDec*>::iterator timer;
    
    for (timer = layer->mTimerDecList.begin(); timer!=layer->mTimerDecList.end();) {
        if ( (*timer) == t ) {
            timer = layer->mTimerDecList.erase(timer);
            
            break;
        }else
        {
            timer++;
        }
    }


}

b2Body* BaseContentLayer::checkRopesWhenBodyDestroied(b2Body *body,bool destoryAnchorNow /* = false*/){
    std::vector<VRope*>::iterator pos = mRopes.begin();
    b2Body* anchor = NULL;
    for (; pos != mRopes.end(); ++pos) {
		if ((*pos)->checkRopeWhenBodyDestroied(body)) {
			anchor = (*pos)->getBodyA();

			if (destoryAnchorNow && anchor != NULL) {
				if ( (*pos)->getBodyA()!=NULL) {
					if ((*pos)->getBodyA()->GetUserData()!=NULL) {
						((CCNode*)anchor->GetUserData())->removeFromParentAndCleanup(true);
						anchor->SetUserData(NULL);
						(*pos)->setAnchorSprite(NULL);
					}
					mWorld->DestroyBody(anchor);
					(*pos)->setBodyA(NULL);
				}
				anchor = NULL;
			}
			delete (*pos);
			mRopes.erase(pos);
            break;
        }
    }  
    return anchor;
}

bool BaseContentLayer::cutRopesWhenPointsThrough(cocos2d::CCPoint pA, cocos2d::CCPoint pB,bool & isKeyItem){
    std::vector<VRope*> ropesToAdd;
    bool cut = false;


	b2Body* anchor = NULL;
    VRope* newRope = NULL;
    std::vector<VRope*>::iterator pos = mRopes.begin();
    for (; pos != mRopes.end(); ++pos) {
        if ((*pos)->cutRopeWithTowPoints(pA, pB, &newRope)) {

			anchor = (*pos)->getBodyA();

			(*pos)->setBodyA(NULL);
            //
            ropesToAdd.insert(ropesToAdd.end(), newRope);
            cut = true;
			isKeyItem = (*pos)->getAnchor()->isKeyItem();
			
            Music::sharedMusicPlayer()->playEffectSound("cut_rope_sound.wav");
        }
    }

	if (anchor!=NULL) {
		if(anchor->GetUserData() !=NULL)
		{
			((CCNode*)anchor->GetUserData())->removeFromParentAndCleanup(true);
		}
		anchor->SetUserData(NULL);
		mWorld->DestroyBody(anchor);
		anchor = NULL;
	}
    
//     for (int i=0; i<ropesToAdd.size(); ++i) {
//         //将新的一节绳子加进来
//         mRopes.insert(mRopes.end(), ropesToAdd[i]);
//     }
	addToRopes(ropesToAdd);
    
    return cut;
}

void BaseContentLayer::addToRopes(std::vector<VRope*> &vopes)
{
	for (size_t i=0; i<vopes.size(); ++i) {
		//将新的一节绳子加进来
		mRopes.insert(mRopes.end(), vopes[i]);
	}
	vopes.clear();
}

void BaseContentLayer::freezeLayer(){
	setLayerFreezy(true);
	unschedule(schedule_selector(BaseContentLayer::ThrowPointSchedule));
    unscheduleAllSelectors();
	if (redStar_StarPar !=NULL)
	{
		redStar_StarPar->stopSystem();
		redStar_StarPar->cleanup();
		redStar_StarPar->setVisible(false);
	}
	if (redStar_FirePar !=NULL)
	{
		redStar_FirePar->stopSystem();
		redStar_FirePar->cleanup();
		redStar_FirePar->setVisible(false);
	}
	//setWaitingSoundPause(true);
	mScense->getWaitingLayer()->setPause();
	setCCBladeVisible(false);
	setTouchEnabled(false);

	if (mActiveBoiler!=NULL) {
          Boiler* boiler = (Boiler*)mActiveBoiler->GetUserData();
          boiler->stop();
          mActiveBoiler = NULL;
      }

}

void BaseContentLayer::removeNode(CCNode* toRemove)
{
	if (toRemove)
		toRemove->removeFromParentAndCleanup(true);
}
void BaseContentLayer::setWaitingPause(bool pause)
{
	mScense->getWaitingLayer()->setPause(pause);
}
bool BaseContentLayer::isWatting()
{
	return !(mScense->getWaitingLayer()->isWaitingDone());
}

void BaseContentLayer::setToDestory(b2Body *toDestory)
{
    for (size_t i=0; i<toDestoryVec.size(); i++) {
        if (toDestory == toDestoryVec[i]) {
            return;
        }
    }
	//如果要被销毁的物体里面已经有了，就返回
	for (size_t i=0; i<mBombToExpolsionVec.size(); ++i) {
		if (mBombToExpolsionVec[i] == toDestory) {
			return;
		}
	}
    
    toDestoryVec.insert(toDestoryVec.end(), toDestory);
}

void BaseContentLayer::setBombToExplosion(b2Body *bomb){
    for (size_t i=0; i<mBombToExpolsionVec.size(); ++i) {
        if (mBombToExpolsionVec[i] == bomb) {
            return;
        }
    }
	//如果要被销毁的物体里面已经有了，就返回
	for (size_t i=0; i<toDestoryVec.size(); i++) {
		if (bomb == toDestoryVec[i]) {
			return;
		}
	}
    
    mBombToExpolsionVec.insert(mBombToExpolsionVec.end(), bomb);
}


void BaseContentLayer::setWind(WindProperty w)
{
    this->wind = w;
}

//点击到炸药
void BaseContentLayer::bombExplosion(b2Body* bombBody){
    
	//add by jiangjie	
	if (!BaseScense::getCurrentScene()->getCollisionEnable())
	{
		return;
	}
	//end

    //检查绳索
    checkRopesWhenBodyDestroied(bombBody,true);
    
    b2Vec2 bombP = bombBody->GetPosition();
    float32 bombGravity = bombBody->GetMass();

    CCNode* userData =(CCNode*) bombBody->GetUserData();
    
    std::list<b2Vec2> bompCenterPoints;
    if (userData != NULL && userData->getTag() == Tag_Box_Bomb_Cirecle) {
        
        b2CircleShape* circle = (b2CircleShape*)bombBody->GetFixtureList()->GetShape();
		b2Transform transform;//存储一个物体的位置和旋转角度的结构体
		transform.Set(bombBody->GetPosition(), bombBody->GetAngle());
		b2AABB aabb;//一个描述外围边框的结构体 bounding box
        
		//根据transform，计算出某个孩子的形状的。关联的边框坐标，存放在aabb中
		circle->ComputeAABB(&aabb, transform, 0);
        
        b2Vec2 origin = aabb.GetCenter();//即将受力的形状的位置;

        if (circle->m_radius == 0.5f) {
            bompCenterPoints.insert(bompCenterPoints.end(), origin);
            
        }else if(circle->m_radius == 1.0f)
        {
            b2Vec2 _temp(0.5,0.0);
            bompCenterPoints.insert(bompCenterPoints.end(), origin-_temp);
            bompCenterPoints.insert(bompCenterPoints.end(), origin+_temp);_temp.Set(0.0f, 0.5f);
            bompCenterPoints.insert(bompCenterPoints.end(), origin+_temp);
            bompCenterPoints.insert(bompCenterPoints.end(), origin-_temp);
        }else{
            
            CCAssert(false, "圆形炸弹半径不对！");
        }
        
    }else{
        //得到炸弹的小组中心点
        bompCenterPoints = getBodyFixtureCeneters(bombBody);
    }
    
	//炸弹的大小，有1,2,4的大小
	int bombSize = bompCenterPoints.size();
	bombGravity /= bombSize;//质量均分一下;

    mScense->bombed(bombSize, 4);
    //先销毁炸弹
    {
        if (bombBody->GetUserData() != NULL) {
            CCNode* node = (CCNode*)bombBody->GetUserData();
            bombBody->SetUserData(NULL);
            
            //爆炸效果！
            {
					int fixtureMax = 4;
//  				Bomb* bomb = BombCache::getCache()->getBomb();
//                  bomb->setPosition(node->getPosition());
//                  addChild(bomb,25);
//                  bomb->setScale((float)bombSize/(float)fixtureMax);
//  				bomb->bomb();

// 				CCSprite* pNode = CCSprite::create();
// 				pNode->setPosition(node->getPosition());
// 				pNode->setScale((float)bombSize/(float)fixtureMax);
// 				node->getParent()->addChild(pNode);
// 				pNode->runAction(CCSequence::create(
// 					AnimatePacker::getInstance()->getAnimate("fullbomb"),
// 					CCRemoveSelf::create(),
// 					NULL));


				CCArmature* pArmature = CCArmature::create("boom");
				pArmature->setPosition(node->getPosition());
				node->getParent()->addChild(pArmature);
				pArmature->getAnimation()->setMovementEventCallFunc(this, movementEvent_selector(BaseContentLayer::MovementEventCallFunc));
				pArmature->getAnimation()->play("boom");
				float scale = bombSize == fixtureMax?1.0f:((float)bombSize * 1.5f / 4.0f);
				pArmature->setScale(scale);
            }
            //
            ((CustomeSprite*)node)->removeShadowIfPosible();
			node->removeFromParent();
        }
        
        mWorld->DestroyBody(bombBody);
    }
    
    
    for (b2Body* b = mWorld->GetBodyList(); b != NULL; b = b->GetNext())
	{
        
        b2Fixture* fixture = b->GetFixtureList();//一个fixture带一个shape，多个fixture夹出一个物体
        
        //给每一个物体的形状分别受力
        for (; fixture!=NULL; fixture=fixture->GetNext()) 
		{
            b2Shape* shape = (b2Shape*)fixture->GetShape();
            
            b2Transform transform;//存储一个物体的位置和旋转角度的结构体
            transform.Set(b->GetPosition(), b->GetAngle());
            
            b2AABB aabb;//一个描述外围边框的结构体 bounding box

			//根据transform，计算出某个孩子的形状的。关联的边框坐标，存放在aabb中
			shape->ComputeAABB(&aabb, transform, 0);

            b2Vec2 shapeCenterP = aabb.GetCenter();//即将受力的形状的位置;
            
            float32 xDelta = shapeCenterP.x - bombP.x;
            float32 yDelta = shapeCenterP.y - bombP.y;
            float32 distance = sqrt(pow((xDelta),2)+pow((yDelta), 2));
            
			if (distance > sqrt((float)bombSize)*BOMB_MIN_EFFETIVE_DIS) {
				continue;
			}
			std::list<b2Vec2>::iterator it;
			b2Vec2 force = b2Vec2(0,0);
			
			//把炸弹拆开出力
			for (it=bompCenterPoints.begin(); it!= bompCenterPoints.end(); ++it)
			{//每一个小块爆炸都跟该物体施加一个力;
				b2Vec2 center = (b2Vec2)(*it);

				xDelta = shapeCenterP.x - center.x;
				yDelta = shapeCenterP.y - center.y;
				distance = sqrt(pow((xDelta),2)+pow((yDelta), 2));

				//计算与爆炸点之间是否有遮挡
				bool shielded = false;
				{
					float32 plus = (float32)BasicLength/(float32)PTM_RATIO/2;
					int testCount = distance/plus;
					float32 xPlus = plus * xDelta/distance;
					float32 yPlus = plus * yDelta/distance;
					b2Vec2 testP = shapeCenterP;
					for (int i=0; i<testCount; ++i) {
						testP.x -= xPlus;
						testP.y -= yPlus;
						b2Fixture* fixtureTmp = whichFixtureContainThePoint(testP);
						if (fixtureTmp != NULL) {

							if (fixtureTmp != fixture) {
								//fixture不是自己，表示被挡住了
								shielded = true;
								break;
							}
						}
					}
				}

				if (!shielded) {
					force += b2Vec2(bombGravity*BombExplosionPower*(xDelta/distance)/pow(distance, 2),
						bombGravity*BombExplosionPower*(yDelta/distance)/pow(distance,2));
				}
			}
            
            if (typeid(*shape) == typeid(b2CircleShape)) {
                
                float factor = ((b2CircleShape*)shape)->m_radius/0.5;
                
                force.x *= factor;
                force.y *= factor;
            }
            
			//CCLog("force : x = %f, y = %f",force.x,force.y);
			if (pow(force.x, 2)+pow(force.y, 2) >= 1600) 
			{
				b2Vec2 imp;
				imp.x = force.x * 1.0f/60.f;
				imp.y = force.y * 1.0f/60.f;
				b->ApplyLinearImpulse(imp, shapeCenterP);
			}
        }
	}
	bompCenterPoints.clear();

	playBombEffectBySize(bombSize);
}

void BaseContentLayer::playBombEffectBySize(int bombSize)
{
	//sound
	{
		string file = "bomb_explosion_sound_0_25.wav";
		switch (bombSize) {
		case 1:
			file = "bomb_explosion_sound_0_25.wav";
			break;
		case 2:
			file = "bomb_explosion_sound_0_5.wav";
			break;
		case 4:
			file = "bomb_explosion_sound_1_0.wav";
			break;
		default:
			CCAssert(false, "can not");
			break;
		}
		Music::sharedMusicPlayer()->playEffectSound(file);
	}
}

void BaseContentLayer::windBlowing()
{
    if (wind.enable) {
        
        for (b2Body* b = mWorld->GetBodyList(); b != NULL; b = b->GetNext()){
            
            b2Vec2 force = b2Vec2(cos(CC_DEGREES_TO_RADIANS(wind.angle))*wind.force,
                                  sin(CC_DEGREES_TO_RADIANS(wind.angle))*wind.force);
            
            b2Vec2 imp;
            imp.x = force.x * 1.0f/60.f;
            imp.y = force.y * 1.0f/60.f;
            b->ApplyLinearImpulse(imp, b->GetWorldCenter());
            
        }
    }
}

void BaseContentLayer::boilerApplyForce(b2Body *boiler, float applyTime){
    b2Vec2 forceCenter = boiler->GetPosition();
    for (b2Body* b = mWorld->GetBodyList(); b != NULL; b = b->GetNext())
	{
        if (b == boiler) {
            //跳过boiler自己
            continue;
        }
		CustomeSprite* sprite = (CustomeSprite*)b->GetUserData();
		if(sprite != NULL && sprite->getTag() == Tag_Boiler_Bubble)
		{
			continue;
		}
        
        b2Fixture* fixture = b->GetFixtureList();//一个fixture带一个shape，多个fixture夹出一个物体
        
        for (; fixture!=NULL; fixture=fixture->GetNext())
		{
            b2Shape* shape = (b2Shape*)fixture->GetShape();
            
            b2Transform transform;//存储一个物体的位置和旋转角度的结构体
            transform.Set(b->GetPosition(), b->GetAngle());
            
            b2AABB aabb;//一个描述外围边框的结构体 bounding box
            
			//根据transform，计算出某个孩子的形状的。关联的边框坐标，存放在aabb中
			shape->ComputeAABB(&aabb, transform, 0);
            
            b2Vec2 shapeCenterP = aabb.GetCenter();//即将受力的形状的位置;
            
            float32 xDelta = shapeCenterP.x - forceCenter.x;
            float32 yDelta = shapeCenterP.y - forceCenter.y;
            float32 distance = sqrt(pow((xDelta),2)+pow((yDelta), 2));
            
            
            
			if (distance > Boiler_MAX_EFFETIVE_DIS) {
				continue;
			}
        
			b2Vec2 force = b2Vec2(0,0);
            //计算与爆炸点之间是否有遮挡
            bool shielded = false;
            {
                float32 plus = (float32)BasicLength/(float32)PTM_RATIO/2;
                int testCount = distance/plus;
                float32 xPlus = plus * xDelta/distance;
                float32 yPlus = plus * yDelta/distance;
                b2Vec2 testP = shapeCenterP;
                for (int i=0; i<testCount; ++i) {
                    testP.x -= xPlus;
                    testP.y -= yPlus;
                    b2Fixture* fixtureTmp = whichFixtureContainThePoint(testP);
                    if (fixtureTmp != NULL) {
                        
                        shielded = true;
                        
                        b2Fixture* boilerFixture = boiler->GetFixtureList();
                        for (; boilerFixture!=NULL; boilerFixture=boilerFixture->GetNext()){
                            if (fixtureTmp == boilerFixture) {
                                //fixture是锅炉,不算挡住
                                shielded = false;
                                break;
                            }
                        }
                        
                        if (fixtureTmp == fixture) {
                            //fixture是自己，不算挡住
                            shielded = false;
                        }
                        
                        if (shielded) {
                            break;
                        }
                        
                    }
                }
            }
            
            if (!shielded) {
                //防止锅炉距离很近的时候力量太大
                static const float distanceFactorForBoiler = 2.0f;
                force = b2Vec2(BoilerForcePower*(xDelta/distance)/pow(distance+distanceFactorForBoiler, 2), BoilerForcePower*(yDelta/distance)/pow(distance+distanceFactorForBoiler,2));
            }
			
			if (pow(force.x, 2)+pow(force.y, 2) >= 1000)
			{
				b2Vec2 imp;
				imp.x = force.x * applyTime;
				imp.y = force.y * applyTime;
				b->ApplyLinearImpulse(imp, shapeCenterP);
			}
        }
	}
}

std::list<b2Vec2> BaseContentLayer::getBodyFixtureCeneters(b2Body* body)
{
	return LF::getBodyFixtureCeneters(body);
}


void BaseContentLayer::useSuperToolClicked(CCObject* alertNode)
{  
	mCreateSuperToolWhenScaleBack = true;
	// 停止可能有的缩放
    if (BaseContentWrapLayer* p = mScense->getBaseContentWrapLayer())
    {
        p->stopAllActions();
        // 恢复到正常大小
        p->setWrapperScaleWithFixPoint(1.0f,CCPointZero,0.2f); // 快速回收
    }
	// 
	CCNotificationCenter::sharedNotificationCenter()->removeObserver(this,MSG_USE_SUPERTOOL);

//#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || DEBUG_IOSMarket)
	CCSprite *item = (CCSprite*) getChildByTag(Tag_SpecialItem); //特殊关卡道具
	if(item) item->runAction(CCSequence::create(CCScaleTo::create(1.0f, 0), CCRemoveSelf::create(), NULL));
//#endif
}

void BaseContentLayer::fixScaleBackEnd(CCObject* wraplayer)
{
	if (mCreateSuperToolWhenScaleBack && mSafeBoarder == NULL )
	{
		openSuperTool();
	}
}

void BaseContentLayer::draw()
{
	// Default GL states: GL_TEXTURE_2D, GL_VERTEX_ARRAY, GL_COLOR_ARRAY, GL_TEXTURE_COORD_ARRAY
	// Needed states:  GL_VERTEX_ARRAY, 
	// Unneeded states: GL_TEXTURE_2D, GL_COLOR_ARRAY, GL_TEXTURE_COORD_ARRAY
	//glDisable(GL_TEXTURE_2D);
	//glDisableClientState(GL_COLOR_ARRAY);
	//glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	
	//world->DrawDebugData();
	
	// restore default GL states
	//glEnable(GL_TEXTURE_2D);
	//glEnableClientState(GL_COLOR_ARRAY);
	//glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    
    //Update rope sprites
	if (m_iPlayFrame <= 0)
	{
		for (size_t i = 0; i < mRopes.size(); i++) {
			mRopes[i]->updateSprites();
		}
	}
}

bool BaseContentLayer::isAnyFixtureInLine2(b2Vec2 startPoint, float angle,b2Vec2* point, std::vector<b2Fixture *> test,float testStep /* = 0.5 */)
{
    bool hit = false;

    vector<b2Vec2> interceptPoints;
    
    for (size_t i=0; i<test.size(); i++)
    {
        b2RayCastInput input;
        
        input.p1.Set(startPoint.x, startPoint.y);
        
        input.p2.Set(startPoint.x+cos(CC_DEGREES_TO_RADIANS(angle))*43,
                     startPoint.y+sin(CC_DEGREES_TO_RADIANS(angle))*43);
        
        input.maxFraction = 1.0f;
        
        b2RayCastOutput output;
        
        bool h = test[i]->RayCast(&output, input, 0);
        
        if (h)
        {
            hit = true;
            b2Vec2 hitPoint = input.p1 +output.fraction * (input.p2 - input.p1);
            interceptPoints.insert(interceptPoints.end(), hitPoint);
        }
    }
    
    //找到最近点，返回
    if (hit) {
        float minDistance = (interceptPoints[0].x-startPoint.x)*(interceptPoints[0].x-startPoint.x)+
                            (interceptPoints[0].y-startPoint.y)*(interceptPoints[0].y-startPoint.y);
        int minIndex = 0;
        for (size_t i=1; i<interceptPoints.size(); i++) {
            
            float temDistance = (interceptPoints[i].x-startPoint.x)*(interceptPoints[i].x-startPoint.x)+
                                (interceptPoints[i].y-startPoint.y)*(interceptPoints[i].y-startPoint.y);
            
            if (temDistance<minDistance) {
                
                minDistance = temDistance;
                
                minIndex = i;
            }
        }
        
        point->Set(interceptPoints[minIndex].x, interceptPoints[minIndex].y);
        return true;
    }
    
    return false;
}

bool BaseContentLayer::isAnyFixtureInLine2(b2Vec2 startPoint, b2Vec2 endPoint,b2Vec2* point, std::vector<b2Fixture *> test,float testStep /* = 0.5 */)
{
    for (size_t i=0; i<test.size(); i++)
    {
        
        b2RayCastInput input;
        
        input.p1.Set(startPoint.x, startPoint.y);
        
        input.p2.Set(endPoint.x,endPoint.y);
        
        input.maxFraction = 1.0f;
        
        b2RayCastOutput output;
        
        bool hit = test[i]->RayCast(&output, input, 0);
        
        if (hit)
        {
            b2Vec2 hitPoint = input.p1 +output.fraction * (input.p2 - input.p1);
            point->Set(hitPoint.x, hitPoint.y);
            return true;
        }
    }
    
    return false;
}


//
bool BaseContentLayer::isAnyFixtureInLine(b2Vec2 startPoint, b2Vec2 endPoint,std::vector<b2Fixture*> except,float testStep /* = 0.5 */)
{
    float32 xDelta = startPoint.x - endPoint.x;
    float32 yDelta = startPoint.y - endPoint.y;
    float32 distance = sqrt(pow((xDelta),2)+pow((yDelta), 2));

    {
        float32 plus = testStep;
        int testCount = distance/plus;
        float32 xPlus = plus * xDelta/distance;
        float32 yPlus = plus * yDelta/distance;
        b2Vec2 testP = startPoint;
        for (int i=0; i<testCount; ++i) {
            testP.x -= xPlus;
            testP.y -= yPlus;
            b2Fixture* fixtureTmp = whichFixtureContainThePoint(testP);
            if (fixtureTmp != NULL) {
                int temp = except.size();
                for (size_t i=0; i<except.size(); i++) {
                    if (fixtureTmp == except[i])
                    {//只要是除外的成员，那么就直接下一循环
                        break;
                    }
                    temp--;
                }
                //如果这个Fixture不是except的元素，那么就是右碰撞
                if (temp <=0 ) {
                    
                    return true;
                }
                
            }
        }
    }
    return false;
}

float32 getExtensionValue(b2Body* currentBody)
{
	return 1.0f;
}

bool ignoreMe(b2Body* currentBody)
{
	CCNode* node = (CCNode*)currentBody->GetUserData();
	//这3个不处理
	if (node->getTag() == Tag_Box_RedStar ||
		node->getTag() == Tag_Box_SpringBed ||
		node->getTag() == Tag_Box_cannot_Destroied||
		node->getTag() == Tag_Box_Table||
		node->getTag() == Tag_Box_Table_Ineffective)
	{
		return true;
	}

	return false;
}

b2Body* BaseContentLayer::whichBodyContainThePoint(b2Vec2 p,bool extension /* = false */)
{
	if (extension)
	{
		return LF::whichBodyContainThePoint(p,mWorld,getExtensionValue,ignoreMe);
	}
	else
	{
		return LF::whichBodyContainThePoint(p,mWorld,NULL,NULL);
	}
}

b2Fixture* BaseContentLayer::whichFixtureContainThePoint(b2Vec2 p,bool extension /* = false*/){
    
	if (extension)
	{
		return LF::whichFixtureContainThePoint(p,mWorld,getExtensionValue,ignoreMe);
	}else
	{
		return LF::whichFixtureContainThePoint(p,mWorld,NULL,NULL);
	}
}

void BaseContentLayer::springBedContactedBegin(b2Body *springBed, b2Body *contactObj)
{
//    CCSprite* bed = (CCSprite*)springBed->GetUserData();
//    
//    bed->initWithFile(ResManager::getManager()->getSharedFilePath(g_gameinPath+"spring_2.png").c_str());
}

void BaseContentLayer::springBedContactEnd(b2Body *springBed, b2Body *contactObj)
{
//    CCSprite* bed = (CCSprite*)springBed->GetUserData();
//    bed->initWithFile(ResManager::getManager()->getSharedFilePath(g_gameinPath+"spring_1.png").c_str());
}

void BaseContentLayer::springBedContact(b2Body *springBed, b2Body *contactObj, float power)
{
	CustomeSprite* temp = (CustomeSprite*)contactObj->GetUserData();
	if (temp == NULL || temp->getTag() == Tag_Box_Table || temp->getTag() == Tag_Box_Table_Ineffective)
	{
		return;
	}
	

	Music::sharedMusicPlayer()->playEffectSound("spring_bed_collision.wav");
    RecordSprite* bed = (RecordSprite*)springBed->GetUserData();
    
    bed->stopAllActions();
    
    CCFiniteTimeAction* frameAction = (CCFiniteTimeAction*)springBedAction->copy()->autorelease();
    
    CCAction* action = SpriteHelper::createAction(frameAction,
                                                  SEL_CallFuncO(&BaseContentLayer::springBedActionEnd),
                                                  bed);
    bed->runAction(action);

	recordEventID(bed, eventID_SpringBedAction);
}

void BaseContentLayer::springBedActionEnd(cocos2d::CCObject *bed)
{
    CCSprite* _bed = (CCSprite*)bed;
    _bed->stopAllActions();
}

void BaseContentLayer::redstarContactTable(b2Vec2 redstarPoint,b2Body* contactedTable)
{
    mRedstarContactTable = true;
    mPositionRedstarFromTable = redstarPoint - contactedTable->GetPosition();
    mRedstarContactTableTime = 0;
	this->contactedTable = contactedTable;

	AchievementManager::getInstance()->redStarContactTable((CCNode*)contactedTable->GetUserData());
    
    //unschedule(schedule_selector(BaseContentLayer::redstarContactTableTimeout));
    //schedule(schedule_selector(BaseContentLayer::redstarContactTableTimeout), RedstarStayOnTable_TimeToSuc);
}


void BaseContentLayer::redstarEndContactTable(){
    mRedstarContactTable = false;
    //unschedule(schedule_selector(BaseContentLayer::redstarContactTableTimeout));
}

void BaseContentLayer::tableBlockFollowCallBack(CCNode* block,void* data)
{

}

b2Body* BaseContentLayer::createTable(GameTable table){
    b2Body* body = NULL;
    
	float tableLength = 6*PTM_RATIO;

    switch (table.type) {
        case GameTableType_Table144:
			tableLength = 144;
            body = createTable144(table.point, table.move, table.angle, table.effective ,table.is_key_item);
            break;
        case GameTableType_Table192:
			tableLength = 192;
            body = createTable192(table.point, table.move, table.angle, table.effective ,table.is_key_item);
            break;
        case GameTableType_Table288:
			tableLength = 288;
            body = createTable288(table.point, table.move, table.angle, table.effective ,table.is_key_item);
            break;
        case GameTableType_Table336:
			tableLength = 336;
            body = createTable336(table.point, table.move, table.angle, table.effective ,table.is_key_item);
            break;
    }


	// 创建轨道
	{
		CustomeSprite* tableSprite = (CustomeSprite*)body->GetUserData();
		float length = tableLength;
		while (true)
		{
			if (!table.move.enable)
				break;
			// 轨道要相对台面往上偏移4个像素,是因为图片的问题
			CCPoint pic_offset = ccp(-4,4);
			pic_offset = ccp(-4,0);
			pic_offset = ccp( pic_offset.x * sin(CC_DEGREES_TO_RADIANS(table.angle)),pic_offset.y * cos(CC_DEGREES_TO_RADIANS(table.angle)) );
			CCPoint point1 = CCPointMake(table.move.moveStartPoint.x*PTM_RATIO, table.move.moveStartPoint.y*PTM_RATIO);
			CCPoint point2 = CCPointMake(table.move.moveBackPoint.x*PTM_RATIO, table.move.moveBackPoint.y*PTM_RATIO);
			CCPoint vectorP1ToP2 = ccpSub(point2,point1);
			if (vectorP1ToP2.x == 0.0f && vectorP1ToP2.y == 0.0f)
			{
				CCLog("LF------>waring! Table%d is moveable but the distance is 0!",(int)length);
				break;
			}
			CCPoint gradient = ccpNormalize(vectorP1ToP2); // 斜率
			CCPoint externtion = ccp(gradient.x * length/2.0f,gradient.y * length/2.0f);

			float trackAngle = CC_RADIANS_TO_DEGREES(ccpAngle(vectorP1ToP2,ccp(1,0)));
			if (vectorP1ToP2.y < 0) // 修正
				trackAngle = -trackAngle;

			float crossAngel = abs(trackAngle - table.angle);
			float scale = abs(cos(CC_DEGREES_TO_RADIANS(crossAngel)));
			externtion = ccp(externtion.x *scale , externtion.y * scale);
			
			// 两头都扩充一点，这里扩充24个像素
			externtion = ccpAdd(externtion,ccpMult(gradient,24.0f));

			// 如果台面的角度跟移动方向有交叉夹角，就需要2个轨道
			if ( abs(crossAngel) != 0 && abs(crossAngel) != 180.0f && abs(crossAngel) != 360)
			{
				float trackDistance = length*0.3f; // 2个轨道之间 分别距桌面中心的最大距离，即桌面和轨道垂直时为最大距离
				CCPoint track_offset = ccp(trackDistance * abs(cos(CC_DEGREES_TO_RADIANS(table.angle))),trackDistance* abs(sin(CC_DEGREES_TO_RADIANS(table.angle))));
				// 
#if USE_SCALE9SPRITE_TRACK == 0
				createTableTrack(
					track,
					ccpSub(point1,externtion),
					ccpAdd(point2,externtion),
					ccpSub(pic_offset,track_offset));

				createTableTrack(
					track,
					ccpSub(point1,externtion),
					ccpAdd(point2,externtion),
					ccpAdd(pic_offset,track_offset));
#else
				//使用9宫格图片替换,需要事先传入准确的位置
				createTableTrack(
					track,
					ccpSub(point1-track_offset,externtion),
					ccpAdd(point2-track_offset,externtion),
					ccpSub(pic_offset,track_offset));

				createTableTrack(
					track,
					ccpSub(point1+track_offset,externtion),
					ccpAdd(point2+track_offset,externtion),
					ccpAdd(pic_offset,track_offset));
#endif

				//// 卡子
				//CCSprite* table_block = CCSprite::create(ResManager::getManager()->getSharedFilePath(g_gameinPath+"table_no_track_part_board.png"));
				//BackgroudLayerFactory::getCurrentBgLayer()->addChild(table_block);
				//table_block->setRotation(trackAngle);

				//table_block->runAction(CCRepeatForever::create(CCCallFuncND::create(this,SEL_CallFuncND(&BaseContentLayer::tableBlockFollowCallBack),)));

				// 创建卡子
				TableBlock::createTableBlock(tableSprite,ccp(-trackDistance,0),ccp(trackDistance,0),point1,point2);
				
			}
			else
			{
				createTableTrack(
					track,
					ccpSub(point1,externtion),
					ccpAdd(point2,externtion),
					pic_offset);
			}

			break;
		}
	}
	lastCreateTablePos = ((CCNode*)body->GetUserData())->getPosition();
    ShadowLayer::sharedShadowLayer()->createShadow((CustomeSprite*)body->GetUserData());
    return body;
}

void BaseContentLayer::createThrowPoint(ThrowPoint throwPoint){
    ThrowPointData throwPointData;
    throwPointData.throwPoint = throwPoint;
    throwPointData.lastThrowTime = 0;
    
    mThrowPointDataVec.insert(mThrowPointDataVec.end(), throwPointData);

	RecordSprite* pFor = (RecordSprite*)this->getChildByTag(Tag_Forever_Rotate);
	if (!pFor)
	{
		pFor = new RecordSprite;
		pFor->autorelease();
		pFor->initWithFile(ResManager::getManager()->getFilePath(g_gameinPath+"fire.png").c_str());
		pFor->setVisible(false);
		pFor->setOpacity(0);
		pFor->setTag(Tag_Forever_Rotate);
		this->addChild(pFor);
		pFor->runAction(CCRepeatForever::create(CCRotateBy::create(0.1f, 360)));
		pFor->setPosition(ccp(-330, -400));
	}
}

void BaseContentLayer::begineThrowPointSchedule(float time)
{
    unschedule(schedule_selector(BaseContentLayer::begineThrowPointSchedule));
	unschedule(schedule_selector(BaseContentLayer::ThrowPointSchedule));
    schedule(schedule_selector(BaseContentLayer::ThrowPointSchedule), ThrowPoint_MinTimeGap);
}

void BaseContentLayer::endThrowPointSchedule()
{
	unschedule(schedule_selector(BaseContentLayer::begineThrowPointSchedule));
	unschedule(schedule_selector(BaseContentLayer::ThrowPointSchedule));
}

void BaseContentLayer::ThrowPointSchedule(ccTime dt){
    for (size_t i=0; i<mThrowPointDataVec.size(); ++i) {
        if (mThrowPointDataVec[i].lastThrowTime % mThrowPointDataVec[i].throwPoint.timeGap == 0) {
            
            //丢
            int shapeIndex = mThrowPointDataVec[i].lastThrowTime / mThrowPointDataVec[i].throwPoint.timeGap % mThrowPointDataVec[i].throwPoint.shapeVec.size() ;
            GameShape shape(mThrowPointDataVec[i].throwPoint.shapeVec[shapeIndex].shape.type, mThrowPointDataVec[i].throwPoint.point, mThrowPointDataVec[i].throwPoint.shapeVec[shapeIndex].shape.angle, mThrowPointDataVec[i].throwPoint.shapeVec[shapeIndex].shape.canDestroy, mThrowPointDataVec[i].throwPoint.shapeVec[shapeIndex].shape.restitution);
			shape.density = mThrowPointDataVec[i].throwPoint.shapeVec[shapeIndex].shape.density;
			shape.is_key_item = mThrowPointDataVec[i].throwPoint.shapeVec[shapeIndex].shape.is_key_item;
            
            b2Body* body = createShape(shape,true);
            if (body != NULL) {
                //CCLog("body=%p, force [%f, %f]", body, mThrowPointDataVec[i].throwPoint.shapeVec[shapeIndex].initForce.x, mThrowPointDataVec[i].throwPoint.shapeVec[shapeIndex].initForce.y);
                //body->ApplyForceToCenter(mThrowPointDataVec[i].throwPoint.shapeVec[shapeIndex].initForce);
                b2Vec2 imp = mThrowPointDataVec[i].throwPoint.shapeVec[shapeIndex].initForce;
				imp.x = imp.x *1.0f/60.0f;
				imp.y = imp.y *1.0f/60.0f;
				body->ApplyLinearImpulse(imp,body->GetWorldCenter());
				body->ApplyTorque(mThrowPointDataVec[i].throwPoint.shapeVec[shapeIndex].initTorque);
            }
        }
        
        mThrowPointDataVec[i].lastThrowTime++;
    }
}

void BaseContentLayer::shapeMotorDestoried(MotorSprite *motor)
{
    vector<MotorSprite*>::iterator it;
    for (it = motorVec.begin(); it!=motorVec.end(); it++) {
        if ((*it) == motor) {
            motorVec.erase(it);
            break;
        }
    }
    
    vector<GameTableMovementData>::iterator moveIt = mGameTableMovementDataVec.begin();
    for (; moveIt!=mGameTableMovementDataVec.end(); moveIt++) {
        
        if ( (*moveIt).body->GetUserData() == motor ) {
            mGameTableMovementDataVec.erase(moveIt);
            break;
        }
    }
    
    motor->setAnchorBody(NULL);
}

void BaseContentLayer::openSuperTool()
{
	if (mUseSuperTool)
	{
		return;
	}
	mUseSuperTool = true;
    BaseScense::getCurrentScene()->setCollisionEnable(true);
	if (mScense->getMenuLayer())
	{
		mScense->getMenuLayer()->setBtnvisible(false);
	}
	showSuperToolGuide();
	GameInforEditer::getGameInfor()->subSuperTool(1);
#if SUPER_TOOL_TYPE == SUPER_TOOL_TYPE_LIKECUTROPE
	mSafeBoarder = new SafeBoarder(mWorld,this);
	mSuperTool = new SuperTool(mWorld,this);
	mSuperTool->setBorders(mSafeBoarder->getBoarders());
#endif
	Runtime::getRuntime()->setUsedSuperTool(true);


#if SUPER_TOOL_TYPE == SUPER_TOOL_TYPE_NO_PHYSICS
	
#endif
}


const int iTag_SuperTool_String = 123423;

void BaseContentLayer::removeSuperTool()
{
	if (mSuperTool)
	{
		mSuperTool->clear();
		mSuperTool->remove();
	}
	this->removeChildByTag(iTag_SuperTool_String);
	CC_SAFE_DELETE(mSafeBoarder);
	CC_SAFE_DELETE(mSuperTool);
}


void BaseContentLayer::showSuperToolGuide()
{
	if (!CCUserDefault::sharedUserDefault()->getBoolForKey(Show_SuperTool_Guide, true))
	{
		return;
	}
	CCUserDefault::sharedUserDefault()->setBoolForKey(Show_SuperTool_Guide, false);
	CCUserDefault::sharedUserDefault()->flush();

	CCLabelTTF* pOne = BaseGuideLayer::createShadowLabel(LFStrings::getValue("teach_word_supertool_one").c_str(), fontStr_KlavikaBold);
	pOne->setAnchorPoint(ccp(0.5f, 0.5f));
	this->addChild(pOne, 10, iTag_SuperTool_String);
	pOne->setPosition(this->convertToNodeSpace(mScreenSize/2));

	float time = 5.0f;
	pOne->runAction(CCSequence::create(CCDelayTime::create(time), CCRemoveSelf::create(), NULL));
}


bool BaseContentLayer::isUsingSuperTool()
{
	return mUseSuperTool;
}

void BaseContentLayer::redStarDestoried(cocos2d::CCNode *redStar)
{
    mRedstarExist = false;
    
    //从效果队列中取出来
    for (size_t i=0; i < effectveRedStarVec.size(); i++) {
        if (effectveRedStarVec[i] == redStar) {
            effectveRedStarVec[i] = NULL;
        }
    }
    if (mFailedWhenRedStarFall) {
        schedule(schedule_selector(BaseContentLayer::failed),2);
    }
    
}

void BaseContentLayer::failed(float time)
{
    unscheduleAllSelectors();

    mScense->failed();
	mScense->getWaitingLayer()->setPause();
	//setWaitingSoundPause(true);
}

void BaseContentLayer::setFailedWhenRedStarFall(bool flag){
    mFailedWhenRedStarFall = flag;
}
void BaseContentLayer::setRedStarBeginEffevtive(bool flag)
{
    needRedStarBeginEff = flag;
}
b2Body* BaseContentLayer::createShape(GameShape shape,bool isThrowShape /*= false*/){
    b2Body* body = NULL;
    
    CustomeSprite* shapeItem = NULL;

    switch (shape.type) {
        case GameShapeType_Box48:
            body = createBox48(shape.point, shape.angle, shape.canDestroy, shape.density);
            break;
        case GameShapeType_Box24:
            body = createBox24(shape.point, shape.angle, shape.canDestroy, shape.density);
            break;
        case GameShapeType_Box48_24:
            body = createBox48_24(shape.point, shape.angle, shape.canDestroy, shape.density);
            break;
        case GameShapeType_Box72_48:
            body = createBox72_48(shape.point, shape.angle, shape.canDestroy, shape.density);
            break;
        case GameShapeType_Box96_48:
            body = createBox96_48(shape.point, shape.angle, shape.canDestroy, shape.density);
            break;
        case GameShapeType_Box168_24:
            body = createBox168_24(shape.point, shape.angle, shape.canDestroy, shape.density);
            break;
        case GameShapeType_Box192_24:
            body = createBox192_24(shape.point, shape.angle, shape.canDestroy, shape.density);
            break;
        case GameShapeType_Box216_24:
            body = createBox216_24(shape.point, shape.angle, shape.canDestroy, shape.density);
            break;
        case GameShapeType_Box240_24:
            body = createBox240_24(shape.point, shape.angle, shape.canDestroy, shape.density);
            break;
        case GameShapeType_Box264_24:
            body = createBox264_24(shape.point, shape.angle, shape.canDestroy, shape.density);
            break;
        case GameShapeType_Box360_24:
            body = createBox360_24(shape.point, shape.angle, shape.canDestroy, shape.density);
            break;
        case GameShapeType_Cross72:
            body = createCross72(shape.point, shape.angle, shape.canDestroy, shape.density);
            break;
        case GameShapeType_Cross48:
            body = createCross48(shape.point, shape.angle, shape.canDestroy, shape.density);
            break;
        case GameShapeType_Triangle56:
            body = createTriangle56(shape.point, shape.angle, shape.canDestroy, shape.density);
            break;
        case GameShapeType_RedStar60:
            body = createRedStar60(shape.point, shape.angle, shape.density,isThrowShape);
            break;
        case GameShapeType_GrayStar60:
            body = createGrayStar60(shape.point, shape.angle, shape.canDestroy, shape.density);
            break;
        case GameShapeType_Fork24:
            body = createFork24(shape.point, shape.angle, shape.canDestroy, shape.density);
            break;
        case GameShapeType_Bomb48:
            body = createBomb48(shape.point, shape.angle, shape.density);
            break;
        case GameShapeType_Bomb24:
            body = createBomb24(shape.point, shape.angle, shape.density);
            break;
        case GameShapeType_Bomb48_24:
            body = createBomb48_24(shape.point, shape.angle, shape.density);
            break;
        case GameShapeType_T72:
            body = createT72(shape.point, shape.angle, shape.canDestroy, shape.density);
            break;
        case GameShapeType_T48:
            body = createT48(shape.point, shape.angle, shape.canDestroy, shape.density);
            break;
        case GameShapeType_Box72_24:
            body = createBox72_24(shape.point, shape.angle, shape.canDestroy, shape.density);
            break;
        case GameShapeType_Box96_24:
            body = createBox96_24(shape.point, shape.angle, shape.canDestroy, shape.density);
            break;
        case GameShapeType_Box120_24:
            body = createBox120_24(shape.point, shape.angle, shape.canDestroy, shape.density);
            break;
        case GameShapeType_L48:
            body = createL48(shape.point, shape.angle, shape.canDestroy, shape.density);
            break;
        case GameShapeType_L72_48:
            body = createL72_48(shape.point, shape.angle, shape.canDestroy, shape.density);
            break;
        case GameShapeType_L72_72:
            body = createL72_72(shape.point, shape.angle, shape.canDestroy, shape.density);
            break;
        case GameShapeType_Circle48:
            body = createCircle48(shape.point, shape.canDestroy, shape.restitution, shape.density);
            break;
        case GameShapeType_Circle24:
            body = createCircle24(shape.point, shape.canDestroy, shape.restitution, shape.density);
            break;
        case GameShapeType_Circle72:
            body = createCircle72(shape.point, shape.canDestroy, shape.restitution, shape.density);
            break;
        case GameShapeType_HalfCircle96:
            body = createHalfCircle96(shape.point, shape.canDestroy, shape.density);
            break;
        case GameShapeType_HalfCircle48:
            body = createHalfCircle48(shape.point, shape.canDestroy, shape.density);
            break;
        case GameShapeType_LocalTimeDecBomb:
            body = createTimeDesBomb(shape.point, shape.angle, shape.density);
            break;
        case GameShapeType_LocalTimeDecBomb_24:
            body = createTimeDesBomb24(shape.point, shape.angle, shape.density);
            break;
        case GameShapeType_GlobalTimeAscBomb:
            body = createTimeAscBomb(shape.point, shape.angle, shape.density);
            break;
        case GameShapeType_Bomb24_Circle:
            body = createBomb24_Circle(shape.point, shape.angle, shape.density);
            break;
        case GameShapeType_Bomb48_Circle:
            body = createBomb48_Circle(shape.point, shape.angle, shape.density);
            break;
        case GameShapeType_Bomb24_Hard:
            body = createSquare24(shape.point, shape.angle, shape.density,"bomb24_no.png", Tag_Box_Bomb_Hard);
            break;
        case GameShapeType_Bomb48_Hard:
            body = createSquare48(shape.point, shape.angle, shape.density,"bomb48_no.png", Tag_Box_Bomb_Hard);
            break;
        case GameShapeType_Bomb24_DarkWhole://wormhole_bomb_24.png
            body = createDarkHole24(shape.point, shape.angle, shape.density);
            break;
        case GameShapeType_Bomb48_DarkWhole:
            body = createDarkHole48(shape.point, shape.angle, shape.density);
            break;
        default:
            CCLog("\n警告：未知的Shape类型！");
            break;
    }
    if (body == NULL) {
        return NULL;
    }
    shapeItem = (CustomeSprite*)body->GetUserData();
    
	shapeItem->setIsKeyItem(shape.is_key_item);
    


	if (shapeItem->getTag() == Tag_Box_can_Destroied)
	{
		AchievementManager::getInstance()->createdOneNormalShape();
	}

    //不能两个都开启
    if (shape.rotation.enable == true && shape.spring.enable == true) {
        CCAssert(false, "不能同时开启旋转和弹性属性");
    }
    //rotation motor
    if (shape.rotation.enable)
    {
		// 有马达，就换图片
		{
			string fileName = shapeItem->getFileName();
			fileName = SpriteHelper::replace_all_distinct(fileName,".png","_forMotor.png");
			shapeItem->initWithFile(fileName.c_str());
		}

        b2Body* anchorBody;
        if (shape.rotation.rotationType == GAME_ROTATION_Type_Free)
        {
			AchievementManager::getInstance()->createdOneMortoShape(shape.rotation.handleAble);
            b2Vec2 anchorPoint = shape.point + shape.rotation.anchorPosition;
            MotorSprite* sprite = NULL;
            //创建一个anchorPoint的body
            {
                sprite = MotorSprite::createMotor(shape.rotation.handleAble, m_curSeasonID, shape.rotation.move.enable);

                addChild(sprite,0,Tag_Anchor_Mator);
                sprite->setPosition(CCPointMake(anchorPoint.x*PTM_RATIO, anchorPoint.y*PTM_RATIO));
                sprite->setMotorSpeed(shape.rotation.motorSpeed);

				// 对小物体的背景此轮，做特殊处理
				if (shape.type == GameShapeType_Box24 || 
					shape.type == GameShapeType_Circle24 
					)
				{
					sprite->setBgGearScale(0.5f);
				}
                
                b2BodyDef bodyDef;
                bodyDef.type = b2_kinematicBody;
                bodyDef.position = anchorPoint;
                bodyDef.userData = sprite;
                anchorBody = mWorld->CreateBody(&bodyDef);
                
                b2PolygonShape box;
				//box.SetAsBox(0.5f, 0.5f);
				box.SetAsBox(0.1f, 0.1f); 
                
                b2FixtureDef fixtureDef;
                fixtureDef.shape = &box;
                fixtureDef.density = Fixture_Density;
                fixtureDef.friction = Fixture_Friction;
                fixtureDef.restitution = Fixture_Restitution;
                
                anchorBody->CreateFixture(&fixtureDef);
            }
            
            b2RevoluteJointDef jointDef;
            jointDef.Initialize(anchorBody, body, anchorBody->GetPosition());
            jointDef.enableMotor = true;
            jointDef.motorSpeed = shape.rotation.motorSpeed;
            jointDef.maxMotorTorque = shape.rotation.maxMotorTorque;
            
            b2Joint* joint = mWorld->CreateJoint(&jointDef);
            
            sprite->setJoint((b2RevoluteJoint*)joint);
            sprite->setAnchorBody(anchorBody);
            motorVec.insert(motorVec.end(), sprite);
            //把马达保存到几何体里面，然后消除几何体时也消除马达
            shapeItem->setData(sprite);
            
            sprite->bundableShape = shapeItem;
            
            if (shape.rotation.stopForBegin) {
                sprite->swith(true);
            }
        }
        else if (shape.rotation.rotationType == GAME_ROTATION_Type_Resilience)
        {
            b2Vec2 anchorPoint = shape.point + shape.rotation.anchorPosition;
            SpringGear* sprite = new SpringGear(shape.rotation.maxMotorTorque*15);
            addChild(sprite,0,Tag_Anchor_SpringGear);
            sprite->release();
            //创建一个anchorPoint的body
            {
                b2BodyDef bodyDef;
                bodyDef.type = b2_kinematicBody;
                bodyDef.position = anchorPoint;
                bodyDef.userData = sprite;
                anchorBody = mWorld->CreateBody(&bodyDef);
                
                b2PolygonShape box;
				box.SetAsBox(0.1f, 0.1f);
				//box.SetAsBox(0.5f, 0.5f);
                
                b2FixtureDef fixtureDef;
                fixtureDef.shape = &box;
                fixtureDef.density = Fixture_Density;
                fixtureDef.friction = Fixture_Friction;
                fixtureDef.restitution = Fixture_Restitution;
                
                anchorBody->CreateFixture(&fixtureDef);
            }
            
            b2RevoluteJointDef jointDef;
            jointDef.Initialize(anchorBody, body, anchorBody->GetPosition());
            jointDef.enableMotor = false;
            
            b2RevoluteJoint* joint = (b2RevoluteJoint*)mWorld->CreateJoint(&jointDef);

            sprite->setRotationJoint(joint);
            sprite->setAnchorBody(anchorBody);
            //把弹性马达保存到几何体里面，然后消除几何体时也消除马达
            shapeItem->setData(sprite);
            sprite->bundableShape = shapeItem;
            sprite->bunableBody = body;
        }
        else
        {
            CCAssert(false, "未知的旋转类型");
        }
        //将movement付给anchorbody
        if (shape.rotation.move.enable)
        {
            CCSprite* ghost = new CCSprite;
			ghost->autorelease();
            ghost->init();
            addChild(ghost);
			// LF 修改，解决起点和位置为不同点时的bug
			ghost->setPosition(CCPointMake(-shape.point.x*PTM_RATIO, shape.point.y*PTM_RATIO));
            //ghost->setPosition(CCPointMake(-shape.rotation.move.moveStartPoint.x*PTM_RATIO, shape.rotation.move.moveStartPoint.y*PTM_RATIO));
            CCMoveTo* move1 = CCMoveTo::create(shape.rotation.move.duration/2, CCPointMake(-shape.rotation.move.moveBackPoint.x*PTM_RATIO, shape.rotation.move.moveBackPoint.y*PTM_RATIO));
            CCMoveTo* move2 = CCMoveTo::create(shape.rotation.move.duration/2, CCPointMake(-shape.rotation.move.moveStartPoint.x*PTM_RATIO, shape.rotation.move.moveStartPoint.y*PTM_RATIO));
            CCEaseInOut* easy1 = CCEaseInOut::create(move1, 2);
            CCEaseInOut* easy2 = CCEaseInOut::create(move2, 2);
            CCSequence* seq = CCSequence::create(easy1, easy2,NULL);
            CCRepeatForever* repeat = CCRepeatForever::create(seq);
            ghost->runAction(repeat);
            
            GameTableMovementData moveData;
            moveData.movement = shape.rotation.move;
            moveData.body = anchorBody;
            moveData.ghost = ghost;
            moveData.ghostLastTickPosition = ghost->getPosition();
            
            mGameTableMovementDataVec.insert(mGameTableMovementDataVec.end(), moveData);
        }

        
    }
    else if(shape.spring.enable)
    {
        b2Body* anchorBody;
        b2Vec2 anchorPoint = shape.point + shape.spring.anchorPosition;
        float worldAngle = shape.spring.angle+shape.angle;
        
        //就是滑轨上面的那个小圈。用于秒速。
        SpringSlide* springSlide = new SpringSlide(shape.spring.elasticForce*10);
		springSlide->autorelease();
        if (body!=NULL && body->GetUserData()!=NULL) {
            
            CCNode* node = ((CCNode*)body->GetUserData());
            node->addChild(springSlide,0,Tag_SpringSlide_Cricle);
            
            springSlide->setPosition(ccp(shape.spring.anchorPosition.x * PTM_RATIO+node->getContentSize().width/2,
                                         node->getContentSize().height/2+shape.spring.anchorPosition.y * PTM_RATIO));
            
            springSlide->setRotation(-shape.spring.angle);
        }
        
        
        SpringSlideAnchor* sprite = NULL;
        //创建一个anchorPoint的body
        {
            sprite = new SpringSlideAnchor();
			sprite->autorelease();
            float angle = worldAngle-90.0f;
            sprite->initWithFile(ResManager::getManager()->getSharedFilePath(g_gameinPath+"elasticity_slide.png").c_str());
            addChild(sprite,-1,Tag_Anchor_SpringSlide);
            sprite->setPosition(CCPointMake(anchorPoint.x*PTM_RATIO, anchorPoint.y*PTM_RATIO));
            sprite->setRotation(angle);
            
            float width = 1024/(float)PTM_RATIO;
            float height = 768/(float)PTM_RATIO;
            
            b2BodyDef bodyDef;
            bodyDef.type = b2_kinematicBody;
            //放到远处
            bodyDef.position = b2Vec2(anchorPoint.x-width*cos(CC_DEGREES_TO_RADIANS(angle)),
                                      anchorPoint.y-height*sin(CC_DEGREES_TO_RADIANS(angle)));
//            bodyDef.position = anchorPoint;
            bodyDef.userData = sprite;
            anchorBody = mWorld->CreateBody(&bodyDef);
            anchorBody->SetTransform(anchorBody->GetPosition(), CC_DEGREES_TO_RADIANS(angle));
            
            
            b2PolygonShape box;
            box.SetAsBox(0.1f, 0.1f);
            
            b2FixtureDef fixtureDef;
            fixtureDef.shape = &box;
            fixtureDef.density = Fixture_Density;
            fixtureDef.friction = Fixture_Friction;
            fixtureDef.restitution = Fixture_Restitution;
            
            anchorBody->CreateFixture(&fixtureDef);
            
            sprite->setAnchorBody(anchorBody);
            shapeItem->setData(sprite);
            
            sprite->bundableShape = shapeItem;
        }

        b2PrismaticJointDef jointDef2;

        b2Vec2 worldAxis(cos( CC_DEGREES_TO_RADIANS(worldAngle)),sin(CC_DEGREES_TO_RADIANS(worldAngle)));
        jointDef2.Initialize(anchorBody, body, anchorBody->GetWorldCenter(),worldAxis);
        jointDef2.lowerTranslation = -shape.spring.maxDistacne;
        jointDef2.upperTranslation = shape.spring.maxDistacne;
        jointDef2.enableLimit = true;
        jointDef2.maxMotorForce = 0.0f;
        jointDef2.motorSpeed = 0.0f;
        jointDef2.enableMotor = true;
        
        b2PrismaticJoint* j = (b2PrismaticJoint*)mWorld->CreateJoint(&jointDef2);
        
        springSlide->setPrismaticJoint(j);
        
        springSlide->bundableBody = body;
    }
    
	//创建阴影
	{
		ShadowLayer::sharedShadowLayer()->createShadow(shapeItem);
	}


	// 创建轨道
	{
		float length = 50;
		while (true)
		{
			if (!shape.rotation.enable)
				break;
			// 轨道要相对台面往上偏移4个像素,是因为图片的问题
			CCPoint pic_offset = ccp(0,0);
			//pic_offset = ccp( pic_offset.x * sin(CC_DEGREES_TO_RADIANS(table.angle)),pic_offset.y * cos(CC_DEGREES_TO_RADIANS(table.angle)) );
			
			CCPoint point1 = CCPointMake(shape.rotation.move.moveStartPoint.x*PTM_RATIO, shape.rotation.move.moveStartPoint.y*PTM_RATIO);
			CCPoint point2 = CCPointMake(shape.rotation.move.moveBackPoint.x*PTM_RATIO, shape.rotation.move.moveBackPoint.y*PTM_RATIO);
			CCPoint anchorOffset = ccp(shape.rotation.anchorPosition.x * PTM_RATIO,shape.rotation.anchorPosition.y * PTM_RATIO);
			point1 = ccpAdd(point1,anchorOffset);
			point2 = ccpAdd(point2,anchorOffset);

			CCPoint vectorP1ToP2 = ccpSub(point2,point1);
			if ( (shape.rotation.move.enable && vectorP1ToP2.x == 0.0f && vectorP1ToP2.y == 0.0f) || !shape.rotation.move.enable)
			{
				string tempFileName = SpriteHelper::splicingNum_String("turn_anchor_S%d_track_circle.png", 1);//mScense->getCurrentSeasonId());
				CCSprite* temp = CCSprite::create(ResManager::getManager()->getSharedFilePath(g_gameinPath+tempFileName).c_str());
				//BackgroudLayerFactory::getCurrentBgLayer()->addChild(temp,0);
				mScense->getBackgroudCoverLayer()->addChild(temp,0);

				b2Vec2 anchorPoint = shape.point + shape.rotation.anchorPosition;
				temp->setPosition(ccp(anchorPoint.x * PTM_RATIO,anchorPoint.y * PTM_RATIO));

				// 对小物体的背景此轮，做特殊处理
				if (shape.type == GameShapeType_Box24 || 
					shape.type == GameShapeType_Circle24 
					)
				{
					temp->setScale(0.5f);
				}

				break;
			}
			CCPoint gradient = ccpNormalize(vectorP1ToP2); // 斜率
			CCPoint externtion = ccp(gradient.x * length/2.0f,gradient.y * length/2.0f);

			float trackAngle = CC_RADIANS_TO_DEGREES(ccpAngle(vectorP1ToP2,ccp(1,0)));
			if (vectorP1ToP2.y < 0) // 修正
				trackAngle = -trackAngle;

			createMotorTrack(
				motorTrackBatchNode,
				ccpSub(point1,externtion),
				ccpAdd(point2,externtion),
				pic_offset);

			break;
		}
	}

    return body;
}

b2Body* BaseContentLayer::createCross72(b2Vec2 p, float32 angle, bool canDestroy, float32 desity){
    if (canDestroy) {
        addBluredSpriteToMap(ResManager::getManager()->getSharedFilePath(g_gameinPath+"cross72.png"));
    }

    CustomeSprite* sprite = new CustomeSprite;
    
    if (canDestroy) {
        sprite->initWithFile(ResManager::getManager()->getSharedFilePath(g_gameinPath+"cross72.png").c_str());
        addChild(sprite, 0, Tag_Box_can_Destroied);
    }else {
        sprite->initWithFile(ResManager::getManager()->getSharedFilePath(g_gameinPath+"cross72_no.png").c_str());
        //sprite->setColor(ccGRAY);
        addChild(sprite, 0, Tag_Box_cannot_Destroied);
    }
    sprite->release();
    
    sprite->setPosition(CCPointMake(p.x*PTM_RATIO, p.y*PTM_RATIO));
	// cross_72创建碎片
	if(USE_MANUAL_FRAGMENT)
	{
		sprite->addFragment_ResMgr("box_frame_24.png",ccp(11,42),0);
		sprite->addFragment_ResMgr("box_frame_24.png",ccp(42,72),90);
		sprite->addFragment_ResMgr("box_frame_24.png",ccp(72,42),-180);
		sprite->addFragment_ResMgr("box_frame_24.png",ccp(42,11),-90);
		sprite->addFragment_ResMgr("cross72_spell_middle1.png",ccp(20,42),0);
		sprite->addFragment_ResMgr("cross72_spell_middle2.png",ccp(32,42),0);
		sprite->addFragment_ResMgr("cross72_spell_middle1.png",ccp(43,53),0);
		sprite->addFragment_ResMgr("cross72_spell_middle1.png",ccp(43,26),0);
		sprite->addFragment_ResMgr("cross72_spell_middle2.png",ccp(56,42),-180);
		sprite->addFragment_ResMgr("cross72_spell_middle1.png",ccp(65,42),0);
	}
	else
	{
		sprite->splitSpriteToFragment();
	}
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(p.x, p.y);
    bodyDef.userData = sprite;
    b2Body* bodyA = mWorld->CreateBody(&bodyDef);
    
    {
        b2PolygonShape box;
        std::vector<b2PolygonShape> boxVec;
        
        box.SetAsBox(0.5f, 0.5f, b2Vec2(0.0f, 0.0f), 0);
        boxVec.insert(boxVec.end(), box);
        box.SetAsBox(0.5f, 0.5f, b2Vec2(-1.0f, 0.0f), 0);
        boxVec.insert(boxVec.end(), box);
        box.SetAsBox(0.5f, 0.5f, b2Vec2(1.0f, 0.0f), 0);
        boxVec.insert(boxVec.end(), box);
        box.SetAsBox(0.5f, 0.5f, b2Vec2(0.0f, 1.0f), 0);
        boxVec.insert(boxVec.end(), box);
        box.SetAsBox(0.5f, 0.5f, b2Vec2(0.0f, -1.0f), 0);
        boxVec.insert(boxVec.end(), box);
        
        
        for (size_t i=0; i<boxVec.size(); ++i) {
            b2FixtureDef fixtureDef;
            fixtureDef.shape = &boxVec[i];	
            fixtureDef.density = desity;
            fixtureDef.friction = Fixture_Friction;
            fixtureDef.restitution = Fixture_Restitution;
            
            bodyA->CreateFixture(&fixtureDef);
        }
    }
    
    if (angle != 0.0f) {
        bodyA->SetTransform(bodyA->GetPosition(), CC_DEGREES_TO_RADIANS(angle));
    }
    
    
    return bodyA;
}
b2Body* BaseContentLayer::createCross48(b2Vec2 p, float32 angle, bool canDestroy, float32 desity){
    //先创建模糊后的纹理
    if (canDestroy) {
        addBluredSpriteToMap(ResManager::getManager()->getSharedFilePath(g_gameinPath+"cross48.png"));
    }
    
    
    
    CustomeSprite* sprite = new CustomeSprite;
    
    if (canDestroy) {
        sprite->initWithFile(ResManager::getManager()->getSharedFilePath(g_gameinPath+"cross48.png").c_str());
        addChild(sprite, 0, Tag_Box_can_Destroied);
    }else {
        sprite->initWithFile(ResManager::getManager()->getSharedFilePath(g_gameinPath+"cross48_no.png").c_str());
        //sprite->setColor(ccGRAY);
        addChild(sprite, 0, Tag_Box_cannot_Destroied);
    }
    sprite->release();
    
    sprite->setPosition(CCPointMake(p.x*PTM_RATIO, p.y*PTM_RATIO));
	// cross_48创建碎片
	if(USE_MANUAL_FRAGMENT)
	{
		sprite->addFragment_ResMgr("box_frame_24.png",ccp(10,30),0);
		sprite->addFragment_ResMgr("box_frame_24.png",ccp(30,50),90);
		sprite->addFragment_ResMgr("box_frame_24.png",ccp(50,30),180);
		sprite->addFragment_ResMgr("box_frame_24.png",ccp(30,10),-90);
		sprite->addFragment_ResMgr("cross48_spell_middle1.png",ccp(20,30),0);
		sprite->addFragment_ResMgr("cross48_spell_middle2.png",ccp(30,30),0);
		sprite->addFragment_ResMgr("cross48_spell_middle1.png",ccp(40,30),180);
	}
	else
	{
		sprite->splitSpriteToFragment();
	}
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(p.x, p.y);
    bodyDef.userData = sprite;
    b2Body* bodyA = mWorld->CreateBody(&bodyDef);
    
    b2Vec2 vecArr[][4] = {{b2Vec2(-1.0f, 0.5f), b2Vec2(-1.0f, -0.5f), b2Vec2(1.0f, -0.5f), b2Vec2(1.0f, 0.5f)}
        ,{b2Vec2(-0.5f, 1.0f), b2Vec2(-0.5f, 0.5f), b2Vec2(0.5f, 0.5f), b2Vec2(0.5f, 1.0f)}
        ,{b2Vec2(-0.5f, -0.5f), b2Vec2(-0.5f, -1.0f), b2Vec2(0.5f, -1.0f), b2Vec2(0.5f, -0.5f)}
    };
    
    for (int boxIndex=0; boxIndex<3; ++boxIndex) {
        b2PolygonShape box;
        box.Set(vecArr[boxIndex], 4);
        
        b2FixtureDef fixtureDef;
        fixtureDef.shape = &box;
        fixtureDef.density = desity;
        fixtureDef.friction = Fixture_Friction;
        fixtureDef.restitution = Fixture_Restitution;
        
        bodyA->CreateFixture(&fixtureDef);
    }
    
    if (angle != 0.0f) {
        bodyA->SetTransform(bodyA->GetPosition(), CC_DEGREES_TO_RADIANS(angle));
    }
    
    
    return bodyA;
}

b2Body* BaseContentLayer::createBox48(b2Vec2 p, float32 angle, bool canDestroy, float32 desity){
    //先创建模糊后的纹理
    if (canDestroy) {
        addBluredSpriteToMap(ResManager::getManager()->getSharedFilePath(g_gameinPath+"box48.png"));
    }
    CustomeSprite* sprite = new CustomeSprite;
    if (canDestroy) {
        sprite->initWithFile(ResManager::getManager()->getSharedFilePath(g_gameinPath+"box48.png").c_str());
        addChild(sprite, 0, Tag_Box_can_Destroied);
    }else {
        sprite->initWithFile(ResManager::getManager()->getSharedFilePath(g_gameinPath+"box48_no.png").c_str());
        //sprite->setColor(ccGRAY);
        addChild(sprite, 0, Tag_Box_cannot_Destroied);
    }sprite->release();
    
    sprite->setPosition(CCPointMake(p.x*PTM_RATIO, p.y*PTM_RATIO));

	// 创建碎片
	if(USE_MANUAL_FRAGMENT)
	{
		sprite->addFragment_ResMgr("box_frame_48.png",ccp(12,32),0);
		sprite->addFragment_ResMgr("box_frame_48.png",ccp(32,49),90);
		sprite->addFragment_ResMgr("box_frame_48.png",ccp(48,28),-180);
		sprite->addFragment_ResMgr("box_frame_48.png",ccp(27,9),-90);
		sprite->addFragment_ResMgr("box_middle_48.png",ccp(20,30),0);
		sprite->addFragment_ResMgr("box_middle_48.png",ccp(28,31),0);
		sprite->addFragment_ResMgr("box_middle_48.png",ccp(40,31),0);
	}
	else
	{
		sprite->splitSpriteToFragment();
	}
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(p.x, p.y);
    bodyDef.userData = sprite;
    b2Body* bodyA = mWorld->CreateBody(&bodyDef);
    
    {
        b2PolygonShape box;
        std::vector<b2PolygonShape> boxVec;
        
        box.SetAsBox(0.5f, 0.5f, b2Vec2(0.5f, 0.5f), 0);
        boxVec.insert(boxVec.end(), box);
        box.SetAsBox(0.5f, 0.5f, b2Vec2(-0.5f, 0.5f), 0);
        boxVec.insert(boxVec.end(), box);
        box.SetAsBox(0.5f, 0.5f, b2Vec2(-0.5f, -0.5f), 0);
        boxVec.insert(boxVec.end(), box);
        box.SetAsBox(0.5f, 0.5f, b2Vec2(0.5f, -0.5f), 0);
        boxVec.insert(boxVec.end(), box);
        
        for (size_t i=0; i<boxVec.size(); ++i) {
            b2FixtureDef fixtureDef;
            fixtureDef.shape = &boxVec[i];	
            fixtureDef.density = desity;
            fixtureDef.friction = Fixture_Friction;
            fixtureDef.restitution = Fixture_Restitution;
            
            bodyA->CreateFixture(&fixtureDef);
        }
    }
    
    if (angle != 0.0f) {
        bodyA->SetTransform(bodyA->GetPosition(), CC_DEGREES_TO_RADIANS(angle));
    }
    
    return bodyA;
}

b2Body* BaseContentLayer::createBox24(b2Vec2 p, float32 angle, bool canDestroy, float32 desity, bool isBullet){
    //先创建模糊后的纹理
    if (canDestroy) {
        addBluredSpriteToMap(ResManager::getManager()->getSharedFilePath(g_gameinPath+"box24.png"));
    }
    
    CustomeSprite* sprite = new CustomeSprite;
    
    if (canDestroy) {
        sprite->initWithFile(ResManager::getManager()->getSharedFilePath(g_gameinPath+"box24.png").c_str());
        addChild(sprite, 0, Tag_Box_can_Destroied);
    }else {
        sprite->initWithFile(ResManager::getManager()->getSharedFilePath(g_gameinPath+"box24_no.png").c_str());
        //sprite->setColor(ccGRAY);
        addChild(sprite, 0, Tag_Box_cannot_Destroied);
    }sprite->release();
    
    sprite->setPosition(CCPointMake(p.x*PTM_RATIO, p.y*PTM_RATIO));

	// box24创建碎片
	if(USE_MANUAL_FRAGMENT)
	{
		sprite->addFragment_ResMgr("box_frame_24.png",ccp(6,16),0);
		sprite->addFragment_ResMgr("box_frame_24.png",ccp(16,24),90);
		sprite->addFragment_ResMgr("box_frame_24.png",ccp(24,14),-180);
		sprite->addFragment_ResMgr("box_frame_24.png",ccp(13,5),-90);
		sprite->addFragment_ResMgr("box_middle_24.png",ccp(10,15),0);
		sprite->addFragment_ResMgr("box_middle_24.png",ccp(14,15),0);
		sprite->addFragment_ResMgr("box_middle_24.png",ccp(20,15),0);
	}
	else
	{
		sprite->splitSpriteToFragment();
	}
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(p.x, p.y);
    bodyDef.userData = sprite;
    b2Body* bodyA = mWorld->CreateBody(&bodyDef);
    
    b2PolygonShape box;
    box.SetAsBox(0.5f, 0.5f);
    
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &box;	
    fixtureDef.density = desity;
    fixtureDef.friction = Fixture_Friction;
    fixtureDef.restitution = Fixture_Restitution;
    
    bodyA->CreateFixture(&fixtureDef);
    
    if (angle != 0.0f) {
        bodyA->SetTransform(bodyA->GetPosition(), CC_DEGREES_TO_RADIANS(angle));
    }
    
    bodyA->SetBullet(isBullet);
    
    return bodyA;
}

b2Body* BaseContentLayer::createBox48_24(b2Vec2 p, float32 angle, bool canDestroy, float32 desity){
    //先创建模糊后的纹理
    if (canDestroy) {
        addBluredSpriteToMap(ResManager::getManager()->getSharedFilePath(g_gameinPath+"box48_24.png"));
    }
    
    
    
    CustomeSprite* sprite = new CustomeSprite;
    
    if (canDestroy) {
        sprite->initWithFile(ResManager::getManager()->getSharedFilePath(g_gameinPath+"box48_24.png").c_str());
        addChild(sprite, 0, Tag_Box_can_Destroied);
    }else {
        sprite->initWithFile(ResManager::getManager()->getSharedFilePath(g_gameinPath+"box48_24_no.png").c_str());
        //sprite->setColor(ccGRAY);
        addChild(sprite, 0, Tag_Box_cannot_Destroied);
    }sprite->release();
    
    sprite->setPosition(CCPointMake(p.x*PTM_RATIO, p.y*PTM_RATIO));
    
	// box48_24创建碎片
	if(USE_MANUAL_FRAGMENT)
	{
		sprite->addFragment_ResMgr("box_frame_24.png",ccp(11,17),0);
		sprite->addFragment_ResMgr("box_frame_48.png",ccp(32,26),90);
		sprite->addFragment_ResMgr("box_frame_24.png",ccp(49,14),180);
		sprite->addFragment_ResMgr("box_frame_48.png",ccp(32,9),-90);
		sprite->addFragment_ResMgr("box_middle_24.png",ccp(21,18),0);
		sprite->addFragment_ResMgr("box_middle_24.png",ccp(26,18),0);
		sprite->addFragment_ResMgr("box_middle_24.png",ccp(33,18),0);
		sprite->addFragment_ResMgr("box_middle_24.png",ccp(40,18),0);
	}
	else
	{
		sprite->splitSpriteToFragment();
	}
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(p.x, p.y);
    bodyDef.userData = sprite;
    b2Body* bodyA = mWorld->CreateBody(&bodyDef);
    
    {
        b2PolygonShape box;
        std::vector<b2PolygonShape> boxVec;
        
        box.SetAsBox(0.5f, 0.5f, b2Vec2(0.5f, 0.0f), 0);
        boxVec.insert(boxVec.end(), box);
        box.SetAsBox(0.5f, 0.5f, b2Vec2(-0.5f, 0.0f), 0);
        boxVec.insert(boxVec.end(), box);
        
        for (size_t i=0; i<boxVec.size(); ++i) {
            b2FixtureDef fixtureDef;
            fixtureDef.shape = &boxVec[i];	
            fixtureDef.density = desity;
            fixtureDef.friction = Fixture_Friction;
            fixtureDef.restitution = Fixture_Restitution;
            
            bodyA->CreateFixture(&fixtureDef);
        }
    }
    
    if (angle != 0.0f) {
        bodyA->SetTransform(bodyA->GetPosition(), CC_DEGREES_TO_RADIANS(angle));
    }
    
    return bodyA;
}

b2Body* BaseContentLayer::createBox72_48(b2Vec2 p, float32 angle, bool canDestroy, float32 desity){
    //先创建模糊后的纹理
    if (canDestroy) {
        addBluredSpriteToMap(ResManager::getManager()->getSharedFilePath(g_gameinPath+"box72_48.png"));
    }
    
    
    
    
    CustomeSprite* sprite = new CustomeSprite;
    
    if (canDestroy) {
        sprite->initWithFile(ResManager::getManager()->getSharedFilePath(g_gameinPath+"box72_48.png").c_str());
        addChild(sprite, 0, Tag_Box_can_Destroied);
    }else {
        sprite->initWithFile(ResManager::getManager()->getSharedFilePath(g_gameinPath+"box72_48_no.png").c_str());
        //sprite->setColor(ccGRAY);
        addChild(sprite, 0, Tag_Box_cannot_Destroied);
    }sprite->release();
    
    sprite->setPosition(CCPointMake(p.x*PTM_RATIO, p.y*PTM_RATIO));

	// box72_48创建碎片
	if(USE_MANUAL_FRAGMENT)
	{
		sprite->addFragment_ResMgr("box_frame_48.png",ccp(12,30),0);
		sprite->addFragment_ResMgr("box_frame_72.png",ccp(45,49),0);
		sprite->addFragment_ResMgr("box_frame_48.png",ccp(73,16),180);
		sprite->addFragment_ResMgr("box_frame_72.png",ccp(45,10),180);
		sprite->addFragment_ResMgr("box_middle_48.png",ccp(22,30),0);
		sprite->addFragment_ResMgr("box_middle_48.png",ccp(33,30),0);
		sprite->addFragment_ResMgr("box_middle_48.png",ccp(47,30),0);
		sprite->addFragment_ResMgr("box_middle_48.png",ccp(60,18),0);
	}
	else
	{
		sprite->splitSpriteToFragment();
	}
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(p.x, p.y);
    bodyDef.userData = sprite;
    b2Body* bodyA = mWorld->CreateBody(&bodyDef);
    
    {
        b2PolygonShape box;
        std::vector<b2PolygonShape> boxVec;
        
        box.SetAsBox(0.5f, 0.5f, b2Vec2(0.0f, 0.5f), 0);
        boxVec.insert(boxVec.end(), box);
        box.SetAsBox(0.5f, 0.5f, b2Vec2(1.0f, 0.5f), 0);
        boxVec.insert(boxVec.end(), box);
        box.SetAsBox(0.5f, 0.5f, b2Vec2(-1.0f, 0.5f), 0);
        boxVec.insert(boxVec.end(), box);
        box.SetAsBox(0.5f, 0.5f, b2Vec2(0.0f, -0.5f), 0);
        boxVec.insert(boxVec.end(), box);
        box.SetAsBox(0.5f, 0.5f, b2Vec2(1.0f, -0.5f), 0);
        boxVec.insert(boxVec.end(), box);
        box.SetAsBox(0.5f, 0.5f, b2Vec2(-1.0f, -0.5f), 0);
        boxVec.insert(boxVec.end(), box);
        
        for (size_t i=0; i<boxVec.size(); ++i) {
            b2FixtureDef fixtureDef;
            fixtureDef.shape = &boxVec[i];	
            fixtureDef.density = desity;
            fixtureDef.friction = Fixture_Friction;
            fixtureDef.restitution = Fixture_Restitution;
            
            bodyA->CreateFixture(&fixtureDef);
        }
    }
    
    if (angle != 0.0f) {
        bodyA->SetTransform(bodyA->GetPosition(), CC_DEGREES_TO_RADIANS(angle));
    }
    
    
    
    return bodyA;
}

b2Body* BaseContentLayer::createBox96_48(b2Vec2 p, float32 angle, bool canDestroy, float32 desity){
    //先创建模糊后的纹理
    if (canDestroy) {
        addBluredSpriteToMap(ResManager::getManager()->getSharedFilePath(g_gameinPath+"box96_48.png"));
    }
    
    
    CustomeSprite* sprite = new CustomeSprite;
    
    if (canDestroy) {
        sprite->initWithFile(ResManager::getManager()->getSharedFilePath(g_gameinPath+"box96_48.png").c_str());
        addChild(sprite, 0, Tag_Box_can_Destroied);
    }else {
        sprite->initWithFile(ResManager::getManager()->getSharedFilePath(g_gameinPath+"box96_48_no.png").c_str());
        //sprite->setColor(ccGRAY);
        addChild(sprite, 0, Tag_Box_cannot_Destroied);
    }sprite->release();
    
    sprite->setPosition(CCPointMake(p.x*PTM_RATIO, p.y*PTM_RATIO));
    
	// box96_48创建碎片
	if(USE_MANUAL_FRAGMENT)
	{
		sprite->addFragment_ResMgr("box_frame_48.png",ccp(12,30),0);
		sprite->addFragment_ResMgr("box_frame_96.png",ccp(56,50),0);
		sprite->addFragment_ResMgr("box_frame_48.png",ccp(96,30),180);
		sprite->addFragment_ResMgr("box_frame_96.png",ccp(56,13),180);
		sprite->addFragment_ResMgr("box_middle_48.png",ccp(24,32),0);
		sprite->addFragment_ResMgr("box_middle_48.png",ccp(42,32),0);
		sprite->addFragment_ResMgr("box_middle_48.png",ccp(56,32),0);
		sprite->addFragment_ResMgr("box_middle_48.png",ccp(71,32),0);
		sprite->addFragment_ResMgr("box_middle_48.png",ccp(87,32),0);

	}
	else
	{
		sprite->splitSpriteToFragment();
	}
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(p.x, p.y);
    bodyDef.userData = sprite;
    b2Body* bodyA = mWorld->CreateBody(&bodyDef);
    
    {
        b2PolygonShape box;
        std::vector<b2PolygonShape> boxVec;
        
        box.SetAsBox(0.5f, 0.5f, b2Vec2(0.5f, 0.5f), 0);
        boxVec.insert(boxVec.end(), box);
        box.SetAsBox(0.5f, 0.5f, b2Vec2(1.5f, 0.5f), 0);
        boxVec.insert(boxVec.end(), box);
        box.SetAsBox(0.5f, 0.5f, b2Vec2(-0.5f, 0.5f), 0);
        boxVec.insert(boxVec.end(), box);
        box.SetAsBox(0.5f, 0.5f, b2Vec2(-1.5f, 0.5f), 0);
        boxVec.insert(boxVec.end(), box);
        box.SetAsBox(0.5f, 0.5f, b2Vec2(0.5f, -0.5f), 0);
        boxVec.insert(boxVec.end(), box);
        box.SetAsBox(0.5f, 0.5f, b2Vec2(1.5f, -0.5f), 0);
        boxVec.insert(boxVec.end(), box);
        box.SetAsBox(0.5f, 0.5f, b2Vec2(-0.5f, -0.5f), 0);
        boxVec.insert(boxVec.end(), box);
        box.SetAsBox(0.5f, 0.5f, b2Vec2(-1.5f, -0.5f), 0);
        boxVec.insert(boxVec.end(), box);

        
        for (size_t i=0; i<boxVec.size(); ++i) {
            b2FixtureDef fixtureDef;
            fixtureDef.shape = &boxVec[i];	
            fixtureDef.density = desity;
            fixtureDef.friction = Fixture_Friction;
            fixtureDef.restitution = Fixture_Restitution;
            
            bodyA->CreateFixture(&fixtureDef);
        }
    }
    
    if (angle != 0.0f) {
        bodyA->SetTransform(bodyA->GetPosition(), CC_DEGREES_TO_RADIANS(angle));
    }
    
    
    
    return bodyA;
}

b2Body* BaseContentLayer::createBox168_24(b2Vec2 p, float32 angle, bool canDestroy, float32 desity){
    //先创建模糊后的纹理
    if (canDestroy) {
        addBluredSpriteToMap(ResManager::getManager()->getSharedFilePath(g_gameinPath+"box168_24.png"));
    }
    
    
    
    
    CustomeSprite* sprite = new CustomeSprite;
    
    if (canDestroy) {
        sprite->initWithFile(ResManager::getManager()->getSharedFilePath(g_gameinPath+"box168_24.png").c_str());
        addChild(sprite, 0, Tag_Box_can_Destroied);
    }else {
        sprite->initWithFile(ResManager::getManager()->getSharedFilePath(g_gameinPath+"box168_24_no.png").c_str());
        //sprite->setColor(ccGRAY);
        addChild(sprite, 0, Tag_Box_cannot_Destroied);
    }sprite->release();
    
    sprite->setPosition(CCPointMake(p.x*PTM_RATIO, p.y*PTM_RATIO));

	// box168_24创建碎片
	if(USE_MANUAL_FRAGMENT)
	{
		sprite->addFragment_ResMgr("box_frame_24.png",ccp(12,16),0);
		sprite->addFragment_ResMgr("box_frame_24.png",ccp(168,16),180);
		sprite->addFragment_ResMgr("box_spell_middle_48.png",ccp(42,24),0);
		sprite->addFragment_ResMgr("box_spell_middle_24.png",ccp(89,24),0);
		sprite->addFragment_ResMgr("box_spell_middle_48.png",ccp(136,24),0);
		sprite->addFragment_ResMgr("box_spell_middle_24.png",ccp(29,16),0);
		sprite->addFragment_ResMgr("box_spell_middle_48.png",ccp(77,16),0);
		sprite->addFragment_ResMgr("box_spell_middle_24.png",ccp(135,16),0);
		sprite->addFragment_ResMgr("box_spell_middle_48.png",ccp(47,10),0);
		sprite->addFragment_ResMgr("box_spell_middle_24.png",ccp(96,10),0);
		sprite->addFragment_ResMgr("box_spell_middle_48.png",ccp(142,10),0);

	}
	else
	{
		sprite->splitSpriteToFragment();
	}
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(p.x, p.y);
    bodyDef.userData = sprite;
    b2Body* bodyA = mWorld->CreateBody(&bodyDef);
    
    {
        b2PolygonShape box;
        std::vector<b2PolygonShape> boxVec;
        
        box.SetAsBox(0.5f, 0.5f, b2Vec2(0.0f, 0.0f), 0);
        boxVec.insert(boxVec.end(), box);
        box.SetAsBox(0.5f, 0.5f, b2Vec2(1.0f, 0.0f), 0);
        boxVec.insert(boxVec.end(), box);
        box.SetAsBox(0.5f, 0.5f, b2Vec2(2.0f, 0.0f), 0);
        boxVec.insert(boxVec.end(), box);
        box.SetAsBox(0.5f, 0.5f, b2Vec2(3.0f, 0.0f), 0);
        boxVec.insert(boxVec.end(), box);
        box.SetAsBox(0.5f, 0.5f, b2Vec2(-1.0f, 0.0f), 0);
        boxVec.insert(boxVec.end(), box);
        box.SetAsBox(0.5f, 0.5f, b2Vec2(-2.0f, 0.0f), 0);
        boxVec.insert(boxVec.end(), box);
        box.SetAsBox(0.5f, 0.5f, b2Vec2(-3.0f, 0.0f), 0);
        boxVec.insert(boxVec.end(), box);
        
        for (size_t i=0; i<boxVec.size(); ++i) {
            b2FixtureDef fixtureDef;
            fixtureDef.shape = &boxVec[i];	
            fixtureDef.density = desity;
            fixtureDef.friction = Fixture_Friction;
            fixtureDef.restitution = Fixture_Restitution;
            
            bodyA->CreateFixture(&fixtureDef);
        }
    }
    
    if (angle != 0.0f) {
        bodyA->SetTransform(bodyA->GetPosition(), CC_DEGREES_TO_RADIANS(angle));
    }
    
    return bodyA;
}

b2Body* BaseContentLayer::createBox192_24(b2Vec2 p, float32 angle, bool canDestroy, float32 desity){
    //先创建模糊后的纹理
    if (canDestroy) {
        addBluredSpriteToMap(ResManager::getManager()->getSharedFilePath(g_gameinPath+"box192_24.png"));
    }
    
    
    CustomeSprite* sprite = new CustomeSprite;
    
    if (canDestroy) {
        sprite->initWithFile(ResManager::getManager()->getSharedFilePath(g_gameinPath+"box192_24.png").c_str());
        addChild(sprite, 0, Tag_Box_can_Destroied);
    }else {
        sprite->initWithFile(ResManager::getManager()->getSharedFilePath(g_gameinPath+"box192_24_no.png").c_str());
        //sprite->setColor(ccGRAY);
        addChild(sprite, 0, Tag_Box_cannot_Destroied);
    }sprite->release();
    
    sprite->setPosition(CCPointMake(p.x*PTM_RATIO, p.y*PTM_RATIO));
    
	// box192创建碎片
	if(USE_MANUAL_FRAGMENT)
	{
		sprite->addFragment_ResMgr("box_frame_24.png",ccp(11,16),0);
		sprite->addFragment_ResMgr("box_frame_24.png",ccp(191,16),180);
		//sprite->addFragment_ResMgr("box_spell_middle_48.png",ccp(40,25),0);
		sprite->addFragment_ResMgr("box_spell_middle_24.png",ccp(80,25),0);
		sprite->addFragment_ResMgr("box_spell_middle_24.png",ccp(103,25),0);
		//sprite->addFragment_ResMgr("box_spell_middle_48.png",ccp(140,25),0);
		sprite->addFragment_ResMgr("box_spell_middle_24.png",ccp(178,25),0);
		sprite->addFragment_ResMgr("box_spell_middle_24.png",ccp(28,18),0);
		sprite->addFragment_ResMgr("box_spell_middle_48.png",ccp(65,18),0);
		//sprite->addFragment_ResMgr("box_spell_middle_24.png",ccp(103,18),0);
		sprite->addFragment_ResMgr("box_spell_middle_24.png",ccp(127,18),0);
		sprite->addFragment_ResMgr("box_spell_middle_48.png",ccp(164,18),0);
		sprite->addFragment_ResMgr("box_spell_middle_48.png",ccp(41,11),0);
		sprite->addFragment_ResMgr("box_spell_middle_24.png",ccp(78,11),0);
		sprite->addFragment_ResMgr("box_spell_middle_48.png",ccp(116,11),0);
		sprite->addFragment_ResMgr("box_spell_middle_24.png",ccp(152,11),0);
		//sprite->addFragment_ResMgr("box_spell_middle_24.png",ccp(177,11),0);
	}
	else
	{
		sprite->splitSpriteToFragment();
	}
    
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(p.x, p.y);
    bodyDef.userData = sprite;
    b2Body* bodyA = mWorld->CreateBody(&bodyDef);
    
    {
        for (int i=0; i<8; ++i) {
            b2PolygonShape box;
            box.SetAsBox(0.5f, 0.5f, b2Vec2(-3.5f+i, 0.0f), 0);
            
            b2FixtureDef fixtureDef;
            fixtureDef.shape = &box;	
            fixtureDef.density = desity;
            fixtureDef.friction = Fixture_Friction;
            fixtureDef.restitution = Fixture_Restitution;
            
            bodyA->CreateFixture(&fixtureDef);
        }
    }
    
    if (angle != 0.0f) {
        bodyA->SetTransform(bodyA->GetPosition(), CC_DEGREES_TO_RADIANS(angle));
    }
    
    return bodyA;
}

b2Body* BaseContentLayer::createBox216_24(b2Vec2 p, float32 angle, bool canDestroy, float32 desity){
    //先创建模糊后的纹理
    if (canDestroy) {
        addBluredSpriteToMap(ResManager::getManager()->getSharedFilePath(g_gameinPath+"box216_24.png"));
    }

    
    
    CustomeSprite* sprite = new CustomeSprite;
    
    if (canDestroy) {
        sprite->initWithFile(ResManager::getManager()->getSharedFilePath(g_gameinPath+"box216_24.png").c_str());
        addChild(sprite, 0, Tag_Box_can_Destroied);
    }else {
        sprite->initWithFile(ResManager::getManager()->getSharedFilePath(g_gameinPath+"box216_24_no.png").c_str());
        //sprite->setColor(ccGRAY);
        addChild(sprite, 0, Tag_Box_cannot_Destroied);
    }sprite->release();
    
    sprite->setPosition(CCPointMake(p.x*PTM_RATIO, p.y*PTM_RATIO));
    
	// box216_24创建碎片
	if(USE_MANUAL_FRAGMENT)
	{
		sprite->addFragment_ResMgr("box_frame_24.png",ccp(12,16),0);
		sprite->addFragment_ResMgr("box_frame_24.png",ccp(216,16),180);
		sprite->addFragment_ResMgr("box_spell_middle_48.png",ccp(48,24),0);
		sprite->addFragment_ResMgr("box_spell_middle_48.png",ccp(105,24),0);
		sprite->addFragment_ResMgr("box_spell_middle_24.png",ccp(148,24),0);
		sprite->addFragment_ResMgr("box_spell_middle_48.png",ccp(189,16),0);
		sprite->addFragment_ResMgr("box_spell_middle_48.png",ccp(42,16),0);
		sprite->addFragment_ResMgr("box_spell_middle_24.png",ccp(92,16),0);
		sprite->addFragment_ResMgr("box_spell_middle_24.png",ccp(132,10),0);
		sprite->addFragment_ResMgr("box_spell_middle_48.png",ccp(185,10),0);
		sprite->addFragment_ResMgr("box_spell_middle_24.png",ccp(40,10),0);
		sprite->addFragment_ResMgr("box_spell_middle_48.png",ccp(90,10),0);
		sprite->addFragment_ResMgr("box_spell_middle_48.png",ccp(150,10),0);
		sprite->addFragment_ResMgr("box_spell_middle_24.png",ccp(200,10),0);

	}
	else
	{
		sprite->splitSpriteToFragment();
	}
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(p.x, p.y);
    bodyDef.userData = sprite;
    b2Body* bodyA = mWorld->CreateBody(&bodyDef);
    
    {
        for (int i=0; i<9; ++i) {
            b2PolygonShape box;
            box.SetAsBox(0.5f, 0.5f, b2Vec2(-4.0f+i, 0.0f), 0);
            
            b2FixtureDef fixtureDef;
            fixtureDef.shape = &box;	
            fixtureDef.density = desity;
            fixtureDef.friction = Fixture_Friction;
            fixtureDef.restitution = Fixture_Restitution;
            
            bodyA->CreateFixture(&fixtureDef);
        }
    }
    
    if (angle != 0.0f) {
        bodyA->SetTransform(bodyA->GetPosition(), CC_DEGREES_TO_RADIANS(angle));
    }
    
    return bodyA;
}

b2Body* BaseContentLayer::createBox240_24(b2Vec2 p, float32 angle, bool canDestroy, float32 desity){
    //先创建模糊后的纹理
    if (canDestroy) {
        addBluredSpriteToMap(ResManager::getManager()->getSharedFilePath(g_gameinPath+"box240_24.png"));
    }
    
    
    
    
    CustomeSprite* sprite = new CustomeSprite;
    
    if (canDestroy) {
        sprite->initWithFile(ResManager::getManager()->getSharedFilePath(g_gameinPath+"box240_24.png").c_str());
        addChild(sprite, 0, Tag_Box_can_Destroied);
    }else {
        sprite->initWithFile(ResManager::getManager()->getSharedFilePath(g_gameinPath+"box240_24_no.png").c_str());
        //sprite->setColor(ccGRAY);
        addChild(sprite, 0, Tag_Box_cannot_Destroied);
    }sprite->release();
    
    sprite->setPosition(CCPointMake(p.x*PTM_RATIO, p.y*PTM_RATIO));
    
	// box240_24创建碎片
	if(USE_MANUAL_FRAGMENT)
	{
		sprite->addFragment_ResMgr("box_frame_24.png",ccp(12,16),0);
		sprite->addFragment_ResMgr("box_frame_24.png",ccp(240,16),180);
		sprite->addFragment_ResMgr("box_spell_middle_48.png",ccp(43,24),0);
		sprite->addFragment_ResMgr("box_spell_middle_24.png",ccp(85,24),0);
		sprite->addFragment_ResMgr("box_spell_middle_48.png",ccp(128,24),0);
		sprite->addFragment_ResMgr("box_spell_middle_24.png",ccp(170,16),0);
		sprite->addFragment_ResMgr("box_spell_middle_48.png",ccp(214,16),0);
		sprite->addFragment_ResMgr("box_spell_middle_48.png",ccp(50,16),0);
		sprite->addFragment_ResMgr("box_spell_middle_48.png",ccp(120,10),0);
		sprite->addFragment_ResMgr("box_spell_middle_24.png",ccp(162,10),0);
		sprite->addFragment_ResMgr("box_spell_middle_48.png",ccp(210,10),0);
		sprite->addFragment_ResMgr("box_spell_middle_24.png",ccp(39,10),0);
		sprite->addFragment_ResMgr("box_spell_middle_48.png",ccp(91,10),0);
		sprite->addFragment_ResMgr("box_spell_middle_48.png",ccp(157,10),0);
		sprite->addFragment_ResMgr("box_spell_middle_24.png",ccp(212,10),0);
	}
	else
	{
		sprite->splitSpriteToFragment();
	}
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(p.x, p.y);
    bodyDef.userData = sprite;
    b2Body* bodyA = mWorld->CreateBody(&bodyDef);
    
    {
        for (int i=0; i<10; ++i) {
            b2PolygonShape box;
            box.SetAsBox(0.5f, 0.5f, b2Vec2(-4.5f+i, 0.0f), 0);
            
            b2FixtureDef fixtureDef;
            fixtureDef.shape = &box;	
            fixtureDef.density = desity;
            fixtureDef.friction = Fixture_Friction;
            fixtureDef.restitution = Fixture_Restitution;
            
            bodyA->CreateFixture(&fixtureDef);
        }
    }
    
    if (angle != 0.0f) {
        bodyA->SetTransform(bodyA->GetPosition(), CC_DEGREES_TO_RADIANS(angle));
    }
    
    return bodyA;
}

b2Body* BaseContentLayer::createBox264_24(b2Vec2 p, float32 angle, bool canDestroy, float32 desity){
    //先创建模糊后的纹理
    if (canDestroy) {
        addBluredSpriteToMap(ResManager::getManager()->getSharedFilePath(g_gameinPath+"box264_24.png"));
    }
    
    
    
    
    CustomeSprite* sprite = new CustomeSprite;
    
    if (canDestroy) {
        sprite->initWithFile(ResManager::getManager()->getSharedFilePath(g_gameinPath+"box264_24.png").c_str());
        addChild(sprite, 0, Tag_Box_can_Destroied);
    }else {
        sprite->initWithFile(ResManager::getManager()->getSharedFilePath(g_gameinPath+"box264_24_no.png").c_str());
        //sprite->setColor(ccGRAY);
        addChild(sprite, 0, Tag_Box_cannot_Destroied);
    }sprite->release();
    
    sprite->setPosition(CCPointMake(p.x*PTM_RATIO, p.y*PTM_RATIO));
    
	// box264_24创建碎片
	if(USE_MANUAL_FRAGMENT)
	{
		sprite->addFragment_ResMgr("box_frame_24.png",ccp(12,16),0);
		sprite->addFragment_ResMgr("box_frame_24.png",ccp(264,16),180);
		sprite->addFragment_ResMgr("box_spell_middle_48.png",ccp(42,24),0);
		sprite->addFragment_ResMgr("box_spell_middle_24.png",ccp(93,24),0);
		sprite->addFragment_ResMgr("box_spell_middle_48.png",ccp(142,24),0);
		sprite->addFragment_ResMgr("box_spell_middle_24.png",ccp(190,16),0);
		sprite->addFragment_ResMgr("box_spell_middle_48.png",ccp(235,16),0);
		sprite->addFragment_ResMgr("box_spell_middle_24.png",ccp(30,16),0);
		sprite->addFragment_ResMgr("box_spell_middle_48.png",ccp(75,10),0);
		sprite->addFragment_ResMgr("box_spell_middle_48.png",ccp(132,10),0);
		sprite->addFragment_ResMgr("box_spell_middle_48.png",ccp(200,10),0);
		sprite->addFragment_ResMgr("box_spell_middle_24.png",ccp(246,10),0);
		sprite->addFragment_ResMgr("box_spell_middle_48.png",ccp(42,10),0);
		sprite->addFragment_ResMgr("box_spell_middle_48.png",ccp(98,10),0);
		sprite->addFragment_ResMgr("box_spell_middle_24.png",ccp(147,10),0);
		sprite->addFragment_ResMgr("box_spell_middle_24.png",ccp(185,10),0);
		sprite->addFragment_ResMgr("box_spell_middle_48.png",ccp(230,10),0);

	}
	else
	{
		sprite->splitSpriteToFragment();
	}

    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(p.x, p.y);
    bodyDef.userData = sprite;
    b2Body* bodyA = mWorld->CreateBody(&bodyDef);
    
    {
        for (int i=0; i<11; ++i) {
            b2PolygonShape box;
            box.SetAsBox(0.5f, 0.5f, b2Vec2(-5.0f+i, 0.0f), 0);
            
            b2FixtureDef fixtureDef;
            fixtureDef.shape = &box;	
            fixtureDef.density = desity;
            fixtureDef.friction = Fixture_Friction;
            fixtureDef.restitution = Fixture_Restitution;
            
            bodyA->CreateFixture(&fixtureDef);
        }
    }
    
    if (angle != 0.0f) {
        bodyA->SetTransform(bodyA->GetPosition(), CC_DEGREES_TO_RADIANS(angle));
    }
    
    return bodyA;
}

b2Body* BaseContentLayer::createBox360_24(b2Vec2 p, float32 angle, bool canDestroy, float32 desity){
    //先创建模糊后的纹理
    if (canDestroy) {
        addBluredSpriteToMap(ResManager::getManager()->getSharedFilePath(g_gameinPath+"box360_24.png"));
    }
    
    
    
    
    CustomeSprite* sprite = new CustomeSprite;
    
    if (canDestroy) {
        sprite->initWithFile(ResManager::getManager()->getSharedFilePath(g_gameinPath+"box360_24.png").c_str());
        addChild(sprite, 0, Tag_Box_can_Destroied);
    }else {
        sprite->initWithFile(ResManager::getManager()->getSharedFilePath(g_gameinPath+"box360_24_no.png").c_str());
        //sprite->setColor(ccGRAY);
        addChild(sprite, 0, Tag_Box_cannot_Destroied);
    }sprite->release();
    
    sprite->setPosition(CCPointMake(p.x*PTM_RATIO, p.y*PTM_RATIO));
    
	// box360_24创建碎片
	if(USE_MANUAL_FRAGMENT)
	{
		sprite->addFragment_ResMgr("box_frame_24.png",ccp(12,16),0);
		sprite->addFragment_ResMgr("box_frame_24.png",ccp(360,16),180);
		sprite->addFragment_ResMgr("box_spell_middle_48.png",ccp(46,24),0);
		sprite->addFragment_ResMgr("box_spell_middle_48.png",ccp(103,24),0);
		sprite->addFragment_ResMgr("box_spell_middle_24.png",ccp(146,24),0);
		sprite->addFragment_ResMgr("box_spell_middle_48.png",ccp(192,16),0);
		sprite->addFragment_ResMgr("box_spell_middle_24.png",ccp(236,16),0);
		sprite->addFragment_ResMgr("box_spell_middle_48.png",ccp(274,16),0);
		sprite->addFragment_ResMgr("box_spell_middle_48.png",ccp(329,10),0);
		sprite->addFragment_ResMgr("box_spell_middle_24.png",ccp(32,10),0);
		sprite->addFragment_ResMgr("box_spell_middle_48.png",ccp(80,10),0);
		sprite->addFragment_ResMgr("box_spell_middle_24.png",ccp(130,10),0);
		sprite->addFragment_ResMgr("box_spell_middle_48.png",ccp(176,10),0);
		sprite->addFragment_ResMgr("box_spell_middle_48.png",ccp(235,10),0);
		sprite->addFragment_ResMgr("box_spell_middle_24.png",ccp(275,10),0);
		sprite->addFragment_ResMgr("box_spell_middle_48.png",ccp(320,10),0);
		sprite->addFragment_ResMgr("box_spell_middle_48.png",ccp(50,10),0);
		sprite->addFragment_ResMgr("box_spell_middle_24.png",ccp(96,10),0);
		sprite->addFragment_ResMgr("box_spell_middle_48.png",ccp(145,10),0);
		sprite->addFragment_ResMgr("box_spell_middle_48.png",ccp(205,10),0);
		sprite->addFragment_ResMgr("box_spell_middle_48.png",ccp(270,10),0);
		sprite->addFragment_ResMgr("box_spell_middle_24.png",ccp(312,10),0);
		sprite->addFragment_ResMgr("box_spell_middle_24.png",ccp(345,10),0);
	}
	else
	{
		sprite->splitSpriteToFragment();
	}
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(p.x, p.y);
    bodyDef.userData = sprite;
    b2Body* bodyA = mWorld->CreateBody(&bodyDef);
    
    {
        for (int i=0; i<15; ++i) {
            b2PolygonShape box;
            box.SetAsBox(0.5f, 0.5f, b2Vec2(-7.0f+i, 0.0f), 0);
            
            b2FixtureDef fixtureDef;
            fixtureDef.shape = &box;	
            fixtureDef.density = desity;
            fixtureDef.friction = Fixture_Friction;
            fixtureDef.restitution = Fixture_Restitution;
            
            bodyA->CreateFixture(&fixtureDef);
        }
    }
    
    if (angle != 0.0f) {
        bodyA->SetTransform(bodyA->GetPosition(), CC_DEGREES_TO_RADIANS(angle));
    }
    
    return bodyA;
}

b2Body* BaseContentLayer::createBox72_24(b2Vec2 p, float32 angle, bool canDestroy, float32 desity){
    //先创建模糊后的纹理
    if (canDestroy) {
        addBluredSpriteToMap(ResManager::getManager()->getSharedFilePath(g_gameinPath+"box72_24.png"));
    }
    
    
    
    
    CustomeSprite* sprite = new CustomeSprite;
    
    if (canDestroy) {
        sprite->initWithFile(ResManager::getManager()->getSharedFilePath(g_gameinPath+"box72_24.png").c_str());
        addChild(sprite, 0, Tag_Box_can_Destroied);
    }else {
        sprite->initWithFile(ResManager::getManager()->getSharedFilePath(g_gameinPath+"box72_24_no.png").c_str());
        //sprite->setColor(ccGRAY);
        addChild(sprite, 0, Tag_Box_cannot_Destroied);
    }sprite->release();
    
    sprite->setPosition(CCPointMake(p.x*PTM_RATIO, p.y*PTM_RATIO));
	
	// box72_24创建碎片
	if(USE_MANUAL_FRAGMENT)
	{
		sprite->addFragment_ResMgr("box_frame_24.png",ccp(12,16),0);
		sprite->addFragment_ResMgr("box_frame_72.png",ccp(43,27),0);
		sprite->addFragment_ResMgr("box_frame_24.png",ccp(75,16),180);
		sprite->addFragment_ResMgr("box_frame_72.png",ccp(43,9),180);
		sprite->addFragment_ResMgr("box_middle_24.png",ccp(21,18),0);
		sprite->addFragment_ResMgr("box_middle_24.png",ccp(31,18),0);
		sprite->addFragment_ResMgr("box_middle_24.png",ccp(40,18),0);
		sprite->addFragment_ResMgr("box_middle_24.png",ccp(50,18),0);
		sprite->addFragment_ResMgr("box_middle_24.png",ccp(59,18),0);
		sprite->addFragment_ResMgr("box_middle_24.png",ccp(66,18),0);
	}
	else
	{
		sprite->splitSpriteToFragment();
	}
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(p.x, p.y);
    bodyDef.userData = sprite;
    b2Body* bodyA = mWorld->CreateBody(&bodyDef);
    
    {
        b2PolygonShape box;
        std::vector<b2PolygonShape> boxVec;
        
        box.SetAsBox(0.5f, 0.5f, b2Vec2(0.0f, 0.0f), 0);
        boxVec.insert(boxVec.end(), box);
        box.SetAsBox(0.5f, 0.5f, b2Vec2(1.0f, 0.0f), 0);
        boxVec.insert(boxVec.end(), box);
        box.SetAsBox(0.5f, 0.5f, b2Vec2(-1.0f, 0.0f), 0);
        boxVec.insert(boxVec.end(), box);
        
        for (size_t i=0; i<boxVec.size(); ++i) {
            b2FixtureDef fixtureDef;
            fixtureDef.shape = &boxVec[i];
            fixtureDef.density = desity;
            fixtureDef.friction = Fixture_Friction;
            fixtureDef.restitution = Fixture_Restitution;
            
            bodyA->CreateFixture(&fixtureDef);
        }
    }
    
    if (angle != 0.0f) {
        bodyA->SetTransform(bodyA->GetPosition(), CC_DEGREES_TO_RADIANS(angle));
    }
    
    
    return bodyA;
}

b2Body* BaseContentLayer::createBox120_24(b2Vec2 p, float32 angle, bool canDestroy, float32 desity){
    //先创建模糊后的纹理
    if (canDestroy) {
        addBluredSpriteToMap(ResManager::getManager()->getSharedFilePath(g_gameinPath+"box120_24.png"));
    }
    
    
    
    
    CustomeSprite* sprite = new CustomeSprite;
    
    if (canDestroy) {
        sprite->initWithFile(ResManager::getManager()->getSharedFilePath(g_gameinPath+"box120_24.png").c_str());
        addChild(sprite, 0, Tag_Box_can_Destroied);
    }else {
        sprite->initWithFile(ResManager::getManager()->getSharedFilePath(g_gameinPath+"box120_24_no.png").c_str());
        //sprite->setColor(ccGRAY);
        addChild(sprite, 0, Tag_Box_cannot_Destroied);
    }sprite->release();
    
    sprite->setPosition(CCPointMake(p.x*PTM_RATIO, p.y*PTM_RATIO));
    
	// box120_24创建碎片
	if(USE_MANUAL_FRAGMENT)
	{
		sprite->addFragment_ResMgr("box_frame_24.png",ccp(11,16),0);
		sprite->addFragment_ResMgr("box_frame_24.png",ccp(120,16),180);
		sprite->addFragment_ResMgr("box_spell_middle_48.png",ccp(43,24),0);
		sprite->addFragment_ResMgr("box_spell_middle_48.png",ccp(92,24),0);
		sprite->addFragment_ResMgr("box_spell_middle_24.png",ccp(28,16),0);
		sprite->addFragment_ResMgr("box_spell_middle_24.png",ccp(65,16),0);
		sprite->addFragment_ResMgr("box_spell_middle_24.png",ccp(103,25),0);
		sprite->addFragment_ResMgr("box_spell_middle_48.png",ccp(41,10),0);
		sprite->addFragment_ResMgr("box_spell_middle_48.png",ccp(94,18),0);
	}
	else
	{
		sprite->splitSpriteToFragment();
	}
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(p.x, p.y);
    bodyDef.userData = sprite;
    b2Body* bodyA = mWorld->CreateBody(&bodyDef);
    
    {
        b2PolygonShape box;
        std::vector<b2PolygonShape> boxVec;
        
        box.SetAsBox(0.5f, 0.5f, b2Vec2(0.0f, 0.0f), 0);
        boxVec.insert(boxVec.end(), box);
        box.SetAsBox(0.5f, 0.5f, b2Vec2(1.0f, 0.0f), 0);
        boxVec.insert(boxVec.end(), box);
        box.SetAsBox(0.5f, 0.5f, b2Vec2(-1.0f, 0.0f), 0);
        boxVec.insert(boxVec.end(), box);
        box.SetAsBox(0.5f, 0.5f, b2Vec2(2.0f, 0.0f), 0);
        boxVec.insert(boxVec.end(), box);
        box.SetAsBox(0.5f, 0.5f, b2Vec2(-2.0f, 0.0f), 0);
        boxVec.insert(boxVec.end(), box);
        
        for (size_t i=0; i<boxVec.size(); ++i) {
            b2FixtureDef fixtureDef;
            fixtureDef.shape = &boxVec[i];
            fixtureDef.density = desity;
            fixtureDef.friction = Fixture_Friction;
            fixtureDef.restitution = Fixture_Restitution;
            
            bodyA->CreateFixture(&fixtureDef);
        }
    }
    
    if (angle != 0.0f) {
        bodyA->SetTransform(bodyA->GetPosition(), CC_DEGREES_TO_RADIANS(angle));
    }
    
    
    return bodyA;
}



b2Body* BaseContentLayer::createBox96_24(b2Vec2 p, float32 angle, bool canDestroy, float32 desity){
    //先创建模糊后的纹理
    if (canDestroy) {
        addBluredSpriteToMap(ResManager::getManager()->getSharedFilePath(g_gameinPath+"box96_24.png"));
    }
    
    
    CustomeSprite* sprite = new CustomeSprite;
    
    if (canDestroy) {
        sprite->initWithFile(ResManager::getManager()->getSharedFilePath(g_gameinPath+"box96_24.png").c_str());
        addChild(sprite, 0, Tag_Box_can_Destroied);
    }else {
        sprite->initWithFile(ResManager::getManager()->getSharedFilePath(g_gameinPath+"box96_24_no.png").c_str());
        //sprite->setColor(ccGRAY);
        addChild(sprite, 0, Tag_Box_cannot_Destroied);
    }sprite->release();
    
    sprite->setPosition(CCPointMake(p.x*PTM_RATIO, p.y*PTM_RATIO));
    
	// box96_24创建碎片
	if(USE_MANUAL_FRAGMENT)
	{
		sprite->addFragment_ResMgr("box_frame_24.png",ccp(12,18),0);
		sprite->addFragment_ResMgr("box_frame_96.png",ccp(53,27),0);
		sprite->addFragment_ResMgr("box_frame_24.png",ccp(98,18),180);
		sprite->addFragment_ResMgr("box_frame_96.png",ccp(53,9),180);
		sprite->addFragment_ResMgr("box_middle_24.png",ccp(18,18),0);
		sprite->addFragment_ResMgr("box_middle_24.png",ccp(28,18),0);
		sprite->addFragment_ResMgr("box_middle_24.png",ccp(42,18),0);
		sprite->addFragment_ResMgr("box_middle_24.png",ccp(54,18),0);
		sprite->addFragment_ResMgr("box_middle_24.png",ccp(64,18),0);
		sprite->addFragment_ResMgr("box_middle_24.png",ccp(76,18),0);
		sprite->addFragment_ResMgr("box_middle_24.png",ccp(86,18),0);
	}
	else
	{
		sprite->splitSpriteToFragment();
	}
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(p.x, p.y);
    bodyDef.userData = sprite;
    b2Body* bodyA = mWorld->CreateBody(&bodyDef);
    
    {
        b2PolygonShape box;
        std::vector<b2PolygonShape> boxVec;
        
        box.SetAsBox(0.5f, 0.5f, b2Vec2(0.5f, 0.0f), 0);
        boxVec.insert(boxVec.end(), box);
        box.SetAsBox(0.5f, 0.5f, b2Vec2(-0.5f, 0.0f), 0);
        boxVec.insert(boxVec.end(), box);
        box.SetAsBox(0.5f, 0.5f, b2Vec2(1.5f, 0.0f), 0);
        boxVec.insert(boxVec.end(), box);
        box.SetAsBox(0.5f, 0.5f, b2Vec2(-1.5f, 0.0f), 0);
        boxVec.insert(boxVec.end(), box);
        
        for (size_t i=0; i<boxVec.size(); ++i) {
            b2FixtureDef fixtureDef;
            fixtureDef.shape = &boxVec[i];
            fixtureDef.density = desity;
            fixtureDef.friction = Fixture_Friction;
            fixtureDef.restitution = Fixture_Restitution;
            
            bodyA->CreateFixture(&fixtureDef);
        }
    }
    
    if (angle != 0.0f) {
        bodyA->SetTransform(bodyA->GetPosition(), CC_DEGREES_TO_RADIANS(angle));
    }
    
    
    return bodyA;
}


b2Body* BaseContentLayer::createTriangle56(b2Vec2 p, float32 angle, bool canDestroy, float32 desity){
    //先创建模糊后的纹理
    if (canDestroy) {
        addBluredSpriteToMap(ResManager::getManager()->getSharedFilePath(g_gameinPath+"triangle56.png"));
    }
    
    
    CustomeSprite* sprite = new CustomeSprite;
    
    if (canDestroy) {
        sprite->initWithFile(ResManager::getManager()->getSharedFilePath(g_gameinPath+"triangle56.png").c_str());
        addChild(sprite, 0, Tag_Box_can_Destroied);
    }else {
        sprite->initWithFile(ResManager::getManager()->getSharedFilePath(g_gameinPath+"triangle56_no.png").c_str());
        addChild(sprite, 0, Tag_Box_cannot_Destroied);
    }sprite->release();
    
    sprite->setPosition(CCPointMake(p.x*PTM_RATIO, p.y*PTM_RATIO));

	// triangle56创建碎片
	if(USE_MANUAL_FRAGMENT)
	{
		sprite->addFragment_ResMgr("triangle56_frame1.png",ccp(22,47),0);
		sprite->addFragment_ResMgr("triangle56_frame2.png",ccp(47,42),0);
		sprite->addFragment_ResMgr("triangle56_frame3.png",ccp(30,26),0);
		sprite->addFragment_ResMgr("triangle56_spell_middle1.png",ccp(21,32),0);
		sprite->addFragment_ResMgr("triangle56_spell_middle2.png",ccp(29,38),0);
		sprite->addFragment_ResMgr("triangle56_spell_middle3.png",ccp(39,38),0);
		sprite->addFragment_ResMgr("triangle56_spell_middle4.png",ccp(45,32),0);
	}
	else
	{
		sprite->splitSpriteToFragment();
	}
    //sprite->setPosition( CCPointMake( p.x, p.y) );
    
    // Define the dynamic body.
    //Set up a 1m squared box in the physics world
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(p.x, p.y);
    bodyDef.userData = sprite;
    b2Body *body = mWorld->CreateBody(&bodyDef);
    
    b2Vec2 trianglArr[] = {
        b2Vec2(0.0f/PTM_RATIO,1.3461f*BasicLength/PTM_RATIO),b2Vec2(-1.1667f*BasicLength/PTM_RATIO,-0.6736f*BasicLength/PTM_RATIO),b2Vec2(1.1667f*BasicLength/PTM_RATIO,-0.6736f*BasicLength/PTM_RATIO)    
    };
    
    b2PolygonShape dynamicBox;
    dynamicBox.Set(trianglArr, 3);
    
    // Define the dynamic body fixture.
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &dynamicBox;	
    fixtureDef.density = desity;
    fixtureDef.friction = Fixture_Friction;
    fixtureDef.restitution = Fixture_Restitution;
    body->CreateFixture(&fixtureDef);
    
    if (angle != 0.0f) {
        body->SetTransform(body->GetPosition(), CC_DEGREES_TO_RADIANS(angle));
    }
    
    return body;
}

b2Body* BaseContentLayer::createFork24(b2Vec2 p, float32 angle, bool canDestroy, float32 desity){
    //先创建模糊后的纹理
    if (canDestroy) {
        addBluredSpriteToMap(ResManager::getManager()->getSharedFilePath(g_gameinPath+"fork24.png"));
    }
    
    
    
    
    CustomeSprite* sprite = new CustomeSprite;
    
    if (canDestroy) {
        sprite->initWithFile(ResManager::getManager()->getSharedFilePath(g_gameinPath+"fork24.png").c_str());
        addChild(sprite, 0, Tag_Box_can_Destroied);
    }else {
        sprite->initWithFile(ResManager::getManager()->getSharedFilePath(g_gameinPath+"fork24_no.png").c_str());
        sprite->setColor(ccGRAY);
        addChild(sprite, 0, Tag_Box_cannot_Destroied);
    }sprite->release();
    
    sprite->setPosition(CCPointMake(p.x*PTM_RATIO, p.y*PTM_RATIO));
	// fork24创建碎片
	if(USE_MANUAL_FRAGMENT)
	{
		sprite->addFragment_ResMgr("fork24_frame.png",ccp(14,18),-35);
		sprite->addFragment_ResMgr("fork24_frame.png",ccp(40,64),90);
		sprite->addFragment_ResMgr("fork24_frame.png",ccp(65,18),-145);
		sprite->addFragment_ResMgr("fork24_spell_middle1.png",ccp(16,26),0);
		sprite->addFragment_ResMgr("fork24_spell_middle2.png",ccp(28,36),0);
		sprite->addFragment_ResMgr("fork24_spell_middle3.png",ccp(41,37),0);
		sprite->addFragment_ResMgr("fork24_spell_middle4.png",ccp(54,35),0);
		sprite->addFragment_ResMgr("fork24_spell_middle1.png",ccp(63,26),180);

	}
	else
	{
		sprite->splitSpriteToFragment();
	}
    // Define the dynamic body.
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(p.x, p.y);
    bodyDef.userData = sprite;
    b2Body *body = mWorld->CreateBody(&bodyDef);
    
    b2Vec2 boxArr[][4] = {
        {b2Vec2(-12.0f/PTM_RATIO,31.785f/PTM_RATIO),b2Vec2(-12.0f/PTM_RATIO,3.785f/PTM_RATIO),b2Vec2(12.0f/PTM_RATIO,3.785f/PTM_RATIO),b2Vec2(12.0f/PTM_RATIO,31.785f/PTM_RATIO)}
        ,{b2Vec2(-12.0f/PTM_RATIO,3.785f/PTM_RATIO),b2Vec2(-36.249f/PTM_RATIO,-11.785f/PTM_RATIO),b2Vec2(-24.249f/PTM_RATIO,-31.0f/PTM_RATIO),b2Vec2(0.0f/PTM_RATIO,-17.0f/PTM_RATIO)}
        ,{b2Vec2(12.0f/PTM_RATIO,3.785f/PTM_RATIO),b2Vec2(0.0f/PTM_RATIO,-17.0f/PTM_RATIO),b2Vec2(24.249f/PTM_RATIO,-31.0f/PTM_RATIO),b2Vec2(36.249f/PTM_RATIO,-11.785f/PTM_RATIO)}
        
    };
    
    for (int trigIndex=0; trigIndex<3; ++trigIndex) {
        b2PolygonShape dynamicBox;
        dynamicBox.Set(boxArr[trigIndex], 4);
        
        // Define the dynamic body fixture.
        b2FixtureDef fixtureDef;
        fixtureDef.shape = &dynamicBox;	
        fixtureDef.density = desity;
        fixtureDef.friction = Fixture_Friction;
        fixtureDef.restitution = Fixture_Restitution;
        body->CreateFixture(&fixtureDef);
    }
    
    //中间那个三角形
    {
        b2Vec2 trianglArr[] = {
            b2Vec2(-12.0f/PTM_RATIO,0.0f/PTM_RATIO),b2Vec2(0.0f/PTM_RATIO,-17.0f/PTM_RATIO),b2Vec2(12.0f/PTM_RATIO,0.0f/PTM_RATIO)
        };
        b2PolygonShape dynamicBox;
        dynamicBox.Set(trianglArr, 3);
        
        // Define the dynamic body fixture.
        b2FixtureDef fixtureDef;
        fixtureDef.shape = &dynamicBox;	
        fixtureDef.density = desity;
        fixtureDef.friction = Fixture_Friction;
        fixtureDef.restitution = Fixture_Restitution;
        body->CreateFixture(&fixtureDef);
    }
    
    if (angle != 0.0f) {
        body->SetTransform(body->GetPosition(), CC_DEGREES_TO_RADIANS(angle));
    }
    
    return body;
}


b2Body* BaseContentLayer::createT72(b2Vec2 p, float32 angle, bool canDestroy, float32 desity){
    //先创建模糊后的纹理
    if (canDestroy) {
        addBluredSpriteToMap(ResManager::getManager()->getSharedFilePath(g_gameinPath+"T72.png"));
    }
    
    
    
    
    CustomeSprite* sprite = new CustomeSprite;
    
    if (canDestroy) {
        sprite->initWithFile(ResManager::getManager()->getSharedFilePath(g_gameinPath+"T72.png").c_str());
        addChild(sprite, 0, Tag_Box_can_Destroied);
    }else {
        sprite->initWithFile(ResManager::getManager()->getSharedFilePath(g_gameinPath+"T72_no.png").c_str());
        //sprite->setColor(ccGRAY);
        addChild(sprite, 0, Tag_Box_cannot_Destroied);
    }sprite->release();
    
    sprite->setPosition(CCPointMake(p.x*PTM_RATIO, p.y*PTM_RATIO));
	// T72创建碎片
	if(USE_MANUAL_FRAGMENT)
	{
		sprite->addFragment_ResMgr("T72_frame1.png",ccp(11,65),0);
		sprite->addFragment_ResMgr("T72_frame2.png",ccp(35,73),0);
		sprite->addFragment_ResMgr("T72_frame3.png",ccp(66,72),0);
		sprite->addFragment_ResMgr("T72_frame4.png",ccp(73,62),0);
		sprite->addFragment_ResMgr("T72_frame5.png",ccp(59,47),0);
		sprite->addFragment_ResMgr("T72_frame6.png",ccp(51,21),0);
		sprite->addFragment_ResMgr("T72_frame7.png",ccp(40,9),0);
		sprite->addFragment_ResMgr("T72_frame8.png",ccp(33,26),0);
		sprite->addFragment_ResMgr("T72_frame9.png",ccp(24,48),0);
		sprite->addFragment_ResMgr("box_middle_24.png",ccp(17,64),0);
		sprite->addFragment_ResMgr("box_middle_24.png",ccp(25,64),0);
		sprite->addFragment_ResMgr("T72_spell_middle1.png",ccp(35,40),0);
		sprite->addFragment_ResMgr("T72_spell_middle2.png",ccp(48,40),0);
		sprite->addFragment_ResMgr("box_middle_24.png",ccp(58,64),0);
		sprite->addFragment_ResMgr("box_middle_24.png",ccp(66,64),0);


	}
	else
	{
		sprite->splitSpriteToFragment();
	}
    // Define the dynamic body.
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(p.x, p.y);
    bodyDef.userData = sprite;
    b2Body *body = mWorld->CreateBody(&bodyDef);
    
    {
        b2PolygonShape box;
        std::vector<b2PolygonShape> boxVec;
        
        box.SetAsBox(0.5f, 0.5f, b2Vec2(0.0f, 0.0f), 0);
        boxVec.insert(boxVec.end(), box);
        box.SetAsBox(0.5f, 0.5f, b2Vec2(0.0f, 1.0f), 0);
        boxVec.insert(boxVec.end(), box);
        box.SetAsBox(0.5f, 0.5f, b2Vec2(0.0f, -1.0f), 0);
        boxVec.insert(boxVec.end(), box);
        box.SetAsBox(0.5f, 0.5f, b2Vec2(-1.0f, 1.0f), 0);
        boxVec.insert(boxVec.end(), box);
        box.SetAsBox(0.5f, 0.5f, b2Vec2(1.0f, 1.0f), 0);
        boxVec.insert(boxVec.end(), box);
        
        for (size_t i=0; i<boxVec.size(); ++i) {
            b2FixtureDef fixtureDef;
            fixtureDef.shape = &boxVec[i];	
            fixtureDef.density = desity;
            fixtureDef.friction = Fixture_Friction;
            fixtureDef.restitution = Fixture_Restitution;
            
            body->CreateFixture(&fixtureDef);
        }
    }
    
    if (angle != 0.0f) {
        body->SetTransform(body->GetPosition(), CC_DEGREES_TO_RADIANS(angle));
    }
    
    return body;
}

b2Body* BaseContentLayer::createT48(b2Vec2 p, float32 angle, bool canDestroy, float32 desity){
    //先创建模糊后的纹理
    if (canDestroy) {
        addBluredSpriteToMap(ResManager::getManager()->getSharedFilePath(g_gameinPath+"T48.png"));
    }
    
    
    
    
    CustomeSprite* sprite = new CustomeSprite;
    
    if (canDestroy) {
        sprite->initWithFile(ResManager::getManager()->getSharedFilePath(g_gameinPath+"T48.png").c_str());
        addChild(sprite, 0, Tag_Box_can_Destroied);
    }else {
        sprite->initWithFile(ResManager::getManager()->getSharedFilePath(g_gameinPath+"T48_no.png").c_str());
        //sprite->setColor(ccGRAY);
        addChild(sprite, 0, Tag_Box_cannot_Destroied);
    }sprite->release();
    
    sprite->setPosition(CCPointMake(p.x*PTM_RATIO, p.y*PTM_RATIO));
	// T48创建碎片
	if(USE_MANUAL_FRAGMENT)
	{
		sprite->addFragment_ResMgr("T48_frame1.png",ccp(10,37),0);
		sprite->addFragment_ResMgr("T48_frame2.png",ccp(31,49),0);
		sprite->addFragment_ResMgr("T48_frame3.png",ccp(67,48),0);
		sprite->addFragment_ResMgr("T48_frame4.png",ccp(73,37),0);
		sprite->addFragment_ResMgr("T48_frame5.png",ccp(60,22),0);
		sprite->addFragment_ResMgr("T48_frame6.png",ccp(44,9),0);
		sprite->addFragment_ResMgr("T48_frame7.png",ccp(24,20),0);
		sprite->addFragment_ResMgr("box_middle_24.png",ccp(17,40),0);
		sprite->addFragment_ResMgr("box_middle_24.png",ccp(26,40),0);
		sprite->addFragment_ResMgr("T48_spell_middle1.png",ccp(34,29),0);
		sprite->addFragment_ResMgr("T48_spell_middle2.png",ccp(43,29),0);
		sprite->addFragment_ResMgr("T48_spell_middle3.png",ccp(52,29),0);
		sprite->addFragment_ResMgr("box_middle_24.png",ccp(61,40),0);
		sprite->addFragment_ResMgr("box_middle_24.png",ccp(69,40),0);
	}
	else
	{
		sprite->splitSpriteToFragment();
	}
    // Define the dynamic body.
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(p.x, p.y);
    bodyDef.userData = sprite;
    b2Body *body = mWorld->CreateBody(&bodyDef);
    
    {
        b2PolygonShape box;
        std::vector<b2PolygonShape> boxVec;
        
        box.SetAsBox(0.5f, 0.5f, b2Vec2(0.0f, 0.5f), 0);
        boxVec.insert(boxVec.end(), box);
        box.SetAsBox(0.5f, 0.5f, b2Vec2(0.0f, -0.5f), 0);
        boxVec.insert(boxVec.end(), box);
        box.SetAsBox(0.5f, 0.5f, b2Vec2(1.0f, 0.5f), 0);
        boxVec.insert(boxVec.end(), box);
        box.SetAsBox(0.5f, 0.5f, b2Vec2(-1.0f, 0.5f), 0);
        boxVec.insert(boxVec.end(), box);
        
        for (size_t i=0; i<boxVec.size(); ++i) {
            b2FixtureDef fixtureDef;
            fixtureDef.shape = &boxVec[i];	
            fixtureDef.density = desity;
            fixtureDef.friction = Fixture_Friction;
            fixtureDef.restitution = Fixture_Restitution;
            
            body->CreateFixture(&fixtureDef);
        }
    }
    
    if (angle != 0.0f) {
        body->SetTransform(body->GetPosition(), CC_DEGREES_TO_RADIANS(angle));
    }
    
    return body;
}

b2Body* BaseContentLayer::createL48(b2Vec2 p, float32 angle, bool canDestroy, float32 desity){
    //先创建模糊后的纹理
    if (canDestroy) {
        addBluredSpriteToMap(ResManager::getManager()->getSharedFilePath(g_gameinPath+"L48.png"));
    }
    
    
    
    
    CustomeSprite* sprite = new CustomeSprite;
    
    if (canDestroy) {
        sprite->initWithFile(ResManager::getManager()->getSharedFilePath(g_gameinPath+"L48.png").c_str());
        addChild(sprite, 0, Tag_Box_can_Destroied);
    }else {
        sprite->initWithFile(ResManager::getManager()->getSharedFilePath(g_gameinPath+"L48_no.png").c_str());
        //sprite->setColor(ccGRAY);
        addChild(sprite, 0, Tag_Box_cannot_Destroied);
    }sprite->release();
    
    sprite->setPosition(CCPointMake(p.x*PTM_RATIO, p.y*PTM_RATIO));
    
	// L48创建碎片
	if(USE_MANUAL_FRAGMENT)
	{
		sprite->addFragment_ResMgr("box_frame_24.png",ccp(11,18),0);
		sprite->addFragment_ResMgr("box_frame_24.png",ccp(41,49),90);
		sprite->addFragment_ResMgr("L48_spell_middle1.png",ccp(26,25),0);
		sprite->addFragment_ResMgr("L48_spell_middle2.png",ccp(30,18),0);
		sprite->addFragment_ResMgr("L48_spell_middle3.png",ccp(32,10),0);
		sprite->addFragment_ResMgr("L48_spell_middle4.png",ccp(34,34),0);
		sprite->addFragment_ResMgr("L48_spell_middle5.png",ccp(42,32),0);
		sprite->addFragment_ResMgr("L48_spell_middle6.png",ccp(48,28),0);
	}
	else
	{
		sprite->splitSpriteToFragment();
	}
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(p.x, p.y);
    bodyDef.userData = sprite;
    b2Body* bodyA = mWorld->CreateBody(&bodyDef);
    
    {
        b2PolygonShape box;
        std::vector<b2PolygonShape> boxVec;
        
        box.SetAsBox(0.5f, 0.5f, b2Vec2(0.5f, 0.5f), 0);
        boxVec.insert(boxVec.end(), box);
        box.SetAsBox(0.5f, 0.5f, b2Vec2(0.5f, -0.5f), 0);
        boxVec.insert(boxVec.end(), box);
        box.SetAsBox(0.5f, 0.5f, b2Vec2(-0.5f, -0.5f), 0);
        boxVec.insert(boxVec.end(), box);
        
        for (size_t i=0; i<boxVec.size(); ++i) {
            b2FixtureDef fixtureDef;
            fixtureDef.shape = &boxVec[i];	
            fixtureDef.density = desity;
            fixtureDef.friction = Fixture_Friction;
            fixtureDef.restitution = Fixture_Restitution;
            
            bodyA->CreateFixture(&fixtureDef);
        }
    }
    
    if (angle != 0.0f) {
        bodyA->SetTransform(bodyA->GetPosition(), CC_DEGREES_TO_RADIANS(angle));
    }
    
    
    return bodyA;
}

b2Body* BaseContentLayer::createL72_48(b2Vec2 p, float32 angle, bool canDestroy, float32 desity){
    //先创建模糊后的纹理
    if (canDestroy) {
        addBluredSpriteToMap(ResManager::getManager()->getSharedFilePath(g_gameinPath+"L72_48.png"));
    }
    
    
    
    
    CustomeSprite* sprite = new CustomeSprite;
    
    if (canDestroy) {
        sprite->initWithFile(ResManager::getManager()->getSharedFilePath(g_gameinPath+"L72_48.png").c_str());
        addChild(sprite, 0, Tag_Box_can_Destroied);
    }else {
        sprite->initWithFile(ResManager::getManager()->getSharedFilePath(g_gameinPath+"L72_48_no.png").c_str());
        //sprite->setColor(ccGRAY);
        addChild(sprite, 0, Tag_Box_cannot_Destroied);
    }sprite->release();
    
    sprite->setPosition(CCPointMake(p.x*PTM_RATIO, p.y*PTM_RATIO));
	// L72_48创建碎片
	if(USE_MANUAL_FRAGMENT)
	{
		sprite->addFragment_ResMgr("box_frame_24.png",ccp(11,18),0);
		sprite->addFragment_ResMgr("box_frame_24.png",ccp(41,74),90);
		sprite->addFragment_ResMgr("L72_48_frame1.png",ccp(24,26),0);
		sprite->addFragment_ResMgr("L72_48_frame2.png",ccp(33,49),90);
		sprite->addFragment_ResMgr("L72_48_frame2.png",ccp(49,50),-90);
		sprite->addFragment_ResMgr("L72_48_frame1.png",ccp(50,22),90);
		sprite->addFragment_ResMgr("L72_48_frame2.png",ccp(33,10),0);
		sprite->addFragment_ResMgr("L72_48_spell_middle1.png",ccp(28,22),0);
		sprite->addFragment_ResMgr("L72_48_spell_middle2.png",ccp(38,30),0);
		sprite->addFragment_ResMgr("L72_48_spell_middle3.png",ccp(38,50),0);
		sprite->addFragment_ResMgr("L72_48_spell_middle4.png",ccp(44,58),0);
		sprite->addFragment_ResMgr("L72_48_spell_middle5.png",ccp(44,30),-180);
		sprite->addFragment_ResMgr("L72_48_spell_middle6.png",ccp(30,14),0);
	}
	else
	{
		sprite->splitSpriteToFragment();
	}
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(p.x, p.y);
    bodyDef.userData = sprite;
    b2Body* bodyA = mWorld->CreateBody(&bodyDef);
    
    {
        b2PolygonShape box;
        std::vector<b2PolygonShape> boxVec;
        
        box.SetAsBox(0.5f, 0.5f, b2Vec2(0.5f, 0.0f), 0);
        boxVec.insert(boxVec.end(), box);
        box.SetAsBox(0.5f, 0.5f, b2Vec2(0.5f, 1.0f), 0);
        boxVec.insert(boxVec.end(), box);
        box.SetAsBox(0.5f, 0.5f, b2Vec2(0.5f, -1.0f), 0);
        boxVec.insert(boxVec.end(), box);
        box.SetAsBox(0.5f, 0.5f, b2Vec2(-0.5f, -1.0f), 0);
        boxVec.insert(boxVec.end(), box);
        
        for (size_t i=0; i<boxVec.size(); ++i) {
            b2FixtureDef fixtureDef;
            fixtureDef.shape = &boxVec[i];	
            fixtureDef.density = desity;
            fixtureDef.friction = Fixture_Friction;
            fixtureDef.restitution = Fixture_Restitution;
            
            bodyA->CreateFixture(&fixtureDef);
        }
    }
    
    if (angle != 0.0f) {
        bodyA->SetTransform(bodyA->GetPosition(), CC_DEGREES_TO_RADIANS(angle));
    }
    
    
    return bodyA;
}

b2Body* BaseContentLayer::createL72_72(b2Vec2 p, float32 angle, bool canDestroy, float32 desity){
    //先创建模糊后的纹理
    if (canDestroy) {
        addBluredSpriteToMap(ResManager::getManager()->getSharedFilePath(g_gameinPath+"L72_72.png"));
    }
    
    
    
    
    CustomeSprite* sprite = new CustomeSprite;
    
    if (canDestroy) {
        sprite->initWithFile(ResManager::getManager()->getSharedFilePath(g_gameinPath+"L72_72.png").c_str());
        addChild(sprite, 0, Tag_Box_can_Destroied);
    }else {
        sprite->initWithFile(ResManager::getManager()->getSharedFilePath(g_gameinPath+"L72_72_no.png").c_str());
        //sprite->setColor(ccGRAY);
        addChild(sprite, 0, Tag_Box_cannot_Destroied);
    }sprite->release();
    
    sprite->setPosition(CCPointMake(p.x*PTM_RATIO, p.y*PTM_RATIO));
	// L72_72创建碎片
	if(USE_MANUAL_FRAGMENT)
	{
		sprite->addFragment_ResMgr("box_frame_24.png",ccp(11,18),0);
		sprite->addFragment_ResMgr("box_frame_24.png",ccp(66,72),90);
		sprite->addFragment_ResMgr("L72_48_frame2.png",ccp(37,27),0);
		sprite->addFragment_ResMgr("L72_48_frame2.png",ccp(58,48),90);
		sprite->addFragment_ResMgr("L72_48_frame1.png",ccp(74,60),90);
		sprite->addFragment_ResMgr("L72_48_frame2.png",ccp(74,30),90);
		sprite->addFragment_ResMgr("L72_48_frame1.png",ccp(65,8),0);
		sprite->addFragment_ResMgr("L72_48_frame2.png",ccp(35,8),0);

		sprite->addFragment_ResMgr("L72_48_spell_middle6.png",ccp(30,21),0);
		sprite->addFragment_ResMgr("L72_48_spell_middle1.png",ccp(53,22),180);
		sprite->addFragment_ResMgr("L72_48_spell_middle2.png",ccp(63,28),0);
		sprite->addFragment_ResMgr("L72_48_spell_middle3.png",ccp(63,52),0);
		sprite->addFragment_ResMgr("L72_48_spell_middle4.png",ccp(70,60),0);
		sprite->addFragment_ResMgr("L72_48_spell_middle5.png",ccp(70,26),0);
		sprite->addFragment_ResMgr("L72_48_spell_middle1.png",ccp(54,14),180);
		sprite->addFragment_ResMgr("L72_48_spell_middle1.png",ccp(30,14),0);
	}
	else
	{
		sprite->splitSpriteToFragment();
	}
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(p.x, p.y);
    bodyDef.userData = sprite;
    b2Body* bodyA = mWorld->CreateBody(&bodyDef);
    
    {
        b2PolygonShape box;
        std::vector<b2PolygonShape> boxVec;
        
        box.SetAsBox(0.5f, 0.5f, b2Vec2(1.0f, 0.0f), 0);
        boxVec.insert(boxVec.end(), box);
        box.SetAsBox(0.5f, 0.5f, b2Vec2(1.0f, 1.0f), 0);
        boxVec.insert(boxVec.end(), box);
        box.SetAsBox(0.5f, 0.5f, b2Vec2(1.0f, -1.0f), 0);
        boxVec.insert(boxVec.end(), box);
        box.SetAsBox(0.5f, 0.5f, b2Vec2(0.0f, -1.0f), 0);
        boxVec.insert(boxVec.end(), box);
        box.SetAsBox(0.5f, 0.5f, b2Vec2(-1.0f, -1.0f), 0);
        boxVec.insert(boxVec.end(), box);
        
        for (size_t i=0; i<boxVec.size(); ++i) {
            b2FixtureDef fixtureDef;
            fixtureDef.shape = &boxVec[i];	
            fixtureDef.density = desity;
            fixtureDef.friction = Fixture_Friction;
            fixtureDef.restitution = Fixture_Restitution;
            
            bodyA->CreateFixture(&fixtureDef);
        }
    }
    
    if (angle != 0.0f) {
        bodyA->SetTransform(bodyA->GetPosition(), CC_DEGREES_TO_RADIANS(angle));
    }
    
    
    return bodyA;
}
b2Body* BaseContentLayer::createCircle48(b2Vec2 p, bool canDestroy, float32 restitution, float32 desity){
    //先创建模糊后的纹理
    if (canDestroy) {
        addBluredSpriteToMap(ResManager::getManager()->getSharedFilePath(g_gameinPath+"circle48.png"));
    }
    CustomeSprite* sprite = new CustomeSprite;
    
    if (canDestroy) {
        sprite->initWithFile(ResManager::getManager()->getSharedFilePath(g_gameinPath+"circle48.png").c_str());
        addChild(sprite, 0, Tag_Box_can_Destroied);
    }else {
        sprite->initWithFile(ResManager::getManager()->getSharedFilePath(g_gameinPath+"circle48_no.png").c_str());
        //sprite->setColor(ccGRAY);
        addChild(sprite, 0, Tag_Box_cannot_Destroied);
    }sprite->release();
    
    sprite->setPosition(CCPointMake(p.x*PTM_RATIO, p.y*PTM_RATIO));
    
	// circle48创建碎片
	if(USE_MANUAL_FRAGMENT)
	{
		sprite->addFragment_ResMgr("circle48_frame1.png",ccp(18,33),0);
		sprite->addFragment_ResMgr("circle48_frame2.png",ccp(42,33),0);
		sprite->addFragment_ResMgr("circle48_frame3.png",ccp(32,11),0);
		sprite->addFragment_ResMgr("circle48_spell_middle1.png",ccp(16,29),0);
		sprite->addFragment_ResMgr("circle48_spell_middle2.png",ccp(26,29),0);
		sprite->addFragment_ResMgr("circle48_spell_middle2.png",ccp(36,29),180);
		sprite->addFragment_ResMgr("circle48_spell_middle1.png",ccp(46,29),180);
	}
	else
	{
		sprite->splitSpriteToFragment();
	}
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(p.x, p.y);
    bodyDef.userData = sprite;
    b2Body* bodyA = mWorld->CreateBody(&bodyDef);
  
    b2CircleShape circle;
    circle.m_radius = 1.0f;

    // Define the dynamic body fixture.
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &circle;	
    fixtureDef.density = desity;
    fixtureDef.friction = Fixture_Friction;
    fixtureDef.restitution = restitution;
    bodyA->CreateFixture(&fixtureDef);
    
    return bodyA;
}

b2Body* BaseContentLayer::createCircle24(b2Vec2 p, bool canDestroy, float32 restitution, float32 desity){
    //先创建模糊后的纹理
    if (canDestroy) {
        if (restitution > 0) {
            addBluredSpriteToMap(ResManager::getManager()->getSharedFilePath(g_gameinPath+"marbles.png"));
        }else{
            addBluredSpriteToMap(ResManager::getManager()->getSharedFilePath(g_gameinPath+"circle24.png"));
        }
    }
    
    CustomeSprite* sprite = new CustomeSprite;
    
    if (canDestroy) {
        if (restitution > 0)
        {
            sprite->initWithFile(ResManager::getManager()->getSharedFilePath(g_gameinPath+"marbles.png").c_str());
			
			AchievementManager::getInstance()->createdOneRestitutionBall24();

        }else
        {
            sprite->initWithFile(ResManager::getManager()->getSharedFilePath(g_gameinPath+"circle24.png").c_str());
        }
        
        addChild(sprite, 0, Tag_Box_can_Destroied);
    }else {
        if (restitution > 0)
        {
            sprite->initWithFile(ResManager::getManager()->getSharedFilePath(g_gameinPath+"marbles_no.png").c_str());
        }else
        {
            sprite->initWithFile(ResManager::getManager()->getSharedFilePath(g_gameinPath+"circle24_no.png").c_str());
        }
        addChild(sprite, 0, Tag_Box_cannot_Destroied);
    }sprite->release();
    
    sprite->setPosition(CCPointMake(p.x*PTM_RATIO, p.y*PTM_RATIO));
    
	if(USE_MANUAL_FRAGMENT)
	{
		if(restitution > 0)
		{
			// marbles创建碎片
			{
				sprite->addFragment_ResMgr("marbles_spell_1.png",ccp(11,25),0);
				sprite->addFragment_ResMgr("marbles_spell_2.png",ccp(25,26),0);
				sprite->addFragment_ResMgr("marbles_spell_3.png",ccp(28,12),0);
				sprite->addFragment_ResMgr("marbles_spell_4.png",ccp(18,7),0);
				sprite->addFragment_ResMgr("marbles_spell_5.png",ccp(9,12),0);
				sprite->addFragment_ResMgr("marbles_spell_6.png",ccp(19,17),0);
			}
		}else
		{
			// circle24创建碎片
			{
				sprite->addFragment_ResMgr("circle24_frame1.png",ccp(12,17),0);
				sprite->addFragment_ResMgr("circle24_frame2.png",ccp(25,17),0);
				sprite->addFragment_ResMgr("circle24_spell_middle1.png",ccp(13,17),0);
				sprite->addFragment_ResMgr("circle24_spell_middle2.png",ccp(17,17),0);
				sprite->addFragment_ResMgr("circle24_spell_middle3.png",ccp(22,17),0);
			}
		}
	}
	else
	{
		sprite->splitSpriteToFragment();
	}
    
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(p.x, p.y);
    bodyDef.userData = sprite;
    b2Body* bodyA = mWorld->CreateBody(&bodyDef);
    b2CircleShape circle;
    circle.m_radius = 0.5f;
    
    // Define the dynamic body fixture.
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &circle;	
    fixtureDef.density = desity;
    fixtureDef.friction = Fixture_Friction;
    fixtureDef.restitution = restitution;
    bodyA->CreateFixture(&fixtureDef);
    
    return bodyA;
}

b2Body* BaseContentLayer::createCircle72(b2Vec2 p, bool canDestroy, float32 restitution, float32 desity){
    //先创建模糊后的纹理
    if (canDestroy) {
        addBluredSpriteToMap(ResManager::getManager()->getSharedFilePath(g_gameinPath+"circle72.png"));
    }
    
    
    
    
    CustomeSprite* sprite = new CustomeSprite;
    
    if (canDestroy) {
        sprite->initWithFile(ResManager::getManager()->getSharedFilePath(g_gameinPath+"circle72.png").c_str());
        addChild(sprite, 0, Tag_Box_can_Destroied);
    }else {
        sprite->initWithFile(ResManager::getManager()->getSharedFilePath(g_gameinPath+"circle72_no.png").c_str());
        //sprite->setColor(ccGRAY);
        addChild(sprite, 0, Tag_Box_cannot_Destroied);
    }sprite->release();
    
    sprite->setPosition(CCPointMake(p.x*PTM_RATIO, p.y*PTM_RATIO));
	// circle72创建碎片
	if(USE_MANUAL_FRAGMENT)
	{
		sprite->addFragment_ResMgr("circle72_frame1.png",ccp(25,48),0);
		sprite->addFragment_ResMgr("circle72_frame2.png",ccp(62,45),0);
		sprite->addFragment_ResMgr("circle72_frame3.png",ccp(41,13),0);
		sprite->addFragment_ResMgr("circle72_spell_middle1.png",ccp(20,39),0);
		sprite->addFragment_ResMgr("circle72_spell_middle2.png",ccp(30,39),0);
		sprite->addFragment_ResMgr("circle72_spell_middle3.png",ccp(42,40),0);
		sprite->addFragment_ResMgr("circle72_spell_middle4.png",ccp(56,39),0);
		sprite->addFragment_ResMgr("circle72_spell_middle4.png",ccp(66,39),0);

	}
	else
	{
		sprite->splitSpriteToFragment();
	}
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(p.x, p.y);
    bodyDef.userData = sprite;
    b2Body* bodyA = mWorld->CreateBody(&bodyDef);
    
    b2CircleShape circle;
    circle.m_radius = 1.5f;
    
    // Define the dynamic body fixture.
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &circle;	
    fixtureDef.density = desity;
    fixtureDef.friction = Fixture_Friction;
    fixtureDef.restitution = restitution;
    bodyA->CreateFixture(&fixtureDef);
    
    return bodyA;
}

b2Body* BaseContentLayer::createHalfCircle96(b2Vec2 p, bool canDestroy, float32 desity){
    //先创建模糊后的纹理
    if (canDestroy) {
        addBluredSpriteToMap(ResManager::getManager()->getSharedFilePath(g_gameinPath+"halfcircle96.png"));
    }
    
    
    CustomeSprite* sprite = new CustomeSprite;
    
    if (canDestroy) {
        sprite->initWithFile(ResManager::getManager()->getSharedFilePath(g_gameinPath+"halfcircle96.png").c_str());
        addChild(sprite, 0, Tag_Box_can_Destroied);
    }else {
        sprite->initWithFile(ResManager::getManager()->getSharedFilePath(g_gameinPath+"halfcircle96_no.png").c_str());
        //sprite->setColor(ccGRAY);
        addChild(sprite, 0, Tag_Box_cannot_Destroied);
    }sprite->release();
    
    sprite->setPosition(CCPointMake(p.x*PTM_RATIO, p.y*PTM_RATIO));
	// halfcircle96创建碎片
	if(USE_MANUAL_FRAGMENT)
	{
		sprite->addFragment_ResMgr("halfcircle96_frame1.png",ccp(32,34),0);
		sprite->addFragment_ResMgr("halfcircle96_frame2.png",ccp(78,34),0);
		sprite->addFragment_ResMgr("halfcircle96_frame3.png",ccp(32,13),0);
		sprite->addFragment_ResMgr("halfcircle96_frame4.png",ccp(77,13),0);
		sprite->addFragment_ResMgr("halfcircle96_spell_middle1.png",ccp(23,25),0);
		sprite->addFragment_ResMgr("halfcircle96_spell_middle2.png",ccp(37,28),0);
		sprite->addFragment_ResMgr("halfcircle96_spell_middle3.png",ccp(56,30),0);
		sprite->addFragment_ResMgr("halfcircle96_spell_middle4.png",ccp(73,30),0);
		sprite->addFragment_ResMgr("halfcircle96_spell_middle5.png",ccp(86,22),0);
	}
	else
	{
		sprite->splitSpriteToFragment();
	}
    // Define the dynamic body.
    //Set up a 1m squared box in the physics world
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(p.x, p.y);
    bodyDef.userData = sprite;
    b2Body *body = mWorld->CreateBody(&bodyDef);
    
    //
    b2Vec2 trianglArr[][8] = {
        {b2Vec2(0.0f/PTM_RATIO,24.0f/PTM_RATIO),b2Vec2(-10.0f/PTM_RATIO,23.0f/PTM_RATIO),b2Vec2(-21.0f/PTM_RATIO,20.0f/PTM_RATIO),b2Vec2(-32.0f/PTM_RATIO,12.0f/PTM_RATIO),b2Vec2(-40.0f/PTM_RATIO,2.0f/PTM_RATIO),b2Vec2(-46.0f/PTM_RATIO,-11.0f/PTM_RATIO),b2Vec2(-48.0f/PTM_RATIO,-24.0f/PTM_RATIO),b2Vec2(0.0f/PTM_RATIO,-24.0f/PTM_RATIO)},
        {b2Vec2(0.0f/PTM_RATIO,24.0f/PTM_RATIO),b2Vec2(0.0f/PTM_RATIO,-24.0f/PTM_RATIO),b2Vec2(48.0f/PTM_RATIO,-24.0f/PTM_RATIO),b2Vec2(46.0f/PTM_RATIO,-11.0f/PTM_RATIO),b2Vec2(40.0f/PTM_RATIO,2.0f/PTM_RATIO),b2Vec2(32.0f/PTM_RATIO,12.0f/PTM_RATIO),b2Vec2(21.0f/PTM_RATIO,20.0f/PTM_RATIO),b2Vec2(10.0f/PTM_RATIO,23.0f/PTM_RATIO)}
    };
    
    for (int trigIndex=0; trigIndex<2; ++trigIndex) {
        b2PolygonShape dynamicBox;
        dynamicBox.Set(trianglArr[trigIndex], 8);
        
        // Define the dynamic body fixture.
        b2FixtureDef fixtureDef;
        fixtureDef.shape = &dynamicBox;	
        fixtureDef.density = desity;
        fixtureDef.friction = Fixture_Friction;
        fixtureDef.restitution = Fixture_Restitution;
        body->CreateFixture(&fixtureDef);
    }
    
    return body;
}



b2Body* BaseContentLayer::createHalfCircle48(b2Vec2 p, bool canDestroy, float32 desity){
    //先创建模糊后的纹理
    if (canDestroy) {
        addBluredSpriteToMap(ResManager::getManager()->getSharedFilePath(g_gameinPath+"halfcircle48.png"));
    }
    
    
    CustomeSprite* sprite = new CustomeSprite;
    
    if (canDestroy) {
        sprite->initWithFile(ResManager::getManager()->getSharedFilePath(g_gameinPath+"halfcircle48.png").c_str());
        addChild(sprite, 0, Tag_Box_can_Destroied);
    }else {
        sprite->initWithFile(ResManager::getManager()->getSharedFilePath(g_gameinPath+"halfcircle48_no.png").c_str());
        //sprite->setColor(ccGRAY);
        addChild(sprite, 0, Tag_Box_cannot_Destroied);
    }sprite->release();
    
    sprite->setPosition(CCPointMake(p.x*PTM_RATIO, p.y*PTM_RATIO));
    
	// halfcircle48创建碎片
	if(USE_MANUAL_FRAGMENT)
	{
		sprite->addFragment_ResMgr("halfcircle48_frame1.png",ccp(18,19),0);
		sprite->addFragment_ResMgr("halfcircle48_frame2.png",ccp(43,19),0);
		sprite->addFragment_ResMgr("halfcircle48_frame3.png",ccp(31,8),0);
		sprite->addFragment_ResMgr("halfcircle48_spell_middle1.png",ccp(15,16),0);
		sprite->addFragment_ResMgr("halfcircle48_spell_middle2.png",ccp(26,18),0);
		sprite->addFragment_ResMgr("halfcircle48_spell_middle3.png",ccp(37,18),0);
		sprite->addFragment_ResMgr("halfcircle48_spell_middle4.png",ccp(46,15),0);
	}
	else
	{
		sprite->splitSpriteToFragment();
	}
    // Define the dynamic body.
    //Set up a 1m squared box in the physics world
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(p.x, p.y);
    bodyDef.userData = sprite;
    b2Body *body = mWorld->CreateBody(&bodyDef);
    
    //
    b2Vec2 trianglArr[][8] = {
        {b2Vec2(0.0f/PTM_RATIO,12.0f/PTM_RATIO),b2Vec2(-5.0f/PTM_RATIO,11.5f/PTM_RATIO),b2Vec2(-10.5f/PTM_RATIO,10.0f/PTM_RATIO),b2Vec2(-16.0f/PTM_RATIO,6.0f/PTM_RATIO),b2Vec2(-20.0f/PTM_RATIO,1.0f/PTM_RATIO),b2Vec2(-23.0f/PTM_RATIO,-5.5f/PTM_RATIO),b2Vec2(-24.0f/PTM_RATIO,-12.0f/PTM_RATIO),b2Vec2(0.0f/PTM_RATIO,-12.0f/PTM_RATIO)},
        {b2Vec2(0.0f/PTM_RATIO,12.0f/PTM_RATIO),b2Vec2(0.0f/PTM_RATIO,-12.0f/PTM_RATIO),b2Vec2(12.0f/PTM_RATIO,-12.0f/PTM_RATIO),b2Vec2(23.0f/PTM_RATIO,-5.5f/PTM_RATIO),b2Vec2(20.0f/PTM_RATIO,1.0f/PTM_RATIO),b2Vec2(16.0f/PTM_RATIO,6.0f/PTM_RATIO),b2Vec2(10.5f/PTM_RATIO,10.0f/PTM_RATIO),b2Vec2(5.0f/PTM_RATIO,11.5f/PTM_RATIO)}
    };
    
    for (int trigIndex=0; trigIndex<2; ++trigIndex) {
        b2PolygonShape dynamicBox;
        dynamicBox.Set(trianglArr[trigIndex], 8);
        
        // Define the dynamic body fixture.
        b2FixtureDef fixtureDef;
        fixtureDef.shape = &dynamicBox;	
        fixtureDef.density = desity;
        fixtureDef.friction = Fixture_Friction;
        fixtureDef.restitution = Fixture_Restitution;
        body->CreateFixture(&fixtureDef);
    }
    
    return body;
}

b2Body* BaseContentLayer::createTimeAscBomb(b2Vec2 p, float32 angle, float32 desity){
    CustomeSprite* sprite = new CustomeSprite;
    sprite->initWithFile(ResManager::getManager()->getSharedFilePath(g_gameinPath+"timeras.png").c_str());
    
    addChild(sprite, 0, Tag_Timer_Asc_Bomb);
    sprite->setPosition(CCPointMake(p.x*PTM_RATIO, p.y*PTM_RATIO));
    sprite->release();
    
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(p.x, p.y);
    bodyDef.userData = sprite;
    b2Body* bodyA = mWorld->CreateBody(&bodyDef);
    
	{
		b2PolygonShape box;
		std::vector<b2PolygonShape> boxVec;
        
		box.SetAsBox(0.5f, 0.5f, b2Vec2(0.5f, 0.5f), 0);
		boxVec.insert(boxVec.end(), box);
		box.SetAsBox(0.5f, 0.5f, b2Vec2(-0.5f, 0.5f), 0);
		boxVec.insert(boxVec.end(), box);
		box.SetAsBox(0.5f, 0.5f, b2Vec2(-0.5f, -0.5f), 0);
		boxVec.insert(boxVec.end(), box);
		box.SetAsBox(0.5f, 0.5f, b2Vec2(0.5f, -0.5f), 0);
		boxVec.insert(boxVec.end(), box);
        
		for (size_t i=0; i<boxVec.size(); ++i) {
			b2FixtureDef fixtureDef;
			fixtureDef.shape = &boxVec[i];
			fixtureDef.density = desity;
			fixtureDef.friction = Fixture_Friction;
			fixtureDef.restitution = Fixture_Restitution;
            
			bodyA->CreateFixture(&fixtureDef);
		}
	}
    
    if (angle != 0.0f) {
        bodyA->SetTransform(bodyA->GetPosition(), CC_DEGREES_TO_RADIANS(angle));
    }
    
    return bodyA;
}
#define PI 3.1415926f
void BaseContentLayer::particleCallBack(CCNode* par)
{
	CCParticleSystem* par1 = (CCParticleSystem*)par;
	// 在原来的基础上旋转 x 度
	//CCPoint newGravity = ccpRotateByAngle(ccpSub(par1->getGravity(),ccp(0,10)),ccp(1,0),CC_DEGREES_TO_RADIANS(2.5f));
	//par1->setGravity(ccpAdd(newGravity,ccp(0,10)));//整体向上偏移

	CCPoint newGravity = ccpRotateByAngle(par1->getGravity(),ccp(1,0),CC_DEGREES_TO_RADIANS(2.5f));
	par1->setGravity(newGravity);//整体向上偏移

	float center_x = 24;
	float center_y = 23;
	float raduis = 8;

	float g_angle = ccpAngle(newGravity,ccp(1,0));//重力相对于X轴的角度
	if (newGravity.y < 0)
	{//修正
		g_angle = -g_angle;
	}

	float par_angle_raduis = PI/2.0f + g_angle;
	//float g_angle_degree = CC_RADIANS_TO_DEGREES(g_angle);
	//float par_angle_degree = CC_RADIANS_TO_DEGREES(par_angle_raduis);
	//CCLog("g_angle = %f",g_angle_degree);
	//CCLog("par_angle_raduis = %f",par_angle_degree);

	par1->setPosition(	center_x + cos(par_angle_raduis)*raduis,
		center_y + sin(par_angle_raduis)*raduis);

}
b2Body* BaseContentLayer::createTimeDesBomb(b2Vec2 p, float32 angle, float32 desity){
    CustomeSprite* sprite = new CustomeSprite;
    sprite->initWithFile(ResManager::getManager()->getSharedFilePath(g_gameinPath+"timerdec.png").c_str());
    addChild(sprite, 0, Tag_Timer_Dec_Bomb);
    sprite->setPosition(CCPointMake(p.x*PTM_RATIO, p.y*PTM_RATIO));
    sprite->release();

	TimerDec* timerDec = TimerDec::createTimerDec();
	timerDec->setPosition(ccp(sprite->getContentSize().width/2, sprite->getContentSize().height/2));
	sprite->addChild(timerDec);
	timerDec->setVisible(false);

	AchievementManager::getInstance()->createdATimeDecBomb();

	{
//		CustomeSprite* fog_cover = CustomeSprite::create(ResManager::getManager()->getSharedFilePath(g_gameinPath+"timerdec_cover.png").c_str());
//		CustomeSprite* fog_cover2 = CustomeSprite::create(ResManager::getManager()->getSharedFilePath(g_gameinPath+"timerdec.png").c_str());
		CCSprite* fog_cover = CCSprite::createWithSpriteFrameName("timerdec_cover.png");
		CCSprite* fog_cover2 = CCSprite::createWithSpriteFrameName("timerdec.png");
		sprite->addChild(fog_cover,13);
		sprite->addChild(fog_cover2,12);
		fog_cover2->setPosition(ccp(sprite->getContentSize().width/2,sprite->getContentSize().height/2));
		fog_cover->setPosition(ccp(sprite->getContentSize().width/2,sprite->getContentSize().height/2));
		fog_cover->runAction(CCRepeatForever::create(CCRotateBy::create(0.04f, 1)));
		createFogParticle(fog_cover,ResManager::getManager()->getSharedFilePath(g_gameinPath+"fire.png").c_str());
	}
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(p.x, p.y);
    bodyDef.userData = sprite;
    b2Body* bodyA = mWorld->CreateBody(&bodyDef);
    
	{
		b2PolygonShape box;
		std::vector<b2PolygonShape> boxVec;
        
		box.SetAsBox(0.5f, 0.5f, b2Vec2(0.5f, 0.5f), 0);
		boxVec.insert(boxVec.end(), box);
		box.SetAsBox(0.5f, 0.5f, b2Vec2(-0.5f, 0.5f), 0);
		boxVec.insert(boxVec.end(), box);
		box.SetAsBox(0.5f, 0.5f, b2Vec2(-0.5f, -0.5f), 0);
		boxVec.insert(boxVec.end(), box);
		box.SetAsBox(0.5f, 0.5f, b2Vec2(0.5f, -0.5f), 0);
		boxVec.insert(boxVec.end(), box);
        
		for (size_t i=0; i<boxVec.size(); ++i) {
			b2FixtureDef fixtureDef;
			fixtureDef.shape = &boxVec[i];
			fixtureDef.density = desity;
			fixtureDef.friction = Fixture_Friction;
			fixtureDef.restitution = Fixture_Restitution;
            
			bodyA->CreateFixture(&fixtureDef);
		}
	}
    
    if (angle != 0.0f) {
        bodyA->SetTransform(bodyA->GetPosition(), CC_DEGREES_TO_RADIANS(angle));
    }
    
    return bodyA;
}

b2Body* BaseContentLayer::createTimeDesBomb24(b2Vec2 p, float32 angle, float32 desity){
    CustomeSprite* sprite = new CustomeSprite;
    sprite->initWithFile(ResManager::getManager()->getSharedFilePath(g_gameinPath+"timerdec.png").c_str());
    addChild(sprite, 0, Tag_Timer_Dec_Bomb);
    sprite->setPosition(CCPointMake(p.x*PTM_RATIO, p.y*PTM_RATIO));
    sprite->release();
    sprite->setScale(0.5f);

	TimerDec* timerDec = TimerDec::createTimerDec(ResManager::getManager()->getSharedFilePath(g_gameinPath+"Frost_ward_small.png").c_str());
	timerDec->setPosition(ccp(sprite->getContentSize().width/2, sprite->getContentSize().height/2));
	sprite->addChild(timerDec);
	timerDec->setVisible(false);

	AchievementManager::getInstance()->createdATimeDecBomb();

	{
//		CustomeSprite* fog_cover = CustomeSprite::create(ResManager::getManager()->getSharedFilePath(g_gameinPath+"timerdec_cover.png").c_str());
//		CustomeSprite* fog_cover2 = CustomeSprite::create(ResManager::getManager()->getSharedFilePath(g_gameinPath+"timerdec.png").c_str());
		CCSprite* fog_cover = CCSprite::createWithSpriteFrameName("timerdec_cover.png");
		CCSprite* fog_cover2 = CCSprite::createWithSpriteFrameName("timerdec.png");
		sprite->addChild(fog_cover,13);
		sprite->addChild(fog_cover2,12);
		fog_cover->runAction(CCRepeatForever::create(CCRotateBy::create(0.04f, 1)));
		fog_cover2->setPosition(ccp(sprite->getContentSize().width/2,sprite->getContentSize().height/2));
		fog_cover->setPosition(ccp(sprite->getContentSize().width/2,sprite->getContentSize().height/2));
		createFog24Particle(fog_cover,ResManager::getManager()->getSharedFilePath(g_gameinPath+"fire.png").c_str());
	}

    ShadowLayer::sharedShadowLayer()->createShadow(sprite);
    
    
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(p.x, p.y);
    bodyDef.userData = sprite;
    b2Body* bodyA = mWorld->CreateBody(&bodyDef);
    
    b2PolygonShape box;
    box.SetAsBox(0.5f, 0.5f);
    
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &box;
    fixtureDef.density = desity;
    fixtureDef.friction = Fixture_Friction;
    fixtureDef.restitution = Fixture_Restitution;
    
    bodyA->CreateFixture(&fixtureDef);
    if (angle != 0.0f) {
        bodyA->SetTransform(bodyA->GetPosition(), CC_DEGREES_TO_RADIANS(angle));
    }
    
    return bodyA;
}

b2Body* BaseContentLayer::createBomb48(b2Vec2 p, float32 angle, float32 desity){
    CustomeSprite* sprite = new CustomeSprite;
    sprite->initWithFile(ResManager::getManager()->getSharedFilePath(g_gameinPath+"bomb48.png").c_str());
    addChild(sprite, 0, Tag_Box_Bomb);
    sprite->release();
    sprite->setPosition(CCPointMake(p.x*PTM_RATIO, p.y*PTM_RATIO));
    lastCreateBombPos = sprite->getPosition();
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(p.x, p.y);
    bodyDef.userData = sprite;
    b2Body* bodyA = mWorld->CreateBody(&bodyDef);
	{
		b2PolygonShape box;
		std::vector<b2PolygonShape> boxVec;

		box.SetAsBox(0.5f, 0.5f, b2Vec2(0.5f, 0.5f), 0);
		boxVec.insert(boxVec.end(), box);
		box.SetAsBox(0.5f, 0.5f, b2Vec2(-0.5f, 0.5f), 0);
		boxVec.insert(boxVec.end(), box);
		box.SetAsBox(0.5f, 0.5f, b2Vec2(-0.5f, -0.5f), 0);
		boxVec.insert(boxVec.end(), box);
		box.SetAsBox(0.5f, 0.5f, b2Vec2(0.5f, -0.5f), 0);
		boxVec.insert(boxVec.end(), box);

		for (size_t i=0; i<boxVec.size(); ++i) {
			b2FixtureDef fixtureDef;
			fixtureDef.shape = &boxVec[i];	
			fixtureDef.density = desity;
			fixtureDef.friction = Fixture_Friction;
			fixtureDef.restitution = Fixture_Restitution;

			bodyA->CreateFixture(&fixtureDef);
		}
	}

    if (angle != 0.0f) {
        bodyA->SetTransform(bodyA->GetPosition(), CC_DEGREES_TO_RADIANS(angle));
    }
    
    return bodyA;
}

b2Body* BaseContentLayer::createBomb24(b2Vec2 p, float32 angle, float32 desity){
    CustomeSprite* sprite = new CustomeSprite;
    sprite->initWithFile(ResManager::getManager()->getSharedFilePath(g_gameinPath+"bomb24.png").c_str());
    addChild(sprite, 0, Tag_Box_Bomb);
    sprite->release();
    sprite->setPosition(CCPointMake(p.x*PTM_RATIO, p.y*PTM_RATIO));
    lastCreateBombPos = sprite->getPosition();
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(p.x, p.y);
    bodyDef.userData = sprite;
    b2Body* bodyA = mWorld->CreateBody(&bodyDef);
    
    b2PolygonShape box;
    box.SetAsBox(0.5f, 0.5f);
    
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &box;	
    fixtureDef.density = desity;
    fixtureDef.friction = Fixture_Friction;
    fixtureDef.restitution = Fixture_Restitution;
    
    bodyA->CreateFixture(&fixtureDef);
    
    if (angle != 0.0f) {
        bodyA->SetTransform(bodyA->GetPosition(), CC_DEGREES_TO_RADIANS(angle));
    }
    
    return bodyA;
}
b2Body* BaseContentLayer::createBomb24_Circle(b2Vec2 p, float32 angle, float32 desity){
    CustomeSprite* sprite = new CustomeSprite;
    sprite->initWithFile(ResManager::getManager()->getSharedFilePath(g_gameinPath+"circle_bomb24.png").c_str());
    addChild(sprite, 0, Tag_Box_Bomb_Cirecle);
    sprite->release();
    
    sprite->setPosition(CCPointMake(p.x*PTM_RATIO, p.y*PTM_RATIO));
    lastCreateBombPos = sprite->getPosition();
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(p.x, p.y);
    bodyDef.userData = sprite;
    b2Body* bodyA = mWorld->CreateBody(&bodyDef);
    
    b2CircleShape circle;
    circle.m_radius = 0.5f;
    
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &circle;
    fixtureDef.density = desity;
    fixtureDef.friction = Fixture_Friction;
    fixtureDef.restitution = Fixture_Restitution;
    
    bodyA->CreateFixture(&fixtureDef);
    
    if (angle != 0.0f) {
        bodyA->SetTransform(bodyA->GetPosition(), CC_DEGREES_TO_RADIANS(angle));
    }
    
    return bodyA;
}

b2Body* BaseContentLayer::createBomb48_Circle(b2Vec2 p, float32 angle, float32 desity){
    CustomeSprite* sprite = new CustomeSprite;
    sprite->initWithFile(ResManager::getManager()->getSharedFilePath(g_gameinPath+"circle_bomb48.png").c_str());
    addChild(sprite, 0, Tag_Box_Bomb_Cirecle);
    sprite->release();
    sprite->setPosition(CCPointMake(p.x*PTM_RATIO, p.y*PTM_RATIO));
    lastCreateBombPos = sprite->getPosition();
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(p.x, p.y);
    bodyDef.userData = sprite;
    b2Body* bodyA = mWorld->CreateBody(&bodyDef);
    
    b2CircleShape circle;
    circle.m_radius = 1.0f;
    
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &circle;
    fixtureDef.density = desity;
    fixtureDef.friction = Fixture_Friction;
    fixtureDef.restitution = Fixture_Restitution;
    
    bodyA->CreateFixture(&fixtureDef);
    
    if (angle != 0.0f) {
        bodyA->SetTransform(bodyA->GetPosition(), CC_DEGREES_TO_RADIANS(angle));
    }
    
    return bodyA;
}

b2Body* BaseContentLayer::createBomb48_24(b2Vec2 p, float32 angle, float32 desity){
    CustomeSprite* sprite = new CustomeSprite;
    sprite->initWithFile(ResManager::getManager()->getSharedFilePath(g_gameinPath+"bomb48_24.png").c_str());
    addChild(sprite, 0, Tag_Box_Bomb);
    sprite->release();
    sprite->setPosition(CCPointMake(p.x*PTM_RATIO, p.y*PTM_RATIO));
    lastCreateBombPos = sprite->getPosition();
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(p.x, p.y);
    bodyDef.userData = sprite;
    b2Body* bodyA = mWorld->CreateBody(&bodyDef);
    

	{
		b2PolygonShape box;
		std::vector<b2PolygonShape> boxVec;

		box.SetAsBox(0.5f, 0.5f, b2Vec2(0.5f, 0.0f), 0);
		boxVec.insert(boxVec.end(), box);
		box.SetAsBox(0.5f, 0.5f, b2Vec2(-0.5f, 0.0f), 0);
		boxVec.insert(boxVec.end(), box);

		for (size_t i=0; i<boxVec.size(); ++i) {
			b2FixtureDef fixtureDef;
			fixtureDef.shape = &boxVec[i];	
			fixtureDef.density = desity;
			fixtureDef.friction = Fixture_Friction;
			fixtureDef.restitution = Fixture_Restitution;

			bodyA->CreateFixture(&fixtureDef);
		}
	}
    //b2PolygonShape box;
    //box.SetAsBox(1.0f, 0.5f);
    //
    //b2FixtureDef fixtureDef;
    //fixtureDef.shape = &box;	
    //fixtureDef.density = desity;
    //fixtureDef.friction = Fixture_Friction;
    //fixtureDef.restitution = Fixture_Restitution;
    //
    //bodyA->CreateFixture(&fixtureDef);
    
    if (angle != 0.0f) {
        bodyA->SetTransform(bodyA->GetPosition(), CC_DEGREES_TO_RADIANS(angle));
    }
    
    return bodyA;
}

//创建24的矩形
b2Body* BaseContentLayer::createSquare24(b2Vec2 p, float32 angle, float32 desity, const char *fileName, int tag)
{ 
    CustomeSprite* sprite = new CustomeSprite;
    sprite->initWithFile(ResManager::getManager()->getSharedFilePath(g_gameinPath+fileName).c_str());
    addChild(sprite, 0, tag);
    sprite->release();
    
    sprite->setPosition(CCPointMake(p.x*PTM_RATIO, p.y*PTM_RATIO));
    lastCreateBombPos = sprite->getPosition();
    
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(p.x, p.y);
    bodyDef.userData = sprite;
    b2Body* bodyA = mWorld->CreateBody(&bodyDef);
    
    b2PolygonShape box;
    box.SetAsBox(0.5f, 0.5f);
    
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &box;
    fixtureDef.density = desity;
    fixtureDef.friction = Fixture_Friction;
    fixtureDef.restitution = Fixture_Restitution;
    
    bodyA->CreateFixture(&fixtureDef);
    
    if (angle != 0.0f) {
        bodyA->SetTransform(bodyA->GetPosition(), CC_DEGREES_TO_RADIANS(angle));
    }
    
    return bodyA;
}

b2Body* BaseContentLayer::createSquare48(b2Vec2 p, float32 angle, float32 desity, const char *fileName, int tag){
    CustomeSprite* sprite = new CustomeSprite;
    sprite->initWithFile(ResManager::getManager()->getSharedFilePath(g_gameinPath+fileName).c_str());
    addChild(sprite, 0, tag);
    sprite->release();
    sprite->setPosition(CCPointMake(p.x*PTM_RATIO, p.y*PTM_RATIO));
    lastCreateBombPos = sprite->getPosition();
    
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(p.x, p.y);
    bodyDef.userData = sprite;
    b2Body* bodyA = mWorld->CreateBody(&bodyDef);
	{
		b2PolygonShape box;
		std::vector<b2PolygonShape> boxVec;
        
		box.SetAsBox(0.5f, 0.5f, b2Vec2(0.5f, 0.5f), 0);
		boxVec.insert(boxVec.end(), box);
		box.SetAsBox(0.5f, 0.5f, b2Vec2(-0.5f, 0.5f), 0);
		boxVec.insert(boxVec.end(), box);
		box.SetAsBox(0.5f, 0.5f, b2Vec2(-0.5f, -0.5f), 0);
		boxVec.insert(boxVec.end(), box);
		box.SetAsBox(0.5f, 0.5f, b2Vec2(0.5f, -0.5f), 0);
		boxVec.insert(boxVec.end(), box);
        
		for (size_t i=0; i<boxVec.size(); ++i) {
			b2FixtureDef fixtureDef;
			fixtureDef.shape = &boxVec[i];
			fixtureDef.density = desity;
			fixtureDef.friction = Fixture_Friction;
			fixtureDef.restitution = Fixture_Restitution;
            
			bodyA->CreateFixture(&fixtureDef);
		}
	}
    
    if (angle != 0.0f) {
        bodyA->SetTransform(bodyA->GetPosition(), CC_DEGREES_TO_RADIANS(angle));
    }
    
    return bodyA;
}

b2Body* BaseContentLayer::createDarkHole24(b2Vec2 p, float32 angle, float32 desity)
{
    DarkHoleBomb* sprite = new DarkHoleBomb(196.0f/2.0f,"wormhole_bomb_24.png","blackhole.png");
    addChild(sprite, 0, Tag_Box_Bomb_DarkWhole);
    sprite->release();
    
    sprite->setPosition(CCPointMake(p.x*PTM_RATIO, p.y*PTM_RATIO));
    lastCreateBombPos = sprite->getPosition();

	{
		CCSprite* fog_cover2 = CCSprite::createWithSpriteFrameName("wormhole_bomb_24.png");
		CCSprite* fog_cover3 = CCSprite::createWithSpriteFrameName("wormhole_bomb_inside_24.png");
		CCSprite* fog_cover = CCSprite::createWithSpriteFrameName("wormhole_bomb_24_cover.png");
		sprite->addChild(fog_cover,15);
		sprite->addChild(fog_cover3,14);
		sprite->addChild(fog_cover2,13);
		fog_cover3->runAction(CCRepeatForever::create(CCRotateBy::create(0.016f, 1)));
		fog_cover2->setPosition(ccp(sprite->getContentSize().width/2,sprite->getContentSize().height/2));
		fog_cover->setPosition(ccp(sprite->getContentSize().width/2,sprite->getContentSize().height/2));
		fog_cover3->setPosition(ccp(sprite->getContentSize().width/2,sprite->getContentSize().height/2));
		//去掉小黑洞的闪烁
//		fog_cover3->runAction(CCRepeat::create(CCSequence::create(CCFadeTo::create(1.0f,100),CCFadeTo::create(1.0f,255),NULL),-1));
		fog_cover->setOpacity(100);
	}
    
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(p.x, p.y);
    bodyDef.userData = sprite;
    b2Body* bodyA = mWorld->CreateBody(&bodyDef);
    
    b2PolygonShape box;
    box.SetAsBox(0.5f, 0.5f);
    
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &box;
    fixtureDef.density = desity;
    fixtureDef.friction = Fixture_Friction;
    fixtureDef.restitution = Fixture_Restitution;
    
    bodyA->CreateFixture(&fixtureDef);
    
    if (angle != 0.0f) {
        bodyA->SetTransform(bodyA->GetPosition(), CC_DEGREES_TO_RADIANS(angle));
    }
    
    return bodyA;
}
b2Body* BaseContentLayer::createDarkHole48(b2Vec2 p, float32 angle, float32 desity)
{
    DarkHoleBomb* sprite = new DarkHoleBomb(196.0f,"wormhole_bomb_48.png","blackhole.png");
    addChild(sprite, 0, Tag_Box_Bomb_DarkWhole);
    sprite->release();
    
    sprite->setPosition(CCPointMake(p.x*PTM_RATIO, p.y*PTM_RATIO));
    lastCreateBombPos = sprite->getPosition();

	{
		CCSprite* fog_cover = CCSprite::createWithSpriteFrameName("wormhole_bomb_48_cover.png");
		CCSprite* fog_cover2 = CCSprite::createWithSpriteFrameName("wormhole_bomb_48.png");
		CCSprite* inside = CCSprite::createWithSpriteFrameName("wormhole_bomb_inside_48.png");
		sprite->addChild(inside,13);
		sprite->addChild(fog_cover,14);
		sprite->addChild(fog_cover2,12);
		inside->runAction(CCRepeatForever::create(CCRotateBy::create(0.016f, 1)));
		fog_cover->setPosition(ccp(sprite->getContentSize().width/2,sprite->getContentSize().height/2));
		fog_cover2->setPosition(ccp(sprite->getContentSize().width/2,sprite->getContentSize().height/2));
		inside->setPosition(ccp(sprite->getContentSize().width/2,sprite->getContentSize().height/2));


//		CCFiniteTimeAction* actionDropInForever = CCRepeat::create(
//			CCSequence::create(												// 无限循环
//				CCDelayTime::create(0.0f),									// 延时
//				CCFadeTo::create(0.5f,255),									// 显示
//				CCSpawn::create(											// 内旋
//					CCRotateBy::create(5.0f,180.0f),
//					CCFadeTo::create(5.0f,10),
//					CCScaleTo::create(5.0f,0.2f),NULL),
//				CCFadeTo::create(0,0),										// 隐藏
//				CCScaleTo::create(0,1.2f),									// 变回原大小
//				NULL),-1);

// 		// 动画是1秒就旋转了36度
// 		for (int i = 0;i<5;i++)
// 		{
// 			CustomeSprite* item1 = CustomeSprite::create(ResManager::getManager()->getSharedFilePath(g_gameinPath+"wormhole_bomb_inside_48_2.png").c_str());
// 			sprite->addChild(item1,11);
// 			item1->setPosition(ccp(sprite->getContentSize().width/2,sprite->getContentSize().height/2));
// 			item1->setOpacity(0);
// 
// 			item1->setRotation(20 * i);
// 			item1->runAction(CCSequence::create(
// 				CCDelayTime::create(1.0f * i),
// 				(CCFiniteTimeAction*)actionDropInForever->copy()->autorelease(),NULL));
// 		}	

		//item2->setOpacity(0);
		//item1->setOpacity(0);

		//item1->runAction(actionDropInForever);
		//item2->runAction(CCSequence::create(
		//	CCDelayTime::create(3.0f),
		//	(CCFiniteTimeAction*)actionDropInForever->copy()->autorelease(),NULL));

		{
			//int num = 5;
			//for (int i=0;i<360/num;i++)
			//{
			//	CCParticleSystem* par3 = CCParticleSystemQuad::create(ResManager::getManager()->getSharedFilePath(g_gameinPath+"DarkWhole.plist").c_str());
			//	ccBlendFunc tBlendFunc = { GL_ONE, GL_ONE_MINUS_SRC_ALPHA };
			//	par3->setBlendFunc(tBlendFunc);
			//	par3->setPositionType(kCCPositionTypeGrouped);
			//	sprite->addChild(par3);
			//	par3->setPosition(ccp(24,22.5));
			//	par3->setRotation(i*num);
			//}
			
			////DarkWhole.plist
			//CCParticleSystemQuad* m_emitter = CCParticleSystemQuad::create(ResManager::getManager()->getSharedFilePath(g_gameinPath+"Phoenix.plist").c_str());
			//ccBlendFunc tBlendFunc = { GL_ONE, GL_ONE_MINUS_SRC_ALPHA };
			//m_emitter->setBlendFunc(tBlendFunc);
			//m_emitter->setPositionType(kCCPositionTypeGrouped);
			//CCParticleSystem* par1 = m_emitter;
			//sprite->addChild(par1);
			//par1->setPosition(ccp(24,22.5));
			//par1->runAction(CCRepeatForever::create(CCRotateBy::create(1.5f,360.0f)));

			
			//CCParticleSystemQuad* m_emitter = CCParticleSystemQuad::create(ResManager::getManager()->getSharedFilePath(g_gameinPath+"BlackHoleParticle.plist").c_str());
			//ccBlendFunc tBlendFunc = { GL_ONE, GL_ONE_MINUS_SRC_ALPHA };
			//m_emitter->setBlendFunc(tBlendFunc);
			//m_emitter->setPositionType(kCCPositionTypeGrouped);
			//sprite->addChild(m_emitter,128);
			//m_emitter->setPosition(ccp(24,22.5));
			//m_emitter->setScale(0.08f);
			//m_emitter->setBlendAdditive(true);
		}
	}
    
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(p.x, p.y);
    bodyDef.userData = sprite;
    b2Body* bodyA = mWorld->CreateBody(&bodyDef);
    
    b2PolygonShape box;
    box.SetAsBox(1.0f, 1.0f);
    
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &box;
    fixtureDef.density = desity;
    fixtureDef.friction = Fixture_Friction;
    fixtureDef.restitution = Fixture_Restitution;
    
    bodyA->CreateFixture(&fixtureDef);
    
    if (angle != 0.0f) {
        bodyA->SetTransform(bodyA->GetPosition(), CC_DEGREES_TO_RADIANS(angle));
    }
    
    return bodyA;
}
void BaseContentLayer::createTableExit()
{
}

void BaseContentLayer::createTrack(CCNode* pParent, CCPoint point, float width, float angle)
{
	float height = 22;

	CCScale9Sprite* pTrack = CCScale9Sprite::create(CCRectMake(22, 0, 1, height), ResManager::getManager()->getFilePath(g_gameinPath+"track_all.png").c_str());
	pParent->addChild(pTrack);
	pTrack->setPosition(point);
	pTrack->setContentSize(CCSizeMake(width, height));
	pTrack->setRotation(angle);
}

void BaseContentLayer::createTrack(CCSpriteBatchNode* middleSheet,CCPoint point1,CCPoint point2,CCSprite* leftHead,CCSprite* rightHead,CCPoint offset)
{
	CCPoint vectorP1ToP2 = ccpSub(point2,point1);							// p1到p2的方向向量
	float angle = CC_RADIANS_TO_DEGREES(ccpAngle(vectorP1ToP2,ccp(1,0)));	// 相对于X轴的角度
	if (vectorP1ToP2.y < 0)
	{//修正
		angle = -angle;
	}
	float distance = ccpDistance(point1,point2);
#if USE_SCALE9SPRITE_TRACK == 0
	float middleLength = middleSheet->getTexture()->getContentSize().width;	// 中间段的纹理长度
	int middleNum = distance / middleLength + 1;							// 需要的中间段数
	int middleNumSub = 1;	// 少创建middleNumSub个
	for (int i=0;i<middleNum-middleNumSub;i++)
	{
		CCTexture2D* texture = middleSheet->getTexture();
		CCSprite* tmpSprite = CCSprite::createWithTexture(texture);
		tmpSprite->setPosition( ccpAdd(ccpAdd(point1,ccpMult(vectorP1ToP2,(float)(i+middleNumSub)/(float)middleNum)),offset));
		tmpSprite->setRotation(-1 * angle);
		middleSheet->addChild(tmpSprite,-1);
	}


	leftHead->setPosition(ccpAdd(point1,offset));
	leftHead->setRotation(-angle);

	rightHead->setPosition(ccpAdd(point2,offset));
	rightHead->setRotation(-angle);
#else
	createTrack(mScense->getBackgroudCoverLayer(), ccpMidpoint(point1, point2), distance, -angle);
#endif
}

void BaseContentLayer::createTableTrack(CCSpriteBatchNode* middleSheet,CCPoint point1,CCPoint point2,CCPoint offset)
{
	//改用9宫格图片替换
#if USE_SCALE9SPRITE_TRACK == 0
	CCSprite* leftHead = NULL;
	CCSprite* rightHead = NULL;
	ResManager* resMgr = ResManager::getManager();

	string tableTrackFile = SpriteHelper::splicingNum_String("table_no_S%d_track_head.png", 1);//mScense->getCurrentSeasonId());
	// 头部1
	{
		CCSprite* head1 = CCSprite::create(resMgr->getSharedFilePath(g_gameinPath+tableTrackFile).c_str());
		//BackgroudLayerFactory::getCurrentBgLayer()->addChild(head1,0);
		mScense->getBackgroudCoverLayer()->addChild(head1,0);

		CCSprite* gear1 = CCSprite::create(resMgr->getSharedFilePath(g_gameinPath+"table_no_track_gear.png").c_str());
		head1->addChild(gear1);
		gear1->setPosition(ccp(gear1->getContentSize().width/2+2,head1->getContentSize().height/2));
		gear1->runAction(CCRepeat::create(CCRotateBy::create(1.0f,180),-1));
		leftHead = head1;
	}
	{
		CCSprite* head2 = CCSprite::create(resMgr->getSharedFilePath(g_gameinPath+tableTrackFile).c_str());
		//BackgroudLayerFactory::getCurrentBgLayer()->addChild(head2,0);
		mScense->getBackgroudCoverLayer()->addChild(head2,0);

		head2->setFlipX(true);
		CCSprite* gear2 = CCSprite::create(resMgr->getSharedFilePath(g_gameinPath+"table_no_track_gear.png").c_str());
		head2->addChild(gear2);
		gear2->setPosition(ccp(head2->getContentSize().width - gear2->getContentSize().width/2 - 2,head2->getContentSize().height/2));
		gear2->runAction(CCRepeat::create(CCRotateBy::create(1.0f,180),-1));
		rightHead = head2;
	}

	createTrack(middleSheet,point1,point2,leftHead,rightHead,offset);
#else
	createTrack(NULL,point1,point2,NULL,NULL,offset);
#endif
}

void BaseContentLayer::createMotorTrack(CCSpriteBatchNode* middleSheet,CCPoint point1,CCPoint point2,CCPoint offset)
{
	//改用9宫格图片替换
#if USE_SCALE9SPRITE_TRACK == 0
	CCSprite* leftHead = NULL;
	CCSprite* rightHead = NULL;
	ResManager* resMgr = ResManager::getManager();

	string motorTrackFile = SpriteHelper::splicingNum_String("turn_anchor_S%d_track_head.png", 1);//mScense->getCurrentSeasonId());

	// 头部1
	{
		CCSprite* head1 = CCSprite::create(resMgr->getSharedFilePath(g_gameinPath+motorTrackFile).c_str());
		//BackgroudLayerFactory::getCurrentBgLayer()->addChild(head1,0);
		mScense->getBackgroudCoverLayer()->addChild(head1,0);

		leftHead = head1;
	}
	{
		CCSprite* head2 = CCSprite::create(resMgr->getSharedFilePath(g_gameinPath+motorTrackFile).c_str());
		//BackgroudLayerFactory::getCurrentBgLayer()->addChild(head2,0);
		mScense->getBackgroudCoverLayer()->addChild(head2,0);

		head2->setFlipX(true);
		rightHead = head2;
	}
	createTrack(middleSheet,point1,point2,leftHead,rightHead,offset);
#else
	createTrack(NULL,point1,point2,NULL,NULL,offset);
#endif
}

b2Body* BaseContentLayer::createTable144(b2Vec2 p, GameMovement move, float32 angle, bool effective,bool isKeyItem){
    CustomeSprite* sprite = new CustomeSprite;
    if (effective) {
        sprite->initWithFile(ResManager::getManager()->getSharedFilePath(g_gameinPath+"table144_24.png").c_str());
        addChild(sprite, 0, Tag_Box_Table);
        effectveTableVec.insert(effectveTableVec.end(), sprite);
        
        {//添加桌子的效果
            //addTableNick(sprite);
			//if (getMachinePerformanceLevel() > 0)
			{
				addTableFlowLight(sprite);
			}
            //addTableHL("table144_24_HL.png", sprite);
        }
    }else {
        sprite->initWithFile(ResManager::getManager()->getSharedFilePath(g_gameinPath+"table144_24_no.png").c_str());
        addChild(sprite, 0, Tag_Box_Table_Ineffective);
    }
    sprite->release();
    sprite->setIsKeyItem(isKeyItem);
    sprite->setPosition(CCPointMake(p.x*PTM_RATIO, p.y*PTM_RATIO));
	//桌面背景
	TableBack::createTableBg(sprite,!move.enable);
    b2BodyDef bodyDef;
    bodyDef.type = b2_kinematicBody;
    bodyDef.position.Set(p.x, p.y);
    bodyDef.userData = sprite;
    b2Body* bodyA = mWorld->CreateBody(&bodyDef);
    
    b2PolygonShape box;
    if (effective) {
        box.SetAsBox(3.0f, 0.5f);
    }else {
        box.SetAsBox(3.0f, 0.5f);
    }
    
    
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &box;	
    fixtureDef.density = Fixture_Density;
    fixtureDef.friction = Fixture_Friction;
    fixtureDef.restitution = Fixture_Restitution;
    
    bodyA->CreateFixture(&fixtureDef);
    
    if (angle != 0.0f) {
        bodyA->SetTransform(bodyA->GetPosition(), CC_DEGREES_TO_RADIANS(angle));
    }
    
    
    if (move.enable) {
        CCSprite* ghost = new CCSprite;
		ghost->autorelease();
        ghost->init();
        addChild(ghost);
//		CCPoint testTemp = sprite->getPosition();
//		CCPoint testTemp2 = CCPointMake(p.x*PTM_RATIO, p.y*PTM_RATIO);

		// LF 修改，解决起点和位置为不同点时的bug
		ghost->setPosition(CCPointMake(-p.x*PTM_RATIO, p.y*PTM_RATIO));
		//ghost->setPosition(CCPointMake(-move.moveStartPoint.x*PTM_RATIO, move.moveStartPoint.y*PTM_RATIO));
        CCMoveTo* move1 = CCMoveTo::create(move.duration/2, CCPointMake(-move.moveBackPoint.x*PTM_RATIO, move.moveBackPoint.y*PTM_RATIO));
        CCMoveTo* move2 = CCMoveTo::create(move.duration/2, CCPointMake(-move.moveStartPoint.x*PTM_RATIO, move.moveStartPoint.y*PTM_RATIO));
        CCEaseInOut* easy1 = CCEaseInOut::create(move1, 2);
        CCEaseInOut* easy2 = CCEaseInOut::create(move2, 2);
        CCSequence* seq = CCSequence::create(easy1, easy2,NULL);
        CCRepeatForever* repeat = CCRepeatForever::create(seq);
		ghost->stopAllActions();
        ghost->runAction(repeat);
        
        GameTableMovementData moveData;
        moveData.movement = move;
        moveData.body = bodyA;
        moveData.ghost = ghost;
        moveData.ghostLastTickPosition = ghost->getPosition();
        
        mGameTableMovementDataVec.insert(mGameTableMovementDataVec.end(), moveData);
    }
    
    return bodyA;
}

b2Body* BaseContentLayer::createTable192(b2Vec2 p, GameMovement move, float32 angle, bool effective,bool isKeyItem){
    CustomeSprite* sprite = new CustomeSprite;

    if (effective) {
        sprite->initWithFile(ResManager::getManager()->getSharedFilePath(g_gameinPath+"table192_24.png").c_str());
        addChild(sprite, 0, Tag_Box_Table);
        effectveTableVec.insert(effectveTableVec.end(), sprite);
        {//添加桌子的效果
            //addTableNick(sprite);
            addTableFlowLight(sprite);
            //addTableHL("table192_24_HL.png", sprite);
        }
    }else {
        sprite->initWithFile(ResManager::getManager()->getSharedFilePath(g_gameinPath+"table192_24_no.png").c_str());
        //sprite->setColor(ccGRAY);
        addChild(sprite, 0, Tag_Box_Table_Ineffective);
    }
    sprite->release();
    sprite->setIsKeyItem(isKeyItem);
    sprite->setPosition(CCPointMake(p.x*PTM_RATIO, p.y*PTM_RATIO));
    
	//桌面背景
	TableBack::createTableBg(sprite,!move.enable);

    b2BodyDef bodyDef;
    bodyDef.type = b2_kinematicBody;
    bodyDef.position.Set(p.x, p.y);
    bodyDef.userData = sprite;
    b2Body* bodyA = mWorld->CreateBody(&bodyDef);
    
    b2PolygonShape box;
    if (effective) {
        box.SetAsBox(4.0f, 0.5f);
    }else {
        box.SetAsBox(4.0f, 0.5f);
    }
    
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &box;	
    fixtureDef.density = Fixture_Density;
    fixtureDef.friction = Fixture_Friction;
    fixtureDef.restitution = Fixture_Restitution;
    
    bodyA->CreateFixture(&fixtureDef);
    
    if (angle != 0.0f) {
        bodyA->SetTransform(bodyA->GetPosition(), CC_DEGREES_TO_RADIANS(angle));
    }
    
    if (move.enable) {
        CCSprite* ghost = new CCSprite;
		ghost->autorelease();
        ghost->init();
        addChild(ghost);
		// LF 修改，解决起点和位置为不同点时的bug
		ghost->setPosition(CCPointMake(-p.x*PTM_RATIO, p.y*PTM_RATIO));
		//ghost->setPosition(CCPointMake(-move.moveStartPoint.x*PTM_RATIO, move.moveStartPoint.y*PTM_RATIO));
        CCMoveTo* move1 = CCMoveTo::create(move.duration/2, CCPointMake(-move.moveBackPoint.x*PTM_RATIO, move.moveBackPoint.y*PTM_RATIO));
        CCMoveTo* move2 = CCMoveTo::create(move.duration/2, CCPointMake(-move.moveStartPoint.x*PTM_RATIO, move.moveStartPoint.y*PTM_RATIO));
        CCEaseInOut* easy1 = CCEaseInOut::create(move1, 2);
        CCEaseInOut* easy2 = CCEaseInOut::create(move2, 2);
        CCSequence* seq = CCSequence::create(easy1, easy2,NULL);
        CCRepeatForever* repeat = CCRepeatForever::create(seq);
        ghost->runAction(repeat);
        
        GameTableMovementData moveData;
        moveData.movement = move;
        moveData.body = bodyA;
        moveData.ghost = ghost;
        moveData.ghostLastTickPosition = ghost->getPosition();
        
        mGameTableMovementDataVec.insert(mGameTableMovementDataVec.end(), moveData);
    }
    
    return bodyA;
}

b2Body* BaseContentLayer::createTable288(b2Vec2 p, GameMovement move, float32 angle, bool effective,bool isKeyItem){
    CustomeSprite* sprite = new CustomeSprite;
    
    if (effective) {
        sprite->initWithFile(ResManager::getManager()->getSharedFilePath(g_gameinPath+"table288_24.png").c_str());
        addChild(sprite, 0, Tag_Box_Table);
        effectveTableVec.insert(effectveTableVec.end(), sprite);
        
        sprite->setPosition(CCPointMake(p.x*PTM_RATIO, p.y*PTM_RATIO));
        {//添加桌子的效果
            //addTableNick(sprite);
            addTableFlowLight(sprite);
            //addTableHL("table288_24_HL.png", sprite);
        }
    }else {
        sprite->initWithFile(ResManager::getManager()->getSharedFilePath(g_gameinPath+"table288_24_no.png").c_str());
        //sprite->setColor(ccGRAY);
        addChild(sprite, 0, Tag_Box_Table_Ineffective);
    }sprite->release();
    sprite->setIsKeyItem(isKeyItem);
    sprite->setPosition(CCPointMake(p.x*PTM_RATIO, p.y*PTM_RATIO));
    
	//桌面背景
	TableBack::createTableBg(sprite,!move.enable);

    b2BodyDef bodyDef;
    bodyDef.type = b2_kinematicBody;
    bodyDef.position.Set(p.x, p.y);
    bodyDef.userData = sprite;
    b2Body* bodyA = mWorld->CreateBody(&bodyDef);
    
    b2PolygonShape box;
    if (effective) {
        box.SetAsBox(6.0f, 0.5f);
    }else {
        box.SetAsBox(6.0f, 0.5f);
    }
    
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &box;	
    fixtureDef.density = Fixture_Density;
    fixtureDef.friction = Fixture_Friction;
    fixtureDef.restitution = Fixture_Restitution;
    
    bodyA->CreateFixture(&fixtureDef);
    
    if (angle != 0.0f) {
        bodyA->SetTransform(bodyA->GetPosition(), CC_DEGREES_TO_RADIANS(angle));
    }
    
    if (move.enable) {
        CCSprite* ghost = new CCSprite;
		ghost->autorelease();
        ghost->init();
        addChild(ghost);
		// LF 修改，解决起点和位置为不同点时的bug
		ghost->setPosition(CCPointMake(-p.x*PTM_RATIO, p.y*PTM_RATIO));
		//ghost->setPosition(CCPointMake(-move.moveStartPoint.x*PTM_RATIO, move.moveStartPoint.y*PTM_RATIO));
        CCMoveTo* move1 = CCMoveTo::create(move.duration/2, CCPointMake(-move.moveBackPoint.x*PTM_RATIO, move.moveBackPoint.y*PTM_RATIO));
        CCMoveTo* move2 = CCMoveTo::create(move.duration/2, CCPointMake(-move.moveStartPoint.x*PTM_RATIO, move.moveStartPoint.y*PTM_RATIO));
        CCEaseInOut* easy1 = CCEaseInOut::create(move1, 2);
        CCEaseInOut* easy2 = CCEaseInOut::create(move2, 2);
        CCSequence* seq = CCSequence::create(easy1, easy2,NULL);
        CCRepeatForever* repeat = CCRepeatForever::create(seq);
        ghost->runAction(repeat);
        
        GameTableMovementData moveData;
        moveData.movement = move;
        moveData.body = bodyA;
        moveData.ghost = ghost;
        moveData.ghostLastTickPosition = ghost->getPosition();
        
        mGameTableMovementDataVec.insert(mGameTableMovementDataVec.end(), moveData);
    }
    
    return bodyA;
}

b2Body* BaseContentLayer::createTable336(b2Vec2 p, GameMovement move, float32 angle, bool effective,bool isKeyItem){
    CustomeSprite* sprite = new CustomeSprite;

    if (effective) {
        sprite->initWithFile(ResManager::getManager()->getSharedFilePath(g_gameinPath+"table336_24.png").c_str());
        addChild(sprite, 0, Tag_Box_Table);
        effectveTableVec.insert(effectveTableVec.end(), sprite);
        
        {//添加桌子的效果
            //addTableNick(sprite);
            addTableFlowLight(sprite);
            //addTableHL("table336_24_HL.png", sprite);
        }
    }else {
        sprite->initWithFile(ResManager::getManager()->getSharedFilePath(g_gameinPath+"table336_24_no.png").c_str());
        //sprite->setColor(ccGRAY);
        addChild(sprite, 0, Tag_Box_Table_Ineffective);
    }sprite->release();
    sprite->setPosition(CCPointMake(p.x*PTM_RATIO, p.y*PTM_RATIO));
    
	//桌面背景
	TableBack::createTableBg(sprite,!move.enable);

    b2BodyDef bodyDef;
    bodyDef.type = b2_kinematicBody;
    bodyDef.position.Set(p.x, p.y);
    bodyDef.userData = sprite;
    b2Body* bodyA = mWorld->CreateBody(&bodyDef);
    
    b2PolygonShape box;
    if (effective) {
        box.SetAsBox(7.0f, 0.5f);
    }else {
        box.SetAsBox(7.0f, 0.5f);
    }
    
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &box;	
    fixtureDef.density = Fixture_Density;
    fixtureDef.friction = Fixture_Friction;
    fixtureDef.restitution = Fixture_Restitution;
    
    bodyA->CreateFixture(&fixtureDef);
    
    if (angle != 0.0f) {
        bodyA->SetTransform(bodyA->GetPosition(), CC_DEGREES_TO_RADIANS(angle));
    }
    
    if (move.enable) {
        CCSprite* ghost = new CCSprite;
		ghost->autorelease();
        ghost->init();
        addChild(ghost);
		// LF 修改，解决起点和位置为不同点时的bug
		ghost->setPosition(CCPointMake(-p.x*PTM_RATIO, p.y*PTM_RATIO));
		//ghost->setPosition(CCPointMake(-move.moveStartPoint.x*PTM_RATIO, move.moveStartPoint.y*PTM_RATIO));
        CCMoveTo* move1 = CCMoveTo::create(move.duration/2, CCPointMake(-move.moveBackPoint.x*PTM_RATIO, move.moveBackPoint.y*PTM_RATIO));
        CCMoveTo* move2 = CCMoveTo::create(move.duration/2, CCPointMake(-move.moveStartPoint.x*PTM_RATIO, move.moveStartPoint.y*PTM_RATIO));
        CCEaseInOut* easy1 = CCEaseInOut::create(move1, 2);
        CCEaseInOut* easy2 = CCEaseInOut::create(move2, 2);
        CCSequence* seq = CCSequence::create(easy1, easy2,NULL);
        CCRepeatForever* repeat = CCRepeatForever::create(seq);
        ghost->runAction(repeat);
        
        GameTableMovementData moveData;
        moveData.movement = move;
        moveData.body = bodyA;
        moveData.ghost = ghost;
        moveData.ghostLastTickPosition = ghost->getPosition();
        
        mGameTableMovementDataVec.insert(mGameTableMovementDataVec.end(), moveData);
    }
    
    return bodyA;
}

b2Body* BaseContentLayer::createRedStar60(b2Vec2 p, float32 angle, float32 desity,bool isThrowShape /*= false*/){
    //同时只能有一个红星存在
    if (mRedstarExist) {
        return NULL;
    }
    mRedstarExist = true;
    
    CustomeSprite* sprite = new CustomeSprite;
    sprite->initWithFile(ResManager::getManager()->getSharedFilePath(g_gameinPath+"star60.png").c_str());

    addChild(sprite, 0, Tag_Box_RedStar);
    sprite->setRotation(angle);
    sprite->release();

#if 0
    {//红星出场的效果动画
        
        if (!isThrowShape && this->needRedStarBeginEff)
        {          
            effectveRedStarVec.insert(effectveRedStarVec.end(), sprite);
            addRedStarFlowLight(sprite);
        }
    }
#endif

	//外发光效果
#if 0
	{
		CCArmature* pArmature = CCArmature::create("starfall");
		pArmature->getAnimation()->play("stay");
		sprite->addChild(pArmature, sprite->getZOrder()-1);
		CCSize size = sprite->getContentSize();
		pArmature->setPosition(ccp(size.width/2, size.height/2 + 5));
	}
#endif
	//流光效果
	{
		CCArmature* pArmature = CCArmature::create("starfall");
		pArmature->getAnimation()->play("starlight");
		sprite->addChild(pArmature);
		CCSize size = sprite->getContentSize();
		pArmature->setPosition(ccp(size.width/2, size.height/2 + 5));
		ccBlendFunc blend = {GL_SRC_ALPHA, GL_ONE};
		pArmature->setBlendFunc(blend);
	}
	//闪烁效果
	{
		ResManager* resMgr = ResManager::getManager();
		CCSprite* starMask = CCSprite::create(resMgr->getSharedFilePath(g_gameinPath+"star60_shanshuo.png").c_str());
		ccBlendFunc blend = {GL_SRC_ALPHA, GL_ONE};
		starMask->setBlendFunc(blend);
		sprite->addChild(starMask, 1, Tag_Box_RedStar_Mask);
		starMask->setPosition(ccp(sprite->getContentSize().width/2,sprite->getContentSize().height/2));
		starMask->runAction(CCRepeat::create(CCSequence::create(CCFadeTo::create(0.9f,0),CCFadeTo::create(0.9f,180),NULL),-1));
	}


	// 红星一直的效果图
#if 0
	{
		//if (getMachinePerformanceLevel() > 0)
		{
			ResManager* resMgr = ResManager::getManager();
			CustomeSprite* redStarLeft = CustomeSprite::create(resMgr->getSharedFilePath(g_gameinPath+"star60_left.png").c_str());
			CustomeSprite* redStarRight = CustomeSprite::create(resMgr->getSharedFilePath(g_gameinPath+"star60_right.png").c_str());
			sprite->addChild(redStarLeft);redStarLeft->setPosition(ccp(sprite->getContentSize().width/2,sprite->getContentSize().height/2));
			sprite->addChild(redStarRight);redStarRight->setPosition(ccp(sprite->getContentSize().width/2,sprite->getContentSize().height/2));
			redStarLeft->setOpacity(0);
			redStarRight->setOpacity(0);
			redStarLeft->runAction(CCRepeat::create(CCSequence::create(CCFadeTo::create(0.8f,255),CCFadeTo::create(0.8f,0),CCDelayTime::create(1.6f),NULL),-1));
			redStarRight->runAction(CCRepeat::create(CCSequence::create(CCDelayTime::create(1.6f),CCFadeTo::create(0.8f,255),CCFadeTo::create(0.8f,0),NULL),-1));
		}
	}
#endif	
    sprite->setPosition(CCPointMake(p.x*PTM_RATIO, p.y*PTM_RATIO));
	initRedStartPos = sprite->getPosition();

    // Define the dynamic body.
    //Set up a 1m squared box in the physics world
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(p.x, p.y);
    bodyDef.userData = sprite;
    b2Body *body = mWorld->CreateBody(&bodyDef);
    
    b2Vec2 trianglArr[][3] = {
        {b2Vec2(0.0f/PTM_RATIO,30.0f/PTM_RATIO),b2Vec2(-11.0f/PTM_RATIO,13.0f/PTM_RATIO),b2Vec2(11.0f/PTM_RATIO,13.0f/PTM_RATIO)}
        ,{b2Vec2(-11.0f/PTM_RATIO,13.0f/PTM_RATIO),b2Vec2(-29.0f/PTM_RATIO,10.0f/PTM_RATIO),b2Vec2(-15.0f/PTM_RATIO,-4.0f/PTM_RATIO)}
        ,{b2Vec2(-15.0f/PTM_RATIO,-4.0f/PTM_RATIO),b2Vec2(-18.0f/PTM_RATIO,-24.0f/PTM_RATIO),b2Vec2(0.0f/PTM_RATIO,-15.0f/PTM_RATIO)}
        ,{b2Vec2(0.0f/PTM_RATIO,-15.0f/PTM_RATIO),b2Vec2(18.0f/PTM_RATIO,-24.0f/PTM_RATIO),b2Vec2(15.0f/PTM_RATIO,-4.0f/PTM_RATIO)}
        ,{b2Vec2(11.0f/PTM_RATIO,13.0f/PTM_RATIO),b2Vec2(15.0f/PTM_RATIO,-4.0f/PTM_RATIO),b2Vec2(29.0f/PTM_RATIO,10.0f/PTM_RATIO)}      
    };
    
    for (int trigIndex=0; trigIndex<5; ++trigIndex) {
        b2PolygonShape dynamicBox;
        dynamicBox.Set(trianglArr[trigIndex], 3);
        
        // Define the dynamic body fixture.
        b2FixtureDef fixtureDef;
        fixtureDef.shape = &dynamicBox;	
        fixtureDef.density = desity;
        fixtureDef.friction = Fixture_Friction;
        fixtureDef.restitution = Fixture_Restitution;
        body->CreateFixture(&fixtureDef);
    }
    
    //中间那个五边形
    {
        b2Vec2 trianglArr[] = {
            b2Vec2(-11.0f/PTM_RATIO,13.0f/PTM_RATIO), b2Vec2(-15.0f/PTM_RATIO,-4.0f/PTM_RATIO), b2Vec2(0.0f/PTM_RATIO,-15.0f/PTM_RATIO),b2Vec2(15.0f/PTM_RATIO,-4.0f/PTM_RATIO),b2Vec2(11.0f/PTM_RATIO,13.0f/PTM_RATIO)
        };
        b2PolygonShape dynamicBox;
        dynamicBox.Set(trianglArr, 5);
        
        // Define the dynamic body fixture.
        b2FixtureDef fixtureDef;
        fixtureDef.shape = &dynamicBox;	
        fixtureDef.density = desity;
        fixtureDef.friction = Fixture_Friction;
        fixtureDef.restitution = Fixture_Restitution;
        body->CreateFixture(&fixtureDef);
    }
    
    if (angle != 0.0f) {
        body->SetTransform(body->GetPosition(), CC_DEGREES_TO_RADIANS(angle));
    }
    
	if (m_iCurPlayMode == playMode_Normal)
	{
		AchievementManager::getInstance()->setRedStar(sprite,body,isThrowShape);
	}

    return body;
}

b2Body* BaseContentLayer::createGrayStar60(b2Vec2 p, float32 angle, bool canDestroy, float32 desity){
    //先创建模糊后的纹理
    if (canDestroy) {
        addBluredSpriteToMap(ResManager::getManager()->getSharedFilePath(g_gameinPath+"graystar60.png"));
    }
    
    
    CustomeSprite* sprite = new CustomeSprite;
    
    if (canDestroy) {
        sprite->initWithFile(ResManager::getManager()->getSharedFilePath(g_gameinPath+"graystar60.png").c_str());
        addChild(sprite, 0, Tag_Box_can_Destroied);
    }else {
        sprite->initWithFile(ResManager::getManager()->getSharedFilePath(g_gameinPath+"graystar60_no.png").c_str());
		addChild(sprite, 0, Tag_Box_cannot_Destroied);
    }sprite->release();
    
    sprite->setPosition(CCPointMake(p.x*PTM_RATIO, p.y*PTM_RATIO));
	// graystar60创建碎片
	if(USE_MANUAL_FRAGMENT)
	{
		sprite->addFragment_ResMgr("graystar60_frame1.png",ccp(21,39),0);
		sprite->addFragment_ResMgr("graystar60_frame2.png",ccp(37,53),0);
		sprite->addFragment_ResMgr("graystar60_frame3.png",ccp(54,40),0);
		sprite->addFragment_ResMgr("graystar60_frame4.png",ccp(46,22),0);
		sprite->addFragment_ResMgr("graystar60_frame5.png",ccp(28,21),0);
		sprite->addFragment_ResMgr("graystar60_spell_middle1.png",ccp(20,38),0);
		sprite->addFragment_ResMgr("graystar60_spell_middle2.png",ccp(27,31),0);
		sprite->addFragment_ResMgr("graystar60_spell_middle3.png",ccp(35,38),0);
		sprite->addFragment_ResMgr("graystar60_spell_middle4.png",ccp(41,36),0);
		sprite->addFragment_ResMgr("graystar60_spell_middle5.png",ccp(49,32),0);
	}
	else
	{
		sprite->splitSpriteToFragment();
	}
    // Define the dynamic body.
    //Set up a 1m squared box in the physics world
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(p.x, p.y);
    bodyDef.userData = sprite;
    b2Body *body = mWorld->CreateBody(&bodyDef);
    
    b2Vec2 trianglArr[][3] = {
        {b2Vec2(0.0f/PTM_RATIO,30.0f/PTM_RATIO),b2Vec2(-11.0f/PTM_RATIO,13.0f/PTM_RATIO),b2Vec2(11.0f/PTM_RATIO,13.0f/PTM_RATIO)}
        ,{b2Vec2(-11.0f/PTM_RATIO,13.0f/PTM_RATIO),b2Vec2(-29.0f/PTM_RATIO,10.0f/PTM_RATIO),b2Vec2(-15.0f/PTM_RATIO,-4.0f/PTM_RATIO)}
        ,{b2Vec2(-15.0f/PTM_RATIO,-4.0f/PTM_RATIO),b2Vec2(-18.0f/PTM_RATIO,-24.0f/PTM_RATIO),b2Vec2(0.0f/PTM_RATIO,-15.0f/PTM_RATIO)}
        ,{b2Vec2(0.0f/PTM_RATIO,-15.0f/PTM_RATIO),b2Vec2(18.0f/PTM_RATIO,-24.0f/PTM_RATIO),b2Vec2(15.0f/PTM_RATIO,-4.0f/PTM_RATIO)}
        ,{b2Vec2(11.0f/PTM_RATIO,13.0f/PTM_RATIO),b2Vec2(15.0f/PTM_RATIO,-4.0f/PTM_RATIO),b2Vec2(29.0f/PTM_RATIO,10.0f/PTM_RATIO)}      
    };
    
    for (int trigIndex=0; trigIndex<5; ++trigIndex) {
        b2PolygonShape dynamicBox;
        dynamicBox.Set(trianglArr[trigIndex], 3);
        
        // Define the dynamic body fixture.
        b2FixtureDef fixtureDef;
        fixtureDef.shape = &dynamicBox;	
        fixtureDef.density = desity;
        fixtureDef.friction = Fixture_Friction;
        fixtureDef.restitution = Fixture_Restitution;
        body->CreateFixture(&fixtureDef);
    }
    
    //中间那个五边形
    {
        b2Vec2 trianglArr[] = {
            b2Vec2(-11.0f/PTM_RATIO,13.0f/PTM_RATIO), b2Vec2(-15.0f/PTM_RATIO,-4.0f/PTM_RATIO), b2Vec2(0.0f/PTM_RATIO,-15.0f/PTM_RATIO),b2Vec2(15.0f/PTM_RATIO,-4.0f/PTM_RATIO),b2Vec2(11.0f/PTM_RATIO,13.0f/PTM_RATIO)
        };
        b2PolygonShape dynamicBox;
        dynamicBox.Set(trianglArr, 5);
        
        // Define the dynamic body fixture.
        b2FixtureDef fixtureDef;
        fixtureDef.shape = &dynamicBox;	
        fixtureDef.density = desity;
        fixtureDef.friction = Fixture_Friction;
        fixtureDef.restitution = Fixture_Restitution;
        body->CreateFixture(&fixtureDef);
    }
    
    if (angle != 0.0f) {
        body->SetTransform(body->GetPosition(), CC_DEGREES_TO_RADIANS(angle));
    }
    
    return body;
}

void BaseContentLayer::removeEmCannon(EMCannonSprite *em)
{
    for (size_t i=0; i<emVec.size(); i++) {
        if (emVec[i] == em) {
            emVec[i] = NULL;
        }
    }
}
//void BaseContentLayer::removeStrobe(Strobe *s)
//{
//    for (int i=0; i<strobeVec.size(); i++) {
//        if (strobeVec[i] == s) {
//            strobeVec[i] = NULL;
//        }
//    }
//}

//void BaseContentLayer::removeBoiler(Boiler *b)
//{
//    vector<Boiler*>::iterator it = boilerVec.begin();
//    for (; it!=boilerVec.end(); it++) {
//        if ((*it) == b) {
//            boilerVec.erase(it);
//            break;
//        }
//    }
//}


//创建闸门144的闸门
void BaseContentLayer::createGameToolsStrobe(GameToolsStrobe strobe)
{
    Strobe* sprite = Strobe::createStrobe(this,mWorld, strobe);
    addChild(sprite,-1,Tag_Box_Strobe);
	sprite->setIsKeyItem(strobe.is_key_item);
    
    //strobeVec.insert(strobeVec.end(), sprite);
    
    ShadowLayer::sharedShadowLayer()->createShadow(sprite);
}

b2Body* BaseContentLayer::createEMCannon(EMCannon emCannon)
{
    EMCannonSprite* sprite = new EMCannonSprite(emCannon,mWorld,this);
    sprite->autorelease();
    addChild(sprite,0,Tag_Box_EMCannoon);
    emVec.insert(emVec.end(), sprite);
    
//    ShadowLayer::sharedShadowLayer()->createShadow(sprite);
    
    return NULL;
}

b2Body* BaseContentLayer::createGameToolsRope(GameToolsRope toolsRope){
    //创建一个anchorPoint的body
    b2Vec2 anchorPoint = toolsRope.anchorPoint;
    b2Body* anchorBody;

	CustomeSprite* sprite = NULL;
    {
        sprite = new CustomeSprite;
        sprite->initWithFile(ResManager::getManager()->getSharedFilePath(g_gameinPath+"rope_anchor.png").c_str());
        addChild(sprite,0,Tag_Box_Rope_Anchor);
        sprite->release();
        sprite->setPosition(CCPointMake(anchorPoint.x*PTM_RATIO, anchorPoint.y*PTM_RATIO));
		sprite->setIsKeyItem(toolsRope.is_key_item);
        
        b2BodyDef bodyDef;
        bodyDef.type = b2_kinematicBody;
        bodyDef.position = anchorPoint;
        bodyDef.userData = sprite;
        anchorBody = mWorld->CreateBody(&bodyDef);
        
        b2PolygonShape box;
        box.SetAsBox(0.1f, 0.1f);
        
        b2FixtureDef fixtureDef;
        fixtureDef.shape = &box;
        fixtureDef.density = Fixture_Density;
        fixtureDef.friction = Fixture_Friction;
        fixtureDef.restitution = Fixture_Restitution;
        
        anchorBody->CreateFixture(&fixtureDef);
    }
    
    //创建悬吊物体
    b2Body* shapeToHang = createShape(toolsRope.shapeToHang);
    
    //创建rope
    {
        VRope* newRope = new VRope;
        newRope->initWithBodysAndRopeLenght(mWorld, anchorBody, shapeToHang, mRopeSpriteBatchNode, toolsRope.ropeLength);
        newRope->reset();
        mRopes.insert(mRopes.end(), newRope);
		newRope->setAnchorSprite(sprite);

		if (toolsRope.shapeToHang.type == GameShapeType_LocalTimeDecBomb || 
			toolsRope.shapeToHang.type == GameShapeType_Bomb48_DarkWhole )
		{
			newRope->setRopeBlockOpacity(newRope->getRopeBlockNum()-1,0);
		}
    }

	AchievementManager::getInstance()->createdARope((CCNode*)shapeToHang->GetUserData());
    
    return shapeToHang;
}
//上面是48的半圆，下面是48*16的box
b2Body* BaseContentLayer::createGameToolsBoiler(GameToolsBoiler toolsBoiler){
    Boiler* sprite = new Boiler();
    addChild(sprite, 0, Tag_Box_Boiler);
    sprite->release();
    sprite->setPosition(CCPointMake(toolsBoiler.point.x*PTM_RATIO, toolsBoiler.point.y*PTM_RATIO));
	sprite->setIsKeyItem(toolsBoiler.is_key_item);
	sprite->createSmokeBodies(mWorld);
	
	//解决小关卡界面直接进入游戏中时，会出现粒子效果显示在界面的问题（当元素从右边移动过来的时候，会造成界面是向中间靠拢的错觉）
#if SHOW_GUANKAMSG_LAYER == 0
	if (!LevelManager::getManager()->getNext() && LevelManager::getManager()->getShowLoadingAnimation())
	{
		sprite->runAction(CCSequence::create(CCDelayTime::create(2.0f),
			CCCallFunc::create(sprite, callfunc_selector(Boiler::start)), 
			NULL));
	}
	else
	{
		sprite->start();
	}
#else
	sprite->start();
#endif

    ShadowLayer::sharedShadowLayer()->createShadow(sprite);
    //boilerVec.insert(boilerVec.end(), sprite);

	boilerNum ++;
	// 如果数量太多，就使用lite版
	if (boilerNum >= 3)
	{
		CCLog("BaseContentLayer::createGameToolsBoiler too many boiler we will use lite version .");
		Boiler::setLite(Boiler::Lite_1);
	}else
	{
		Boiler::setLite(Boiler::Lite_no);
	}

	// 安卓，低配置
	if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	{
		Boiler::setLite(Boiler::Lite_2);
	}
	
    
    //按顺序检测下、左、右、上方是否有桌面
    b2Body* table = NULL;
    b2Vec2 testPoint;
    //下
    {
        testPoint = toolsBoiler.point + b2Vec2(0, -1.5);
        b2Body* testBody = whichBodyContainThePoint(testPoint);
        if (testBody != NULL) {
            CCSprite* sprite = (CCSprite*)testBody->GetUserData();
            if (sprite->getTag() == Tag_Box_Table || sprite->getTag() == Tag_Box_Table_Ineffective) {
                table = testBody;
            }
        }
    }
    //左
    if (table == NULL) {
        testPoint = toolsBoiler.point + b2Vec2(-1.5, 0);
        b2Body* testBody = whichBodyContainThePoint(testPoint);
        if (testBody != NULL) {
            CCSprite* sprite = (CCSprite*)testBody->GetUserData();
            if (sprite->getTag() == Tag_Box_Table || sprite->getTag() == Tag_Box_Table_Ineffective) {
                table = testBody;
            }
        }
    }
    //右
    if (table == NULL) {
        testPoint = toolsBoiler.point + b2Vec2(1.5, 0);
        b2Body* testBody = whichBodyContainThePoint(testPoint);
        if (testBody != NULL) {
            CCSprite* sprite = (CCSprite*)testBody->GetUserData();
            if (sprite->getTag() == Tag_Box_Table || sprite->getTag() == Tag_Box_Table_Ineffective) {
                table = testBody;
            }
        }
    }
    //上
    if (table == NULL) {
        testPoint = toolsBoiler.point + b2Vec2(0, 1.5);
        b2Body* testBody = whichBodyContainThePoint(testPoint);
        if (testBody != NULL) {
            CCSprite* sprite = (CCSprite*)testBody->GetUserData();
            if (sprite->getTag() == Tag_Box_Table || sprite->getTag() == Tag_Box_Table_Ineffective) {
                table = testBody;
            }
        }
    }
    if (table == NULL) {
        CCAssert(false, "锅炉的上下左右都没有桌面");
    }
    
    
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(toolsBoiler.point.x, toolsBoiler.point.y);
    bodyDef.userData = sprite;
    b2Body* bodyA = mWorld->CreateBody(&bodyDef);
    
    //半圆
    {
        b2Vec2 trianglArr[][8] = {
            {b2Vec2(0.0f/PTM_RATIO,12.0f/PTM_RATIO),b2Vec2(-5.0f/PTM_RATIO,11.5f/PTM_RATIO),b2Vec2(-10.5f/PTM_RATIO,10.0f/PTM_RATIO),b2Vec2(-16.0f/PTM_RATIO,6.0f/PTM_RATIO),b2Vec2(-20.0f/PTM_RATIO,1.0f/PTM_RATIO),b2Vec2(-23.0f/PTM_RATIO,-5.5f/PTM_RATIO),b2Vec2(-24.0f/PTM_RATIO,-12.0f/PTM_RATIO),b2Vec2(0.0f/PTM_RATIO,-12.0f/PTM_RATIO)},
            {b2Vec2(0.0f/PTM_RATIO,12.0f/PTM_RATIO),b2Vec2(0.0f/PTM_RATIO,-12.0f/PTM_RATIO),b2Vec2(12.0f/PTM_RATIO,-12.0f/PTM_RATIO),b2Vec2(23.0f/PTM_RATIO,-5.5f/PTM_RATIO),b2Vec2(20.0f/PTM_RATIO,1.0f/PTM_RATIO),b2Vec2(16.0f/PTM_RATIO,6.0f/PTM_RATIO),b2Vec2(10.5f/PTM_RATIO,10.0f/PTM_RATIO),b2Vec2(5.0f/PTM_RATIO,11.5f/PTM_RATIO)}
        };
        
        for (int trigIndex=0; trigIndex<2; ++trigIndex) {
            b2PolygonShape dynamicBox;
            dynamicBox.Set(trianglArr[trigIndex], 8);
            
            // Define the dynamic body fixture.
            b2FixtureDef fixtureDef;
            fixtureDef.shape = &dynamicBox;
            fixtureDef.density = Fixture_Density;
            fixtureDef.friction = Fixture_Friction;
            fixtureDef.restitution = Fixture_Restitution;
            bodyA->CreateFixture(&fixtureDef);
        }
    }
    
    //矩形
    {
		b2PolygonShape box;
		std::vector<b2PolygonShape> boxVec;
        
		box.SetAsBox(0.5f, 0.3f, b2Vec2(0.5f, -1.0f), 0);
		boxVec.insert(boxVec.end(), box);
		box.SetAsBox(0.5f, 0.3f, b2Vec2(-0.5f, -1.0f), 0);
		boxVec.insert(boxVec.end(), box);
        
		for (size_t i=0; i<boxVec.size(); ++i) {
			b2FixtureDef fixtureDef;
			fixtureDef.shape = &boxVec[i];
			fixtureDef.density = Fixture_Density;
			fixtureDef.friction = Fixture_Friction;
			fixtureDef.restitution = Fixture_Restitution;
            
			bodyA->CreateFixture(&fixtureDef);
		}
	}
    
    sprite->body = bodyA;
    
    
    //根据table旋转锅炉
    float angle = 0.0f;
    if (testPoint.y > toolsBoiler.point.y) {
        angle = 180;
    }else if (testPoint.x > toolsBoiler.point.x){
        angle = 90;
    }else if (testPoint.x < toolsBoiler.point.x){
        angle = -90;
    }
    
    if (angle != 0.0f) {
        bodyA->SetTransform(bodyA->GetPosition(), CC_DEGREES_TO_RADIANS(angle));
    }
    
    sprite->setAngle(angle);
    
    //锅炉和桌面加上焊接关节
    b2WeldJointDef jd;
    jd.Initialize(table, bodyA, testPoint);
    mWorld->CreateJoint(&jd);
    
    return bodyA;
}

b2Body* BaseContentLayer::createGameToolsSpringBed(GameToolsSpringBed toolsSpringBed){
    CustomeSprite* sprite = new CustomeSprite;
    sprite->initWithFile(ResManager::getManager()->getSharedFilePath(g_gameinPath+"spring_1.png").c_str());
    addChild(sprite, 0, Tag_Box_SpringBed);
    sprite->release();
    sprite->setPosition(CCPointMake(toolsSpringBed.point.x*PTM_RATIO, toolsSpringBed.point.y*PTM_RATIO));
//    sprite->setColor(ccRED);
    ShadowLayer::sharedShadowLayer()->createShadow(sprite);
    //按顺序检测下、左、右、上方是否有桌面
    b2Body* table = NULL;
    b2Vec2 testPoint;
    //下
    {
        testPoint = toolsSpringBed.point + b2Vec2(0, -1.0);
        b2Body* testBody = whichBodyContainThePoint(testPoint);
        if (testBody != NULL) {
            CCSprite* sprite = (CCSprite*)testBody->GetUserData();
            if (sprite->getTag() == Tag_Box_Table || sprite->getTag() == Tag_Box_Table_Ineffective) {
                table = testBody;
            }
        }
    }
    //左
    if (table == NULL) {
        testPoint = toolsSpringBed.point + b2Vec2(-1.0, 0);
        b2Body* testBody = whichBodyContainThePoint(testPoint);
        if (testBody != NULL) {
            CCSprite* sprite = (CCSprite*)testBody->GetUserData();
            if (sprite->getTag() == Tag_Box_Table || sprite->getTag() == Tag_Box_Table_Ineffective) {
                table = testBody;
            }
        }
    }
    //右
    if (table == NULL) {
        testPoint = toolsSpringBed.point + b2Vec2(1.0, 0);
        b2Body* testBody = whichBodyContainThePoint(testPoint);
        if (testBody != NULL) {
            CCSprite* sprite = (CCSprite*)testBody->GetUserData();
            if (sprite->getTag() == Tag_Box_Table || sprite->getTag() == Tag_Box_Table_Ineffective) {
                table = testBody;
            }
        }
    }
    //上
    if (table == NULL) {
        testPoint = toolsSpringBed.point + b2Vec2(0, 1.0);
        b2Body* testBody = whichBodyContainThePoint(testPoint);
        if (testBody != NULL) {
            CCSprite* sprite = (CCSprite*)testBody->GetUserData();
            if (sprite->getTag() == Tag_Box_Table || sprite->getTag() == Tag_Box_Table_Ineffective) {
                table = testBody;
            }
        }
    }
    if (table == NULL) {
        CCAssert(false, "弹簧床的上下左右都没有桌面");
    }
    
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(toolsSpringBed.point.x, toolsSpringBed.point.y);
    bodyDef.userData = sprite;
    b2Body* bodyA = mWorld->CreateBody(&bodyDef);
    
    b2PolygonShape box;
    box.SetAsBox(2.5f, 0.5f); //弹簧簧床尺寸120*24
    
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &box;
    fixtureDef.density = Fixture_Density;
    fixtureDef.friction = Fixture_Friction;
    fixtureDef.restitution = toolsSpringBed.restitution;
    bodyA->CreateFixture(&fixtureDef);
    
    //根据table旋转锅炉
    float angle = 0.0f;
    if (testPoint.y > toolsSpringBed.point.y) {
        angle = 180;
    }else if (testPoint.x > toolsSpringBed.point.x){
        angle = 90;
    }else if (testPoint.x < toolsSpringBed.point.x){
        angle = -90;
    }
    
    if (angle != 0.0f) {
        bodyA->SetTransform(bodyA->GetPosition(), CC_DEGREES_TO_RADIANS(angle));
    }
    
    //锅炉和桌面加上焊接关节
    b2WeldJointDef jd;
    jd.Initialize(table, bodyA, testPoint);
    mWorld->CreateJoint(&jd);
    
    return bodyA;
}



b2Body* BaseContentLayer::getCurrentRedStarFromWorld()
{
	for (b2Body* b = mWorld->GetBodyList(); b != NULL; b = b->GetNext())
	{        
		if (b->GetUserData() != NULL) {
			CCSprite* myActor = (CCSprite*)b->GetUserData();
			if (myActor->getTag() == Tag_Box_RedStar)
			{
				return b;
			}
		}
	}
	return NULL;
}

void BaseContentLayer::createSpecialItem(string fileStr, b2Vec2 pos)
{
	{
		//坐标偏移
		CCSize gWinSize = CCDirector::sharedDirector()->getWinSize();
		float32 oX = gWinSize.width/2/PTM_RATIO;// 就是当前的屏幕中心
		float32 oY = gWinSize.height/10/PTM_RATIO+5.5;// 就是10分之1的下面+5.5个单位  --> 76.8 + 5.5*24 = 208.8 --> 8.7的box2d位置

		ResManager* resManager = ResManager::getManager();
		CCSprite *itemImg = CCSprite::create(resManager->getSharedFilePath(g_daojuPath+fileStr).c_str());
		addChild(itemImg, 0, Tag_SpecialItem);
		itemImg->setScale(0.42f);
		itemImg->setPosition(ccp((oX+pos.x) *PTM_RATIO, (oY+pos.y) *PTM_RATIO));
		//道具背后的光效

		//if (getMachinePerformanceLevel() > 0)
		{
			CCSprite* daojux1 = CCSprite::create(resManager->getSharedFilePath(g_daojuPath+"daoju_xiaoguo_1.png").c_str());
			daojux1->setScale(1.0f/0.42f);
			daojux1->setPosition(ccp(itemImg->getContentSize().width/2,itemImg->getContentSize().height/2));
			daojux1->runAction(CCRepeatForever::create(CCSequence::create(CCRotateTo::create(2.0f,-180),CCRotateTo::create(2.0f,-360),NULL)));
			itemImg->addChild(daojux1,-1);
			CCSprite* daojux2 = CCSprite::create(resManager->getSharedFilePath(g_daojuPath+"daoju_xiaoguo_2.png").c_str());
			daojux2->setScale(1.0f/0.42f);
			daojux2->setPosition(ccp(itemImg->getContentSize().width/2,itemImg->getContentSize().height/2));
			daojux2->runAction(CCRepeatForever::create(CCSequence::create(CCRotateTo::create(1.5f,180),CCRotateTo::create(1.5f,360),NULL)));
			itemImg->addChild(daojux2,-2);
			CCSprite* daojux3 = CCSprite::create(resManager->getSharedFilePath(g_daojuPath+"daoju_xiaoguo_3.png").c_str());
			daojux3->setScale(1.0f/0.42f);
			daojux3->setPosition(ccp(itemImg->getContentSize().width/2,itemImg->getContentSize().height/2));
			itemImg->addChild(daojux3,-3);
		}
		
	}
}

#define ScaleRect(__RECT_VAR__,_SCALRE_NUM_) \
	__RECT_VAR__.origin.x += _SCALRE_NUM_ / 2.0f;\
	__RECT_VAR__.origin.y += _SCALRE_NUM_ / 2.0f;\
	__RECT_VAR__.size.width -= _SCALRE_NUM_;\
	__RECT_VAR__.size.height -= _SCALRE_NUM_

//碰撞到道具
void BaseContentLayer::specialToolCollide()
{
//屏蔽剧情道具相关
/*
	//正在修改
	//如果本关卡没有剧情道具不处理
	CCSprite *item = (CCSprite*) getChildByTag(Tag_SpecialItem); //特殊关卡道具
	if(!item)
		return;
	//如果本关卡已经获得剧情道具就不处理
	LevelManager* manager = LevelManager::getManager();

 	JuqingDaoJu daoju;
 	if(!JuqingDaoJuManager::getManager()->getItemInSection(manager->getLastSeasonId(),manager->getLastSectionId(),daoju))
 		return;
 	if(daoju.state != DJS_NotGetted)
 		return;

	//如果星星没有不处理
	CCSprite *star = (CCSprite*) getChildByTag(Tag_Box_RedStar); //星星
	if(!star)
		return;
	
	CCRect r1 = item->boundingBox();
	CCRect r2 = star->boundingBox();
	ScaleRect(r1,30.0f);
	ScaleRect(r2,20.0f);

	if(r1.intersectsRect(r2)){

		CCUserDefault::sharedUserDefault()->setBoolForKey(Juqingdaoju_Teach_Is_Done, true);
		CCUserDefault::sharedUserDefault()->flush();
		Music::sharedMusicPlayer()->playEffectSound("juqingdaoju_souji.wav");

		if (manager->getLastSeasonId() == 1 && manager->getLastSectionId() == 5)
		{
			GuideLayer_JuqingDaoju* pLayer = dynamic_cast<GuideLayer_JuqingDaoju*>(BaseScense::getCurrentScene()->getGuideLayer());
			if (pLayer)
			{
				if (CCSprite* pSpr = pLayer->getTipSpr())
				{
					pSpr->runAction(CCSequence::create(CCFadeOut::create(0.2f), CCRemoveSelf::create(), NULL));
				}
			}
		}

		CCSize winsize = CCDirector::sharedDirector()->getWinSize();
		CCPoint point = item->getPosition();
		CCPoint wpoint = item->getParent()->convertToWorldSpace(point);

		if(CCDirector::sharedDirector()->getRunningScene()->getChildByTag(Tag_SpecialItem_Copy))
			CCDirector::sharedDirector()->getRunningScene()->removeChildByTag(Tag_SpecialItem_Copy);
		GameLayerInfo* gameLayer = GameLayerFactory::getInstance()->getGameLayer(manager->getLastSeasonId(),manager->getLastSectionId());
		float bili = gameLayer->initScale / 1.0f;
		CCSprite *itemImg = CCSprite::create();
		itemImg->initWithTexture(item->getTexture());
		itemImg->setPosition(wpoint);
		itemImg->setScale(0.42f*bili);
		itemImg->setTag(Tag_SpecialItem_Copy);
		CCSprite* daojux1 = CCSprite::create(ResManager::getManager()->getSharedFilePath(g_daojuPath+"daoju_xiaoguo_1.png").c_str());
		daojux1->setScale(1.0f/0.42f);
		daojux1->setPosition(ccp(itemImg->getContentSize().width/2,itemImg->getContentSize().height/2));
		daojux1->runAction(CCRepeatForever::create(CCSequence::create(CCRotateTo::create(2.0f,-180),CCRotateTo::create(2.0f,-360),NULL)));
		itemImg->addChild(daojux1,-1);
		CCSprite* daojux2 = CCSprite::create(ResManager::getManager()->getSharedFilePath(g_daojuPath+"daoju_xiaoguo_2.png").c_str());
		daojux2->setScale(1.0f/0.42f);
		daojux2->setPosition(ccp(itemImg->getContentSize().width/2,itemImg->getContentSize().height/2));
		daojux2->runAction(CCRepeatForever::create(CCSequence::create(CCRotateTo::create(1.5f,180),CCRotateTo::create(1.5f,360),NULL)));
		itemImg->addChild(daojux2,-2);
		CCSprite* daojux3 = CCSprite::create(ResManager::getManager()->getSharedFilePath(g_daojuPath+"daoju_xiaoguo_3.png").c_str());
		daojux3->setScale(1.0f/0.42f);
		daojux3->setPosition(ccp(itemImg->getContentSize().width/2,itemImg->getContentSize().height/2));
		itemImg->addChild(daojux3,-3);
		CCDirector::sharedDirector()->getRunningScene()->addChild(itemImg, 1000);

		item->removeFromParent();

		CCPoint pos = MenuLayer::getCurrentLayer()->getJuqingDaojuPos(daoju.daojuId);
		itemImg->runAction(CCSequence::create(
			CCScaleTo::create(0.5f,0.5f),
			CCMoveTo::create(0.5f,pos),
			CCFadeOut::create(0.2f),
			CCCallFuncN::create(this,SEL_CallFuncN(&BaseContentLayer::getJuqingDaoJu)),
			NULL));
// 			itemImg->runAction(CCSequence::create(
// 				CCScaleTo::create(0.5f,0.5f),
// 				CCEaseSineOut::create(CCMoveTo::create(0.5f,ccp(56,winsize.height - 260))),
// 				NULL));

		JuqingDaoJuManager::getManager()->setItemIsGettedState(manager->getLastSeasonId(),manager->getLastSectionId(),DJS_GettedByGame);
	}
*/
	//CCSize winsize = CCDirector::sharedDirector()->getWinSize();

	//CCRenderTexture* render = CCRenderTexture::create(winsize.width,winsize.height);
	////render->setVisible(false);
	//if(CollisionDetection::GetInstance()->areTheSpritesColliding(star,item,true,render)){
	//	//碰撞到了
	//	item->runAction(CCSequence::create(CCFadeOut::create(0.3f),CCRemoveSelf::create(item),NULL));
	//	//获取道具
	//	//JuqingDaoJuManager::getManager()->setItemIsGettedState(manager->getLastSeasonId(),manager->getLastSectionId(),DJS_GettedByGame);
	//	CCLog("ok");
	//}
}
void BaseContentLayer::getJuqingDaoJu(CCNode *node)
{
	//屏蔽剧情道具相关
/*
	node->removeFromParent();
	JuqingDaoJu daoju;
	LevelManager* manager = LevelManager::getManager();
	JuqingDaoJuManager::getManager()->getItemInSection(manager->getLastSeasonId(),manager->getLastSectionId(),daoju);
	MenuLayer::getCurrentLayer()->getJuqingDaoju(daoju);
*/
}

void BaseContentLayer::recordEventID(RecordSprite* pNode, int iEventID)
{
	if (m_iCurPlayMode != playMode_Normal)
	{
		return;
	}
	if (m_bRecord && pNode && m_iCurRecordFrame > 0 && m_iCurRecordFrame <= (int)m_AllFramesData.size())
	{
		//在当前记录事件元素里面的才能注册事件
		bool bCurScene = false;
		for (size_t i = 0; i < m_AllNeedRecordSpriteVec.size(); i++)
		{
			if (pNode == m_AllNeedRecordSpriteVec.at(i))
			{
				bCurScene = true;
				break;
			}
		}
		if (!bCurScene)
		{
			return;
		}
		pNode->m_SaveData.eventId = iEventID;

		//如果当前帧没有记录，那么就新增，如果有记录，那就直接修改
		bool bFind = false;
		for (size_t i = 0; i < m_CurFrameData.size(); i++)
		{
			if (m_CurFrameData.at(i).id == pNode->m_SaveData.id)
			{
				bFind = true;
				if (m_CurFrameData.at(i).eventId != eventID_Touch)
				{
					m_CurFrameData.at(i).eventId = iEventID;
				}
				break;
			}
		}

		if (!bFind)
		{
			if (iEventID != eventID_Remove && iEventID != eventID_Touch)
			{
				m_CurNeedRecordSpriteVec.push_back(pNode);
			}
			else
			{
				m_CurFrameData.push_back(pNode->m_SaveData);
			}
		}
	}
}

void BaseContentLayer::MovementEventCallFunc(CCArmature *pArmature, MovementEventType movementType, const char *movementID)
{
	std::string id = movementID;

	if (movementType == LOOP_COMPLETE)
	{
	}
	else if (movementType == COMPLETE)
	{
		if (id.compare(g_ThiefAnimation_siwang.c_str()) == 0)
		{
			m_pArmature->removeFromParent();
			m_pArmature = NULL;
			failed(0.0f);
		}
		else if (id.compare(g_ThiefAnimation_jingxia.c_str()) == 0)
		{
			this->runAction(CCSequence::create(CCDelayTime::create(0.1f), 
				CCCallFuncN::create(this, callfuncN_selector(BaseContentLayer::playZuo)),
				NULL));
		}
		else if (id.compare(g_ThiefAnimation_zuoxia.c_str()) == 0)
		{
			showBaseContentLayerInAnimation();
		}
		else if (id.compare("boom") == 0)
		{
			pArmature->removeFromParent();
		}
		else
		{
			bool bFind = false;
			for (size_t i = 0; i < m_tempRandomArmatureVec.size(); i++)
			{
				if (id == m_tempRandomArmatureVec.at(i))
				{
					bFind = true;
					m_tempRandomArmatureVec.erase(m_tempRandomArmatureVec.begin()+i);

					if (m_tempRandomArmatureVec.size() <= 0)
					{
						for (size_t k = 0; k < m_randomArmatureVec.size(); k++)
						{
							if (id == m_randomArmatureVec.at(k))
							{
								std::swap(m_randomArmatureVec.at(0), m_randomArmatureVec.at(k));
								break;
							}
						}
						m_tempRandomArmatureVec = m_randomArmatureVec;
					}

					this->runAction(CCSequence::create(CCDelayTime::create(1.2f), 
						CCCallFunc::create(this, callfunc_selector(BaseContentLayer::runNoOpAction)),
						NULL));
					break;
				}
			}
		}
	}
	else if (movementType == START)
	{

	}
}

void BaseContentLayer::playZuoXia(CCNode* pNode)
{
	m_pArmature->getAnimation()->play(g_ThiefAnimation_zuoxia.c_str());
}

void BaseContentLayer::playZuo(CCNode* pNode)
{
	m_pArmature->getAnimation()->play(g_ThiefAnimation_zuo.c_str());
	m_CollisionWithRoleNode.clear();
}

void BaseContentLayer::setRoleVisible(bool e)
{
	if (m_pArmature)
	{
		m_pArmature->setVisible(e);
	}
}

static const int iTag_Menu = 11110;
static const int iTag_ColorLayer = 11011;
static const int iTag_Btn_Play = 11111;
static const int iTag_Btn_Pause = 11112;

void BaseContentLayer::showPlayOrPauseBtn(cocos2d::extension::CCScrollView* pParent, bool bPlay)
{
	if (m_iCurPlayMode != playMode_PlayGameRecord)
	{
		return;
	}
	CCNode* pLayer = pParent->getContainer();
	CCMenu* pMenu = (CCMenu*)pLayer->getChildByTag(iTag_Menu);
	if (pMenu)
	{
		pMenu->removeFromParent();
	}
	CCLayerColor * pLayercolor = (CCLayerColor*)pLayer->getChildByTag(iTag_ColorLayer);
	if (pLayercolor)
	{
		pLayercolor->removeFromParent();
	}
	pMenu = CCMenu::create();
	pMenu->setPosition(CCPointZero);
	pParent->addChild(pMenu, 2, iTag_Menu);
	pLayercolor = CCLayerColor::create(ccc4(0.0f, 0.0f, 0.0f, 75.0f));
	pLayercolor->setPosition(CCPointZero);
	pParent->addChild(pLayercolor, 1, iTag_ColorLayer);

	ResManager* resManager = ResManager::getManager();
	CCMenuItemImage* pPlay = NULL;
	pPlay =	CCMenuItemImage::create(
		resManager->getSharedFilePath(g_stop+"ZT_gongluetu_bofang.png").c_str(),
		resManager->getSharedFilePath(g_stop+"ZT_gongluetu_bofang.png").c_str(),
		this,SEL_MenuHandler(&BaseContentLayer::playOrPause));
	pPlay->setPosition(ccp(pParent->getContentSize().width/2 - (mScreenSize.width/2-designResolutionSize.width/2), pParent->getContentSize().height/2-(mScreenSize.height/2-designResolutionSize.height/2)));
	if (bPlay)
	{
		pPlay->setTag(iTag_Btn_Play);
	}
	else
	{
		pPlay->setTag(iTag_Btn_Pause);
	}
	setPlayRecord(false);
	pMenu->addChild(pPlay);

	if (m_AllFramesData.size() <= 0)
	{
		pMenu->setTouchEnabled(false);
	}

}

void BaseContentLayer::playOrPause(CCObject* pObj)
{
	Music::sharedMusicPlayer()->buttonClicked();
	ResManager* resManager = ResManager::getManager();
	CCMenuItemImage* pBtn = (CCMenuItemImage*)pObj;
	if (pBtn->getTag() == iTag_Btn_Play)
	{
		setPlayRecord(true);
		starPlay();
// 		SpriteHelper::resumeSubNodeAndChildrenInParentNodeByTag(this,Tag_Box_Boiler);
// 		SpriteHelper::resumeSubNodeAndChildrenInParentNodeByTag(this,Tag_Box_RedStar);
// 		SpriteHelper::resumeNodeAndChildren(this);
// 		SpriteHelper::resumeNodeAndChildren(this->getParent()->getChildByTag(Tag_Layer_Background_Cover));
// 
// 		//恢复碎片
// 		{
// 			SpriteHelper::resumeSubNodeAndChildrenInParentNodeByTag(this,Tag_Fragment);
// 		}
		if (pBtn->getParent()->getParent()->getChildByTag(iTag_ColorLayer))
		{
			pBtn->getParent()->getParent()->getChildByTag(iTag_ColorLayer)->removeFromParent();
		}
		pBtn->getParent()->removeFromParent();

// 		pBtn->initWithNormalImage(resManager->getSharedFilePath(g_buttonPath+"set_help.png").c_str(),
// 			resManager->getSharedFilePath(g_buttonPath+"set_help_select.png").c_str(),
// 			resManager->getSharedFilePath(g_buttonPath+"set_help.png").c_str(),
// 			this,SEL_MenuHandler(&BaseContentLayer::playOrPause));
// 		pBtn->setTag(iTag_Btn_Pause);
	}
	else if (pBtn->getTag() == iTag_Btn_Pause)
	{
		setPlayRecord(false);

		pBtn->initWithNormalImage(resManager->getSharedFilePath(g_buttonPath+"Play_buttom.png").c_str(),
			resManager->getSharedFilePath(g_buttonPath+"Play_buttom_select.png").c_str(),
			resManager->getSharedFilePath(g_buttonPath+"Play_buttom.png").c_str(),
			this,SEL_MenuHandler(&BaseContentLayer::playOrPause));
		pBtn->setTag(iTag_Btn_Play);

		SpriteHelper::pauseSubNodeAndChildrenInParentNodeByTag(this,Tag_Box_Boiler);
		SpriteHelper::pauseSubNodeAndChildrenInParentNodeByTag(this,Tag_Box_RedStar);
		SpriteHelper::pauseNodeAndChildren(this);
		SpriteHelper::pauseNodeAndChildren(this->getParent()->getChildByTag(Tag_Layer_Background_Cover));

		// 暂停碎片
		{
			SpriteHelper::pauseSubNodeAndChildrenInParentNodeByTag(this,Tag_Fragment);
		}
	}
}