//
//  GameScenseDefine.cpp
//  demo-box2d
//
//  Created by Davidloeng loeng on 12-6-26.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#include <iostream> 
#include "GameScenseDefine.h"
#include "CommonDefine.h"
#include "BaseScense.h"
#include "CurtainLayer.h"
#include "CurtainSideLayer.h"
#include "GameLayerFactory.h"
#include "BaseContentWrapLayer.h"
#include "VersionControl.h"
#include "DoorLayer.h"
#include "JuqingDaoJuManager.h"
#include "History.h"
#include "GuideLayerFactory.h"
#include "LevelManager.h"
#include "ShadowLayer.h"
#include "Uimsg.h"
#include "ScrollController.h"
#include "MessageBar.h"
#include "AppMacros.h"
USING_NS_CC_EXT;

extern const int g_iTest_Record;

GameScenseDefine* GameScenseDefine::_GameScenseDefine = NULL;

GameScenseDefine::GameScenseDefine(){

    CCSize gWinSize = CCDirector::sharedDirector()->getWinSize();
}

GameScenseDefine::~GameScenseDefine(){
    CC_SAFE_DELETE(_GameScenseDefine);
}

GameScenseDefine* GameScenseDefine::SharedGameScenseDefine(){
    if (_GameScenseDefine == NULL) {
        _GameScenseDefine = new GameScenseDefine;
    }
    
    return _GameScenseDefine;
}

int GameScenseDefine::getSceneTotalCount(){

	return GameLayerFactory::getInstance()->getTotalSectionNum();
}

