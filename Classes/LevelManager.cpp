#include "LevelManager.h"
#include "SectionSelectorLayer.h"
#include "ResManager.h"
#include "GameLayerFactory.h"
#include "MaskLayer.h"
#include "CurtainLayer.h"
#include "CurtainSideLayer.h"
#include "SeasonSelectorLayer.h"
#include "InitGuideLayer2.h"
#include "TexturePreLoad.h"
#include "Music.h"
#include "CurtainLayerClosedFrame.h"
#include "VersionControl.h"
#include "DoorLayer.h"
#include "History.h"
#include "Starseneui.h"
#include "BaseScense.h"

LevelManager* LevelManager::manager;

LevelManager* LevelManager::getManager()
{
	if (manager == NULL)
		manager = new LevelManager();
	return manager;
}
LevelManager::LevelManager()
{
	m_iLastSeasonId = 1;
	m_iLastSectionId = 1;
	needUnlockSeasonId = -1;
	needScrollToSeasonId = -1;
	m_bShowLoadingAnimation = true;
	m_bNext = false;
	m_bStartApp = true;
	m_bSlideToPage3 = false;
	m_bShowUnlockAni = false;
}

CCLayer* LevelManager::createSectionLayer(int seasonId)
{
	//ResManager::getManager()->setPrefixPath("global_public_pic/");//用全局的图片
	SectionSelectorLayer* layer = SectionSelectorLayer::createLayer(seasonId,m_iLastSeasonId,m_iLastSectionId,this);
	m_iLastSeasonId = seasonId;

	return layer;
}

void LevelManager::GameInToMainLayer()
{
	setNext(false);
	ResManager::getManager()->removeAllArmatureInfo();
	LoadingLayer* layer = new LoadingLayer(Intent_GameIn2Main);
	CCDirector::sharedDirector()->getRunningScene()->addChild(layer,1000);
	layer->release();
}

void LevelManager::SectionToGameIn()
{
	setNext(false);
	LoadingLayer* layer = new LoadingLayer(Intent_Section2GameIn,m_iLastSeasonId);
	CCScene* pScene = CCDirector::sharedDirector()->getRunningScene();
	pScene->addChild(layer);
	layer->release();
}

void LevelManager::goToInitGuideUI()
{
	CCScene* scene = CCScene::create();
	InitGuideLayer2* layer = new InitGuideLayer2();
	scene->addChild(layer);layer->release();
	CCTransitionFade* tran = CCTransitionFade::create(0.5,scene);
	CCDirector::sharedDirector()->replaceScene(tran);
}
void LevelManager::goToSeasonUI(bool fromSection/* = false*/)							// 到season选择的界面
{

	GameInToSeasonLayer();

// 	mScene = CCScene::create();
// 
// 	//背景
// 	SeasonSelectorLayerBackgroud* bg = new SeasonSelectorLayerBackgroud();
// 	mScene->addChild(bg,-1);bg->release();
// 	
// 
// 
// 	//标题
// 	SeasonSelectorLayerTitle* title = new SeasonSelectorLayerTitle();
// 	//title->setVisible(false);
// 	mScene->addChild(title,1);title->release();
// 
// 
// 
// 	//按钮
// 	SeasonSelectorLayerMenu* menu = new SeasonSelectorLayerMenu();
// 	mScene->addChild(menu,128);menu->release();
// 	//menu->setVisible(false);
// 
// 	//未知
// 	SeasonSelectorLayerGameGoods* gamegoods = SeasonSelectorLayerGameGoods::create(title,menu);
// 	mScene->addChild(gamegoods,129, gamegoods_tag);
// 	//gamegoods->setVisible(false);
// 
// 	//分数
// 	SeasonSelectorLayerTotalScore* scoreLayer = new SeasonSelectorLayerTotalScore();
// 	//scoreLayer->setVisible(false);
// 	mScene->addChild(scoreLayer);scoreLayer->release();
// 
// 
// 	SeasonSelectorLayer* layer = SeasonSelectorLayer::createSeasonSelectorLayer(SeasonSelectorLayer::lastCreatedId());
// 	mScene->addChild(layer);
// 	layer->setCombinedLayers(bg,menu,title,gamegoods);
// 	menu->setSeasonlayer(layer);
// 	
// 
// 
// 	if (fromSection)
// 	{
// 		layer->zoomIn(false,false);
// 		layer->zoomOut(true);
// 		CCDirector::sharedDirector()->replaceScene(mScene);
// 	}else
// 	{
// 		CCTransitionFade* tran = CCTransitionFade::create(0.5,mScene);
// 		CCDirector::sharedDirector()->replaceScene(tran);
// 	}
// 
// 	//needUnlockSeasonId = 2;
// 	// 需要解锁
// 	if (needUnlockSeasonId != -1)
// 	{
// 		layer->setUnlockSeasonId(needUnlockSeasonId);
// 		needUnlockSeasonId = -1;
// 	}
// 
// 	if (needScrollToSeasonId != -1)
// 	{
// 		layer->scrollToSeason(needScrollToSeasonId,1.5f);
// 		//layer->scrollToPageSimple(needScrollToSeasonId-1,1.5f);
// 		needScrollToSeasonId = -1;
// 	}
}