CCScene* GameScenseDefine::createScenseByIndex(int index,bool needCurtain/* = true*/){

	if (index >= getSceneTotalCount()) {
		CCLog("%s, %d, impossible index=%d", __FILE__, __LINE__, index);
		return NULL;
	}
	GameLayerFactory* layerFactory = GameLayerFactory::getInstance();
	std::pair<int, int> info1 = layerFactory->getLevelFormat(index);
	int seasonId = info1.first;
	int sectionId = info1.second;
    
    if (seasonId == 1 && sectionId == 2)
    {
        CCUserDefault::sharedUserDefault()->setBoolForKey(Show_Guide_1_2_Teach, needCurtain);
        CCUserDefault::sharedUserDefault()->flush();
    }
	GameInforEditer::getGameInfor()->setLastClickId(seasonId, sectionId);
 	BaseScense* scense = new BaseScense(index);
 	scense->autorelease();

	GameLayerInfo* gameLayer = GameLayerFactory::getInstance()->getGameLayer(seasonId,sectionId);
	scense->getBaseContentWrapLayer()->setWrapperInitScaleWithCenterPoint(gameLayer->initScale, gameLayer->initScaleCenterPos,false);

	BaseContentLayer* layer = NULL;
	if (g_iTest_Record != 0)
	{
		vector<vector<record> > allRecordData;
		if (!Recorder::getInstance()->getOneSectionRecord(index, &allRecordData))
		{
			MessageBar::show(LFStrings::getValue("NODataFound").c_str());
			//CCMessageBox("Can't found the record data!", "Error");
			layer = new BaseContentLayer(scense);
			scense->setCollisionEnable(true);
		}
		else
		{
			layer = new BaseContentLayer(scense, playMode_PlayGameRecord, &allRecordData);
			layer->starPlay();
			layer->setPlayRecord(true);
		}
	}
	else
	{
		layer = new BaseContentLayer(scense);
	}
	layer->autorelease();	
	scense->setContentLayer(layer);
	scense->getBaseContentWrapLayer()->getWrapper()->addChild(layer, 10, Tag_Layer_BaseContentLayer);

	b2Body* body = NULL;
	for (size_t i=0; i<gameLayer->gameTableVec.size(); ++i) {
		body= layer->createTable(gameLayer->gameTableVec[i]);
	}
	for (size_t i=0; i<gameLayer->gameShapeVec.size(); ++i) {
		body= layer->createShape(gameLayer->gameShapeVec[i]);
	}
	for (size_t i=0; i<gameLayer->throwPointVec.size(); ++i) {
		layer->createThrowPoint(gameLayer->throwPointVec[i]);
	}
    
    for (size_t i=0; i<gameLayer->springBedVec.size(); ++i) {
		body=  layer->createGameToolsSpringBed(gameLayer->springBedVec[i]);
	}
    for (size_t i=0; i<gameLayer->boilerVec.size(); ++i) {
        body= layer->createGameToolsBoiler(gameLayer->boilerVec[i]);
	}
    //创建闸门
    for (size_t i=0; i<gameLayer->strobeVec.size(); i++) {
        layer->createGameToolsStrobe(gameLayer->strobeVec[i]);
    }
    for (size_t i=0; i<gameLayer->emCannonVec.size(); i++) {
        layer->createEMCannon(gameLayer->emCannonVec[i]);
	}
	for (size_t i=0; i<gameLayer->ropeVec.size(); ++i) {
		body= layer->createGameToolsRope(gameLayer->ropeVec[i]);
	}
	//屏蔽剧情道具相关
/*
	// 获取某个关卡配置的道具 // 没有就返回false //该关卡有道具、且没有获得过该道具
	JuqingDaoJu data;
	if (JuqingDaoJuManager::getManager()->getItemInSection(seasonId, sectionId, data)
		&& data.state == DJS_NotGetted)
	{
		layer->createSpecialItem(data.fileName_WithOutWhiteBoarder, data.posInGame);
	}
    
 */   
	layer->setFailedWhenRedStarFall(gameLayer->failedWhenRedStarFall);
    layer->setRedStarBeginEffevtive(gameLayer->needRedStarBeginEffective);
	layer->setMinClickNum(gameLayer->min_click_num);
	layer->setMinTimeCost(gameLayer->min_time_cost);


	// 更新完的次数
	GameInforEditer::getGameInfor()->incSectionPlayTime(seasonId,sectionId);
	// 扣除星星
	//{
	//	// 还需要等待时间，那么就用星星买时间
	//	// 所以对于1-2，本来就不要星星，本来时间也是0，所以不扣星星
	//	// 对于1-12.第一次玩，如果星星不够，等待时间还没到就报错
	//	// 如果星星不够，等待时间到了就不要钱了
	//	if (GameInforEditer::getGameInfor()->getTimeToWaitForFirstPlay(seasonId,sectionId) > 0)
	//	{
	//		// 如果是1-1~1-11是不需要星星的，所以免费玩
	//		// 如果是1-12是要5颗的，但是玩过了就是0颗了
	//		int needStar = GameInforEditer::getGameInfor()->getStarCostForFirstPlay(seasonId,sectionId);

	//		if (needStar > GameInforEditer::getGameInfor()->getTotalFreeScore())
	//		{
	//			CCAssert(false,"");
	//		}
	//		GameInforEditer::getGameInfor()->subMyStarNum(needStar);
	//	}
	//	
	//}
	// 标记为玩过了
	{
		SectionInfor* sectionInfor = GameInforEditer::getGameInfor()->getSectionInfor(seasonId,sectionId);
		if (!GameInforEditer::getGameInfor()->isIgnoreAllLock())
		{
			CCAssert(sectionInfor != NULL,"");
		}
		if (sectionInfor)
		{
			sectionInfor->locked = false;
			sectionInfor->isPlayed = true;
		}
	}

	return scense;
}


CCScrollView* GameScenseDefine::playRecord(int seasonID, int sectionID, std::vector< std::vector<record> >* allRecordData, bool bAutoPlay /*= false*/)
{
	if (!allRecordData)
	{
		CCAssert(false, "allRecordData not be null when playRecord");
		return NULL;
	}
	BaseScense* pScene = BaseScense::getCurrentScene();
	//
	BaseContentWrapLayer* pW = BaseContentWrapLayer::createLayer(pScene);

	pScene->setContentLayerWapper(pW, false);

	ShadowLayer* pShadowLayer = new ShadowLayer();
	pShadowLayer->autorelease();

	CCLayer* pBgCover = CCLayer::create();
	pW->getWrapper()->addChild(pShadowLayer,0,Tag_Layer_shadow);
	pW->getWrapper()->addChild(pBgCover,-1,Tag_Layer_Background_Cover);

	GameLayerInfo* gameLayer = GameLayerFactory::getInstance()->getGameLayer(seasonID, sectionID);

	pW->setWrapperInitScaleWithCenterPoint(gameLayer->initScale, gameLayer->initScaleCenterPos,false);

	BaseContentLayer* layer = new BaseContentLayer(pScene, playMode_PlayGameRecord, allRecordData);
	layer->autorelease();
	layer->setLayerFreezy(true);
	layer->setRecord(false);
	pW->getWrapper()->addChild(layer, 100, Tag_Layer_BaseContentLayer);
	pW->setBaseContentLayer(layer);
		
//	pW->getWrapper()->setPosition(tmpPoint);

	BaseBackgroundLayer* pb =  new BaseBackgroundLayer(LevelManager::getManager()->getLastSeasonId(),LevelManager::getManager()->getLastSectionId());
	pb->autorelease();
	pW->addChild(pb, -1, Tag_Layer_Background);
	pb->setBaseContentLayer(layer);

	if (allRecordData->size() > 0)
	{
		b2Body* body = NULL;
		for (size_t i=0; i<gameLayer->gameTableVec.size(); ++i) {
			body= layer->createTable(gameLayer->gameTableVec[i]);
		}
		for (size_t i=0; i<gameLayer->gameShapeVec.size(); ++i) {
			body= layer->createShape(gameLayer->gameShapeVec[i]);
		}
		for (size_t i=0; i<gameLayer->throwPointVec.size(); ++i) {
			layer->createThrowPoint(gameLayer->throwPointVec[i]);
		}

		for (size_t i=0; i<gameLayer->springBedVec.size(); ++i) {
			body=  layer->createGameToolsSpringBed(gameLayer->springBedVec[i]);
		}
		for (size_t i=0; i<gameLayer->boilerVec.size(); ++i) {
			body= layer->createGameToolsBoiler(gameLayer->boilerVec[i]);
		}
		//创建闸门
		for (size_t i=0; i<gameLayer->strobeVec.size(); i++) {
			layer->createGameToolsStrobe(gameLayer->strobeVec[i]);
		}
		for (size_t i=0; i<gameLayer->emCannonVec.size(); i++) {
			layer->createEMCannon(gameLayer->emCannonVec[i]);
		}
		for (size_t i=0; i<gameLayer->ropeVec.size(); ++i) {
			body= layer->createGameToolsRope(gameLayer->ropeVec[i]);
		}
	}

	layer->setFailedWhenRedStarFall(gameLayer->failedWhenRedStarFall);
	layer->setRedStarBeginEffevtive(gameLayer->needRedStarBeginEffective);
	layer->setMinClickNum(gameLayer->min_click_num);
	layer->setMinTimeCost(gameLayer->min_time_cost);


	CCScrollView* pview = CCScrollView::create();
	pview->ignoreAnchorPointForPosition(false);
	pview->setAnchorPoint(ccp(0.5f, 0.5f));
	pview->setPosition(ccp(CCDirector::sharedDirector()->getWinSize().width/2, CCDirector::sharedDirector()->getWinSize().height/2));
	pview->setViewSize(designResolutionSize);
//	pview->setViewSize(CCDirector::sharedDirector()->getWinSize());
	pview->setScale(0.68f);
	pview->setTouchEnabled(false);

	pview->addChild(pW, 1, Tag_Layer_PlayRecordBaseContetWapper);
	pW->setPositionX(pview->getViewSize().width/2-pW->getContentSize().width/2*pW->getScaleX());
	pW->setPositionY(pview->getViewSize().height/2-pW->getContentSize().height/2*pW->getScaleY());


	layer->starPlay();
	layer->setPlayRecord(bAutoPlay);

	if (!bAutoPlay)
	{
		layer->showPlayOrPauseBtn(pview, true);
	}

	return pview;
}