void LevelManager::GameInToSectionLayer()
{
	setNext(false);
	ResManager::getManager()->removeAllArmatureInfo();
	CCScene* pScene = Starseneui::scene();
	Starseneui* pLayer = (Starseneui*)pScene->getChildByTag(Starseneui_tag);
	pLayer->Goxiaogk(Smalllayer_tag);

	CCTransitionFade* pTran = CCTransitionFade::create(0.5f, pScene, ccWHITE);
	CCDirector::sharedDirector()->replaceScene(pTran);
}

void LevelManager::GameInToSeasonLayer()
{
	setNext(false);
	ResManager::getManager()->removeAllArmatureInfo();

	CCScene* pScene = Starseneui::scene();
	Starseneui* pLayer = (Starseneui*)pScene->getChildByTag(Starseneui_tag);
	pLayer->GoBiggk(Bigcheckpoint_tag);

	CCTransitionFade* pTran = CCTransitionFade::create(0.5f, pScene);
	CCDirector::sharedDirector()->replaceScene(pTran);
}

void LevelManager::goToSectionUI(int seasonId,bool needTrans/* = true*/)				// 到section选择界面
{
	GameInToSectionLayer();

	return;
	ResManager::getManager()->removeAllArmatureInfo();

	CCScene* scene = CCScene::create();

	int sectionNum = GameLayerFactory::getInstance()->getAllSectionNumBySeasonId(seasonId);
	int pageNum = sectionNum / (Section_Item_Num_In_Row*Section_Item_Num_In_Col);
	if (sectionNum % (Section_Item_Num_In_Row*Section_Item_Num_In_Col) != 0)
		pageNum++;
	//SeasonSelectorLayerGameGoods* gamegoods = SeasonSelectorLayerGameGoods::create(title,menu);
	//scene->addChild(gamegoods,129, gamegoods_tag);
	
	CCLayer* mask = MaskLayer::createLayer(seasonId,pageNum);
	scene->addChild(createSectionLayer(seasonId));
	scene->addChild(mask,10);
	
	//section选择的背景
	{
#if USE_DOOR_REPLACE_CURTAIN == 1
		CCSprite* bg = CCSprite::create(ResManager::getManager()->getSharedFilePath(g_mainlayerPath+"main_background.png").c_str());
		scene->addChild(bg, -1);
		bg->setPosition(ccp(CCDirector::sharedDirector()->getWinSize().width/2, CCDirector::sharedDirector()->getWinSize().height/2));
		bg->setScale(1382.f/1024.f);
#else
		CCLayer* bg_layer = CurtainLayerClosedFrame::create();
		scene->addChild(bg_layer,-1);
#endif

#if AUTO_ADJUST_SCREEN == 1 && USE_DOOR_REPLACE_CURTAIN != 1
		CCLayer* curtain_side = new CurtainSideLayer();
		scene->addChild(curtain_side);
		curtain_side->release();
#endif
	}

	if (needTrans)
	{
		CCTransitionFade* tran = CCTransitionFade::create(0.3f,scene);
		CCDirector::sharedDirector()->replaceScene(tran);
	}else
	{
		CCDirector::sharedDirector()->replaceScene(scene);
	}

}
void LevelManager::goToSectionUI(bool needTrans/* = true*/)
{
	goToSectionUI(m_iLastSeasonId,needTrans);
}
void LevelManager::retry(bool needCurtain)
{
	if (!needCurtain)
	{
		LevelManager::getManager()->setNext(needCurtain);
	}

	setShowLoadingAnimation(needCurtain);
	int sectionIndex = GameLayerFactory::getInstance()->getSectionIndex(m_iLastSeasonId,m_iLastSectionId);
    CCScene* scene = GameScenseDefine::SharedGameScenseDefine()->createScenseByIndex(sectionIndex,needCurtain);
	CCTransitionFade* tran = NULL;
	if (needCurtain)
	{
 		tran = CCTransitionFade::create(0.6f,scene);
 	}else
 	{
		tran = CCTransitionFade::create(0.2f, scene);
	}
	CCDirector::sharedDirector()->replaceScene(tran);
}
void LevelManager::goTo(int seasonId,int sectionId)
{
	m_iLastSeasonId = seasonId;
	m_iLastSectionId = sectionId;
	int sectionIndex = GameLayerFactory::getInstance()->getSectionIndex(m_iLastSeasonId,m_iLastSectionId);
	CCScene* scene = GameScenseDefine::SharedGameScenseDefine()->createScenseByIndex(sectionIndex);
    
    CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.2f, scene));
}
void LevelManager::nextSection()
{
	setNext(false);
	GameLayerFactory* layerFactory = GameLayerFactory::getInstance();
	int nextSeasonId = layerFactory->getNextSeasonId(m_iLastSeasonId,m_iLastSectionId);
	int nextSectionId = layerFactory->getNextSectionId(m_iLastSeasonId,m_iLastSectionId);

	if (nextSeasonId >= GameLayerFactory::getInstance()->getConfigSeasonNum())
	{
		GameInToSectionLayer();
		return;
	}
	
	setLastSectionId(nextSectionId);
	goTo(nextSeasonId, nextSectionId);
}
void LevelManager::preSection()
{
	setNext(false);
	int preSeasonId = m_iLastSeasonId;
	int preSectionId = m_iLastSectionId -1;

	if (preSectionId <= 0)
	{
		preSeasonId --;

		if (preSeasonId <= 0)
		{
			CCLog("LevelManager::preSection : current level dos not have pre level ....");
			GameInToSectionLayer();
			return;
		}

		preSectionId = GameLayerFactory::getInstance()->getAllSectionNumBySeasonId(preSeasonId);
	}
	goTo(preSeasonId,preSectionId);
}