void GameScenseDefine::createBaseContentLayer(int seasonid, int sectionid)
{
	LevelManager::getManager()->setLastSeasonId(seasonid);
	LevelManager::getManager()->setLastSectionId(sectionid);

	GameInforEditer::getGameInfor()->setLastClickId(seasonid, sectionid);

	BaseScense* pScene = BaseScense::getCurrentScene();
	//
	BaseContentWrapLayer* pW = pScene->getBaseContentWrapLayer();
	pW->removeAllChildren();
	pW->setWapper(CCLayer::create());

	ScrollController* pS = new ScrollController();
	pS->autorelease();
	pW->setScrollControlerr(pS);

	ShadowLayer* pShadowLayer = new ShadowLayer();
	pShadowLayer->autorelease();

	CCLayer* pBgCover = CCLayer::create();
	pW->getWrapper()->addChild(pShadowLayer,0,Tag_Layer_shadow);
	pW->getWrapper()->addChild(pBgCover,-1,Tag_Layer_Background_Cover);

	int sectionIndex = GameLayerFactory::getInstance()->getSectionIndex(seasonid, sectionid);
	pScene->setSectionIndex(sectionIndex);
	
	MenuLayer* pMenuLayer = new MenuLayer(sectionIndex);
	pMenuLayer->autorelease();
	pScene->setMenuLayer(pMenuLayer);
	
	BaseBackgroundLayer* pb = new BaseBackgroundLayer(LevelManager::getManager()->getLastSeasonId(),LevelManager::getManager()->getLastSectionId());
	pb->autorelease();
	pW->addChild(pb, -1, Tag_Layer_Background);
	pScene->setBackgroudLayer(pb);

	//添加底部星星评价
	const ConfigSectionInfo* curLevelinfo = GameLayerFactory::getInstance()->getConfigSectionInfo(seasonid, sectionid);
	StarState* pBar = StarState::create(curLevelinfo->iPefect_Gold, curLevelinfo->iBest_Gold);
	pBar->setPosition(ccp(0, -g_iTaiJieHight));
	pW->addChild(pBar, 10001, Tag_StarStateBar);
	pMenuLayer->setStarState(pBar);
	
	BaseGuideLayer* guideLayer = GuideLayerFactory::getFactory()->getGuideLayer(seasonid,sectionid);
	guideLayer->setBaseScene(pScene);
	pScene->setGuideLayer(guideLayer);
	guideLayer->setVisible(false);

	GameLayerInfo* gameLayer = GameLayerFactory::getInstance()->getGameLayer(seasonid, sectionid);
	pW->setWrapperInitScaleWithCenterPoint(gameLayer->initScale, gameLayer->initScaleCenterPos,false);


	BaseContentLayer* layer = NULL;
	if (g_iTest_Record != 0)
	{
		vector<vector<record> > allRecordData;
		if (!Recorder::getInstance()->getOneSectionRecord(GameLayerFactory::getInstance()->getSectionIndex(seasonid,sectionid), &allRecordData))
		{
			//MessageBar::show(LFStrings::getValue("NODataFound").c_str());
			//CCMessageBox("Can't found the record data!", "Error"); 
			layer = new BaseContentLayer(pScene);
			pScene->setCollisionEnable(true);
		}
		else
		{
			layer = new BaseContentLayer(pScene, playMode_PlayGameRecord, &allRecordData);
			layer->starPlay();
			layer->setPlayRecord(true);
		}
	}
	else
	{
		layer = new BaseContentLayer(pScene);
	}
	layer->autorelease();
	pW->getWrapper()->addChild(layer, 0, Tag_Layer_BaseContentLayer);
	
	pScene->setContentLayer(layer);

	b2Body* body = NULL;
	for (size_t i=0; i<gameLayer->gameTableVec.size(); ++i) {
		body= layer->createTable(gameLayer->gameTableVec[i]);
	}
	for (size_t i=0; i<gameLayer->gameShapeVec.size(); ++i) {
		body= layer->createShape(gameLayer->gameShapeVec[i]);
	}
	for (size_t i=0; i<gameLayer->throwPointVec.size(); ++i) {
		layer->createThrowPoint(gameLayer->throwPointVec[i]);
	}

	for (size_t i=0; i<gameLayer->springBedVec.size(); ++i) {
		body=  layer->createGameToolsSpringBed(gameLayer->springBedVec[i]);
	}
	for (size_t i=0; i<gameLayer->boilerVec.size(); ++i) {
		body= layer->createGameToolsBoiler(gameLayer->boilerVec[i]);
	}
	//创建闸门
	for (size_t i=0; i<gameLayer->strobeVec.size(); i++) {
		layer->createGameToolsStrobe(gameLayer->strobeVec[i]);
	}
	for (size_t i=0; i<gameLayer->emCannonVec.size(); i++) {
		layer->createEMCannon(gameLayer->emCannonVec[i]);
	}
	for (size_t i=0; i<gameLayer->ropeVec.size(); ++i) {
		body= layer->createGameToolsRope(gameLayer->ropeVec[i]);
	}

	layer->setFailedWhenRedStarFall(gameLayer->failedWhenRedStarFall);
	layer->setRedStarBeginEffevtive(gameLayer->needRedStarBeginEffective);
	layer->setMinClickNum(gameLayer->min_click_num);
	layer->setMinTimeCost(gameLayer->min_time_cost);
}