void LevelManager::reset()
{
    delete manager;
    manager = NULL;
    LevelManager::getManager();
	//reset后这个参数需要重置为false,
	//因为调用Reset肯定是运行了的，所以这里需要手动改为false
	manager->m_bStartApp = false;
}

void LevelManager::setNeedUnLockSeason(int seasonId)
{
	needUnlockSeasonId = seasonId;
}
void LevelManager::setNeedScrollToSeason(int seasonId)
{
	needScrollToSeasonId = seasonId;
}

//==============回调接口==================//

//season被选中之后调用
void LevelManager::seasonSelected(int seasonId,int lastSeasonId,int lastSectionId,CCPoint seasonPos)
{
	
}
//section被选中之后
void LevelManager::sectionSelected(int sectionId,CCPoint sectionPos)
{
	m_iLastSectionId = sectionId;

	CCUserDefault::sharedUserDefault()->setIntegerForKey(CCString::createWithFormat("Last_season%d_seation", m_iLastSeasonId)->getCString(), m_iLastSectionId);
	CCUserDefault::sharedUserDefault()->flush();

	//// 隐藏
	//SectionSelectorLayer::getCurrentInstance()->setVisible(false);
	//MaskLayer::getCurrentInsstance()->setVisible(false);

//	SectionSelectorLayer::getCurrentInstance()->removeFromParentAndCleanup(true);
//	MaskLayer::getCurrentInsstance()->removeFromParentAndCleanup(true);
    
    CCDirector::sharedDirector()->getRunningScene()->removeAllChildren();

	LoadingLayer* layer = new LoadingLayer(Intent_Section2GameIn,m_iLastSeasonId);
	CCScene* pScene = CCDirector::sharedDirector()->getRunningScene();
	pScene->addChild(layer);
	layer->release();
}

//section页面放弃选择，也就是返回了
void LevelManager::sectionBackToSeason(int fromSeasonId)
{
	
}

void LevelManager::setLastSeasonId(int iLastSeasonId, bool bSetClickSeasonId/* = true*/)
{
	m_iLastSeasonId = iLastSeasonId;
}