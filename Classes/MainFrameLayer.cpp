#include "MainFrameLayer.h"
#include "ResManager.h"
#include <string>
#include "stdio.h"
#include "stdlib.h"
#include "LFShareKit.h"
#include "LevelManager.h"
#include "GameInforEditer.h"
#include <cmath>
//#include "PhysicsSimulationLayer.h"
#include "SeasonSelectorLayer.h"
#include "InitGuideLayer2.h"
#include "Music.h"
#include "LFStrings.h"
#include "VersionControl.h"
#include "collectLayer.h"
#include "rankingLayer.h"
#include "IOSStoreLayer.h"
#include "Debug.h"
#include "VersionControl.h"
#include "font.h"
#include "LFStrings.h"


#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
//#include "protocol.h"
extern unsigned char g_AppStoreUrlType;
extern std::string g_AppStoreUpdateCommentMsg;
extern std::string g_AppStoreUpdateCommentUrl;


#endif

using namespace std;

#define AUTO_MOVE_DISTANCE (768.0f/4.0f)	// 当滑动超过AUTO_MOVE_DISTANCE，就会自动滑动

#define Group_Item_Distance 100.0f
#define Offset_Y -100.0f
#define Offset_X -100.0f

#define UFO_ShowDelayTime 7
#define StarEffect_Time  3

static bool isReinputedTheNumber = false;


#define UI_mainaramite_name "STAR_FALL_UI_zhuye.ExportJson"   
#define UI_animation_data  "STAR_FALL_UI_zhuye"


enum Maintag{

	armrate_chuxiantag=1000,
	back_tag,

};

MainFrameLayer::MainFrameLayer()
	:m_nDmjEyeCount(0),
	m_nQsyEyeCount(0),
	m_nDmjArmCount(0),
	m_nDmjStarCount(0),
	m_nStarRotate(0)
{

	
	
	init();
	//标记
	mark=1;

	// 物理效果图层
	{
		//PhysicsSimulationLayer* layer = new PhysicsSimulationLayer();
		//addChild(layer,1);
		//layer->release();
		//hideAll(false);
	}
	hideAll(false);
	settingGroup.clear();
	shareGroup.clear();

	if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	{
		setKeypadEnabled(true);
	}
	
	ResManager* resManager = ResManager::getManager();
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	width=winSize.width;
	hight=winSize.height;
	CCSprite* bg = CCSprite::create(resManager->getSharedFilePath(g_mainlayerPath+"main_background.png").c_str());
	bg->setTag(back_tag);
	bg->setScale(1382.f/1024.f);
	addChild(bg,-2);
	bg->setPosition(ccp(winSize.width/2,winSize.height/2));
	mBG = bg;
	 
	mStartItem = CCMenuItemImage::create(
		resManager->getSharedFilePath(g_buttonPath+"Play_buttom2.png").c_str(),
		resManager->getSharedFilePath(g_buttonPath+"Play_buttom2.png").c_str(),
		this,SEL_MenuHandler(&MainFrameLayer::startItemClicked));
	mStartItem->setScale(2.0f);
	mStartItem->setPosition(ccp(winSize.width/2,hight*0.45));
	mStartItem->setOpacity(0);

	CCMenu* starMenu = CCMenu::create(mStartItem,NULL);
	starMenu->setPosition(CCPointZero);
	addChild(starMenu,129);
	//facebook
	//Buttonfadein();
	
	//动画播放
	CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfoAsync(ResManager::getManager()->getFilePath(UI_mainaramite_name).c_str(),this, schedule_selector(MainFrameLayer::dataLoaded));
	
	/*

	{

	mTilte = MainThingEffect::create();
	mTilte->initWithTexture(CCSprite::create(resManager->getFilePathByLanguageType(g_mainlayerPath+"main_title.png").c_str())->getTexture());

	
	mTilte->setTag(1);
	addChild(mTilte,1);
	m_sMainThingList.push_back(mTilte);
	mTilte->setParent(this);
	mTilte->setTag(6);

	mTilte->setPosition(ccp(winSize.width/2,winSize.height/2 + 60));
	{// person
		CCSize titleSize = mTilte->getTextureRect().size;
		CCSprite *s1 = CCSprite::create((resManager->getSharedFilePath()+g_mainlayerPath+"main_renwu_qsy.png").c_str());
		s1->setPosition(ccp(winSize.width/2 + 82 - titleSize.width / 2,winSize.height/2 + 60 + 396-262  - titleSize.height / 2));

		{
			for (int i = 1; i < 6; i ++)
			{
				CCString *str = CCString::createWithFormat((resManager->getSharedFilePath()+g_mainlayerPath+"main_xiaoguo_yanjing_shanguang_%d.png").c_str(),i);
				CCSprite *spr = CCSprite::create(str->getCString());
				spr->setPosition(ccp(105,241));
				s1->addChild(spr,2);
				spr->setVisible(false);
				m_sQsyEyeList.insert(map<int,CCSprite*>::value_type(i,spr));
			}
			{
				map<int,CCSprite *>::iterator iter = m_sQsyEyeList.find(0);
				if (iter != m_sQsyEyeList.end())
				{
					iter->second->setVisible(true);
				}
			}
		}
		addChild(s1,1);
		CCSprite *s2 = CCSprite::create((resManager->getSharedFilePath()+g_mainlayerPath+"main_renwu_dmj.png").c_str());
		addChild(s2,1);
		s2->setPosition(ccp(winSize.width/2 + 748 - titleSize.width / 2,winSize.height/2 + 60 + 396-264  - titleSize.height / 2));

		{
			for (int i = 1; i < 5; i ++)
			{
				CCString *str = CCString::createWithFormat((resManager->getSharedFilePath()+g_mainlayerPath+"main_xiaoguo_zhayan_%d.png").c_str(),i);
				CCSprite *spr = CCSprite::create(str->getCString());
				spr->setPosition(ccp(104,264));
				s2->addChild(spr,2);
				spr->setVisible(false);
				m_sDmjEyeList.insert(map<int,CCSprite*>::value_type(i,spr));
			}
			{
				map<int,CCSprite *>::iterator iter = m_sDmjEyeList.find(4);
				if (iter != m_sDmjEyeList.end())
				{
					iter->second->setVisible(true);
				}
			}

			{
				for (int i = 1; i < 4; i ++)
				{
					CCString *str = CCString::createWithFormat((resManager->getSharedFilePath()+g_mainlayerPath+"main_xiaoguo_taishou_%d.png").c_str(),i);
					CCSprite *spr = CCSprite::create(str->getCString());
					spr->setPosition(ccp(145,157));
					s2->addChild(spr,2);
					spr->setVisible(false);
					m_sDmjArmList.insert(map<int,CCSprite*>::value_type(i,spr));
				}
				for (int i = 1; i < 4; i ++)
				{
					CCString *str = CCString::createWithFormat((resManager->getSharedFilePath()+g_mainlayerPath+"main_xiaoguo_taishou_xing_%d.png").c_str(),i);
					CCSprite *spr = CCSprite::create(str->getCString());
					spr->setAnchorPoint(CCPointZero);
					spr->setPosition(ccp(160,197));
					s2->addChild(spr);
					spr->setVisible(false);
					m_sDmjStarList.insert(map<int,CCSprite*>::value_type(i,spr));
				}

			}
		}
		mStar = MainThingEffect::create();
		mStar->initWithTexture(CCSprite::create((resManager->getSharedFilePath()+g_mainlayerPath+"main_title_xingxing.png").c_str())->getTexture());
		addChild(mStar,1);
		mStar->setPosition(ccp(winSize.width/2 + 400 - titleSize.width / 2,winSize.height/2 + 60 + 396-143  - titleSize.height / 2));
		m_sMainThingList.push_back(mStar);
		mStar->setTag(6);
		mStar->setParent(this);
		m_nStarRotate = mStar->getRotation();
	}


	setItem->setVisible(true);
	setItem->setEnabled(true);
	setItem->setPosition(ccp(58,58));

	setPic = CCSprite::create(resManager->getSharedFilePath(g_buttonPath+"main_set.png").c_str());
	setItem->addChild(setPic);
	setPic->setPosition(ccp(setItem->getContentSize().width/2,setItem->getContentSize().height/2));

	


	
	{
		shareItem = CCMenuItemImage::create(
			resManager->getSharedFilePath(g_buttonPath+"main_button.png").c_str(),
			resManager->getSharedFilePath(g_buttonPath+"main_button_select.png").c_str(),
			this,SEL_MenuHandler(&MainFrameLayer::shareClicked));
		//shareItem->setPosition(ccp(winSize.width - 58,58));
		shareItem->setPosition(setItem->getPosition());
		sharePic = CCSprite::create(resManager->getSharedFilePath(g_buttonPath+"main_share.png").c_str());
		shareItem->addChild(sharePic);
		sharePic->setPosition(ccp(shareItem->getContentSize().width/2,shareItem->getContentSize().height/2));
		shareItem->setVisible(false);
	}

	CCMenuItemImage* restar_test = CCMenuItemImage::create(
		resManager->getSharedFilePath(g_buttonPath+"restart.png").c_str(),
		resManager->getSharedFilePath(g_buttonPath+"restart_select.png").c_str(),
		this,SEL_MenuHandler(&MainFrameLayer::restarClicked));

	CCMenuItemImage* infoItem = CCMenuItemImage::create(
		resManager->getSharedFilePath(g_buttonPath+"set_info.png").c_str(),
		resManager->getSharedFilePath(g_buttonPath+"set_info_select.png").c_str(),
		this,SEL_MenuHandler(&MainFrameLayer::mingxieItemClicked));

	CCMenuItemImage* moreGameItem = CCMenuItemImage::create(
		resManager->getSharedFilePath(g_buttonPath+"set_more.png").c_str(),
		resManager->getSharedFilePath(g_buttonPath+"set_more_select.png").c_str(),
		this,SEL_MenuHandler(&MainFrameLayer::moreGameItemClicked));

	CCMenuItemImage* helpItem = CCMenuItemImage::create(
		resManager->getSharedFilePath(g_buttonPath+"set_help.png").c_str(),
		resManager->getSharedFilePath(g_buttonPath+"set_help_select.png").c_str(),
		this,SEL_MenuHandler(&MainFrameLayer::helpItemClicked));
	
	//current_language_**.png
	languageItem = CCMenuItemImage::create(
		resManager->getSharedFilePath(g_buttonPath+"current_language.png").c_str(),
		resManager->getSharedFilePath(g_buttonPath+"current_language_select.png").c_str(),
		this,SEL_MenuHandler(&MainFrameLayer::languageItemClicked));

	CCMenuItemImage* gameCenter_achieve = NULL;
	if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS && LFShareKit::getKit()->isGameCenterEnabled())
	{
		gameCenter_achieve = CCMenuItemImage::create(
			resManager->getSharedFilePath(g_buttonPath+"gamecenter.png").c_str(),
			resManager->getSharedFilePath(g_buttonPath+"gamecenter_select.png").c_str(),
			this,SEL_MenuHandler(&MainFrameLayer::achieveClicked));
		gameCenter_achieve->setPosition(ccp(500,50));
		gameCenter_achieve->setVisible(false);
		gameCenter_achieve->setEnabled(false);
	}

	//排行榜   商店  收藏
	{
// 		storeItem = CCMenuItemImage::create(
// 			getString("main_button.png").c_str(),
// 			getString("main_button_select.png").c_str(),
// 			this,SEL_MenuHandler(&MainFrameLayer::storeClicked));
// 		storeItem->setPosition(ccp(winSize.width - 58,58));
// 		CCSprite *storePic = CCSprite::create("main_anniu_shangdian.png");
// 		storeItem->addChild(storePic);
// 		storePic->setPosition(ccp(storeItem->getContentSize().width/2,storeItem->getContentSize().height/2));

		rankingItem = CCMenuItemImage::create(
			resManager->getSharedFilePath(g_buttonPath+"main_button.png").c_str(),
			resManager->getSharedFilePath(g_buttonPath+"main_button_select.png").c_str(),
			this,SEL_MenuHandler(&MainFrameLayer::rankingClicked));
		rankingItem->setPosition(ccp(winSize.width - 58,58));
		CCSprite *rankingPic = CCSprite::create(resManager->getSharedFilePath(g_buttonPath+"main_anniu_paihangbang.png").c_str());
		rankingItem->addChild(rankingPic);
		rankingPic->setPosition(ccp(rankingItem->getContentSize().width/2,rankingItem->getContentSize().height/2));

		mCollectItem = CCMenuItemImage::create(
			resManager->getSharedFilePath(g_buttonPath+"main_button.png").c_str(),
			resManager->getSharedFilePath(g_buttonPath+"main_button_select.png").c_str(),
			this,SEL_MenuHandler(&MainFrameLayer::collectClicked));
		mCollectItem->setPosition(ccp(winSize.width - 58, 58 + 120));
		CCSprite *collectPic = CCSprite::create(resManager->getSharedFilePath(g_buttonPath+"main_anniu_daojushouji.png").c_str());
		mCollectItem->addChild(collectPic);
		collectPic->setPosition(ccp(mCollectItem->getContentSize().width/2,mCollectItem->getContentSize().height/2));
	}

	CCMenu* starMenu = CCMenu::create(mStartItem,shareItem,rankingItem,mCollectItem,NULL);
	starMenu->setPosition(CCPointZero);
	addChild(starMenu,129);


	{
		// 存放到CCMenu里面，就把不会release了
		CCArray* menuItemArray = CCArray::create();
		menuItemArray->addObject(restar_test);
		menuItemArray->addObject(languageItem);
		menuItemArray->addObject(moreGameItem);
		menuItemArray->addObject(helpItem);
		menuItemArray->addObject(infoItem);

		if(NeedMoreGameButton == 0)
		{
			moreGameItem->setVisible(false);
		}
		else
		{
			shareGroup.push_back(moreGameItem);
		}

		if(NeedHelpButton == 0)
		{
			helpItem->setVisible(false);
		}
		else
		{
			shareGroup.push_back(helpItem);
		}

// 		if(NeedExitButton != 1)
// 		{
// 			infoItem->setVisible(false);
// 		}
// 		else
// 		{
			shareGroup.push_back(infoItem);
//		}

		if (gameCenter_achieve)
			menuItemArray->addObject(gameCenter_achieve);

		CCMenu* itemMenu = CCMenu::createWithArray(menuItemArray);
		addChild(itemMenu,12);
		itemMenu->setPosition(CCPointZero);
	}
	
	// 不要了
// 	facebook->setVisible(false);
// 	twitter->setVisible(false);
// 	itemSina->setVisible(false);
	//shareGroup.insert(shareGroup.end(),facebook);
	//shareGroup.insert(shareGroup.end(),twitter);
	//shareGroup.insert(shareGroup.end(),itemSina);

	if (gameCenter_achieve)
		shareGroup.insert(shareGroup.end(),gameCenter_achieve);

	settingGroup.insert(settingGroup.end(),itemEffctiveSound);
	settingGroup.insert(settingGroup.end(),itemBackgroundSound);

    if (userInfoBtn != NULL)
    {
        settingGroup.insert(settingGroup.end(), userInfoBtn);
    }
    if (itemExit != NULL)
	{
		settingGroup.insert(settingGroup.end(),itemExit);
	}


	//settingGroup.insert(settingGroup.end(),languageItem); // 暂时不要语言切换
	//settingGroup.insert(settingGroup.end(),restar_test);
	languageItem->setVisible(false);
	restar_test->setVisible(false);



	// 初始排序，位置
	{
		if (shareGroup.empty())
			shareItem->setVisible(false);
		if (settingGroup.empty())
			setItem->setVisible(false);

		// 第一个放在最右面
		float offset_x = Offset_X;
		for(int i=0;i<shareGroup.size();i++)
		{
			shareGroup[i]->setPosition(ccp(shareItem->getPositionX() + offset_x - (shareGroup.size()-i -1)*Group_Item_Distance, shareItem->getPositionY()));
		}
		// 第一个放在最上面
		float offset_y = Offset_Y;
		for (int i=0;i<settingGroup.size();i++)
		{
			settingGroup[i]->setPosition(ccp(setItem->getPositionX() ,setItem->getPositionY() + offset_y - (settingGroup.size()-i -1)*Group_Item_Distance));
		}

		schedule(schedule_selector(MainFrameLayer::tick));
	}
	this->schedule(SEL_SCHEDULE(&MainFrameLayer::dmjEye),KDMJEyeSideTime);
	this->schedule(SEL_SCHEDULE(&MainFrameLayer::qsyEye),KDMJEyeSideTime);
	//this->schedule(SEL_SCHEDULE(&MainFrameLayer::dmjPushUpArm),KDMJPushUpSideTime);
	//schedule(schedule_selector(MainFrameLayer::toShowInputPhoneNumerIfNeed),0.6f);
	scheduleOnce(schedule_selector(MainFrameLayer::toShowUpdateAlertIfNeed),0.5f);
	schedule(schedule_selector(MainFrameLayer::toShowUFO),UFO_ShowDelayTime,-1,3);
	schedule(schedule_selector(MainFrameLayer::toShowStarEffect),StarEffect_Time,-1,1);
//	schedule(schedule_selector(MainFrameLayer::toShowStartMenuEffect),2);
	
	{
		Music::sharedMusicPlayer()->playMainMusic();
	}

	AdManager::getManager()->adHideBannerAd();

	// 尝试初始化
	if (!OrderList::isPayChannelInfoInited())
	{
		PlantformProtocal pp;
		CmdHelper::getHelper()->cmdQueryPayChannelInfo(pp.getPLMN());
	}
	// 尝试获取号码
	string phoneNum = GameInforEditer::getGameInfor()->getPhoneNumber();
	CmdHelper::getHelper()->cmdQueryUnlockInfo(GameInforEditer::getGameInfor()->getClientId(),phoneNum);

	//int pos[8][2] = {{140,450},{207,118},{480,200},{1180,220},{537,34},{816,280},{1029,73},{1220,480}};
	int pos[8][2] = {{160,460},{222,218},{1207,313},{458,188},{680,48},{815,265},{998,172},{1241,500}};
	for (int i = 1; i < 9; i ++)
	{
		CCString *str = CCString::createWithFormat((resManager->getSharedFilePath()+g_mainlayerPath+"main_xiaoguo_piaofu_%d.png").c_str(),i);
		CCSprite *oneSpr = CCSprite::create(str->getCString());
		MainThingEffect *one = MainThingEffect::create();
		one->initWithTexture(oneSpr->getTexture());
		//one->setAnchorPoint(CCPointZero);
		one->setPosition(ccp(pos[i - 1][0],pos[i - 1][1]));
		addChild(one,-1);
		one->setTag(i);
		one->setParent(this);
		m_sMainThingList.push_back(one);
	}
	moveThing();
}
	*/
}




CCScene* MainFrameLayer::createScene()
{
	CCScene* scene = CCScene::create();

	MainFrameLayer* layer = createLayer();
	scene->addChild(layer);

	//20150122  scf
// 	// 是yeahmobi的短信购买，那么就先判断是否买过，但没有，那么就需要提供手机号码输入的机会
// 	if (PaymentFactory::getFactory()->getCurrentPaymentManager(false)!=NULL &&
// 		PaymentFactory::getFactory()->getCurrentPaymentManager(false)->getPaymentTag() == PT_YEAH_MOBI_SMS)
// 	{
// 		PlantformProtocal pp;
// 		string phoneNumberFromSDK = pp.getPhoneNumber();
// 		string phoneNumberFromLocal = GameInforEditer::getGameInfor()->getPhoneNumber();
// 
// 		if ( (phoneNumberFromSDK != "") && phoneNumberFromLocal != phoneNumberFromSDK)
// 			GameInforEditer::getGameInfor()->setUserInputPhoneNumber(phoneNumberFromSDK);
// 
// 		// 购买过，但是没有解锁成功，那么就要求重新输入
// 		if (GameInforEditer::getGameInfor()->checkPayState() == PayState_PAY_NOT_UNLOCK && !isReinputedTheNumber && phoneNumberFromSDK == "")
// 		{
// 			isReinputedTheNumber = true;
// 			CCNotificationCenter::sharedNotificationCenter()->addObserver(layer,SEL_CallFuncO(&MainFrameLayer::phoneNumberInputed),MSG_PHONENUMBER_INPUT_OK,NULL);
// 			scene->addChild(PhoneNuberGetterFrame::createFrame(LFStrings::getValue("please_input_phone_again")),1024);
// 		}
// 	}
	return scene;
}
MainFrameLayer* MainFrameLayer::createLayer()
{
	MainFrameLayer* layer = new MainFrameLayer();
	layer->autorelease();
	return layer;
}

void MainFrameLayer::keyBackClicked()
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	if (lfAlert==NULL)
	{
		CCDirector::sharedDirector()->end();
	}
#endif
}
void MainFrameLayer::keyMenuClicked()
{
	
}

//void MainFrameLayer::toShowInputPhoneNumerIfNeed(float t)
//{
//	unschedule(schedule_selector(MainFrameLayer::toShowInputPhoneNumerIfNeed));
//}
void MainFrameLayer::phoneNumberInputed(CCObject* null_obj)
{
//	CCNotificationCenter::sharedNotificationCenter()->removeObserver(this,MSG_PHONENUMBER_INPUT_OK);
	
// 	// 输入了号码之后，发现已经取到数据了，那么就不用再查询了
// 	if (GameInforEditer::getGameInfor()->checkPayState() == PayState_PAY_NOT_UNLOCK)
// 	{
// 		// 尝试获取号码
// 		string phoneNum = GameInforEditer::getGameInfor()->getPhoneNumber();
// 		// 查询3次
// 		CmdHelper::getHelper()->cmdQueryUnlockInfo(GameInforEditer::getGameInfor()->getClientId(),phoneNum);
// 		CmdHelper::getHelper()->cmdQueryUnlockInfo(GameInforEditer::getGameInfor()->getClientId(),phoneNum);
// 		CmdHelper::getHelper()->cmdQueryUnlockInfo(GameInforEditer::getGameInfor()->getClientId(),phoneNum);
// 	}
}

void MainFrameLayer::toShowUpdateAlertIfNeed(float t)
{
	//未使用
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	return;
// 	ResManager* resMgr = ResManager::getManager();
// 	CCSprite* base = CCSprite::create(resMgr->getFilePathByLanguageType("upgrade_board.png").c_str());
// 
// 	CCMenuItemImage* okItem = CCMenuItemImage::create(
// 		resMgr->getFilePathByLanguageType("upgrade_word_yes.png").c_str(),
// 		resMgr->getFilePathByLanguageType("upgrade_word_yes_select.png").c_str(),
// 		this,SEL_MenuHandler(&MainFrameLayer::commentOkClicked)
// 		);
// 	CCMenuItemImage* latterItem = CCMenuItemImage::create(
// 		resMgr->getFilePathByLanguageType("upgrade_word_later.png").c_str(),
// 		resMgr->getFilePathByLanguageType("upgrade_word_later_select.png").c_str(),
// 		this,SEL_MenuHandler(&MainFrameLayer::commentLatterClicked)
// 		);
// 	CCMenuItemImage* neverItem = CCMenuItemImage::create(
// 		resMgr->getFilePathByLanguageType("upgrade_word_never.png").c_str(),
// 		resMgr->getFilePathByLanguageType("upgrade_word_never_select.png").c_str(),
// 		this,SEL_MenuHandler(&MainFrameLayer::commentNeverClicked)
// 		);
// 
// 	CCMenu* menu = CCMenu::create(okItem,latterItem,neverItem,NULL);
// 
// 	okItem->setPosition(ccp(223,233));
// 	latterItem->setPosition(ccp(223,161));
// 	neverItem->setPosition(ccp(223,90));
// 
// 	lfAlert = new LFAlert();
// 	lfAlert->setBaseContainer(base,menu);
// 	addChild(lfAlert,512);
// 	lfAlert->release();
#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    
//     if (g_AppStoreUrlType != appStoreUrlType_UpdateUrl) {
//         return;
//     }
// 	ResManager* resMgr = ResManager::getManager();
// 	CCSprite* base = CCSprite::create(resMgr->getFilePathByLanguageType("upgrade_board.png").c_str());
// 
// 	CCMenuItemImage* okItem = CCMenuItemImage::create(
// 		resMgr->getFilePathByLanguageType("upgrade_word_yes.png").c_str(),
// 		resMgr->getFilePathByLanguageType("upgrade_word_yes_select.png").c_str(),
// 		this,SEL_MenuHandler(&MainFrameLayer::commentOkClicked)
// 		);
// 	CCMenuItemImage* latterItem = CCMenuItemImage::create(
// 		resMgr->getFilePathByLanguageType("upgrade_word_later.png").c_str(),
// 		resMgr->getFilePathByLanguageType("upgrade_word_later_select.png").c_str(),
// 		this,SEL_MenuHandler(&MainFrameLayer::commentLatterClicked)
// 		);
// 	CCMenuItemImage* neverItem = CCMenuItemImage::create(
// 		resMgr->getFilePathByLanguageType("upgrade_word_never.png").c_str(),
// 		resMgr->getFilePathByLanguageType("upgrade_word_never_select.png").c_str(),
// 		this,SEL_MenuHandler(&MainFrameLayer::commentNeverClicked)
// 		);
// 
// 	CCMenu* menu = CCMenu::create(okItem,latterItem,neverItem,NULL);
// 
// 	okItem->setPosition(ccp(223,233));
// 	latterItem->setPosition(ccp(223,161));
// 	neverItem->setPosition(ccp(223,90));
// 
// 	lfAlert = new LFAlert();
// 	lfAlert->setBaseContainer(base,menu);
// 	addChild(lfAlert,512);
// 	lfAlert->release();
    
#endif
}
void MainFrameLayer::commentOkClicked(CCObject* obj)
{
	if (lfAlert)
	{
		lfAlert->removeFromParentAndCleanup(true);
		lfAlert = NULL;
	}
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	if (lfAlert)
	{
		lfAlert->removeFromParentAndCleanup(true);
		lfAlert = NULL;
	}
    
    LFShareKit::getKit()->openURl(g_AppStoreUpdateCommentUrl);
    
    // 这次游戏就不再弹出了
//    g_AppStoreUrlType = appStoreUrlType_Empty;
    
#endif
}
void MainFrameLayer::commentLatterClicked(CCObject* obj)
{
	if (lfAlert)
	{
		lfAlert->removeFromParentAndCleanup(true);
		lfAlert = NULL;
	}
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	if (lfAlert)
	{
		lfAlert->removeFromParentAndCleanup(true);
		lfAlert = NULL;
	}
    
    // 这次游戏就不再弹出了
//    g_AppStoreUrlType = appStoreUrlType_Empty;
    
#endif
}
void MainFrameLayer::commentNeverClicked(CCObject* obj)
{
	if (lfAlert)
	{
		lfAlert->removeFromParentAndCleanup(true);
		lfAlert = NULL;
	}
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	if (lfAlert)
	{
		lfAlert->removeFromParentAndCleanup(true);
		lfAlert = NULL;
	}
    
    // 这次游戏就不再弹出了
//    g_AppStoreUrlType = appStoreUrlType_Empty;
#endif
}

void MainFrameLayer::startItemClicked(CCObject* obj)
{

// 	//分享test
// 	RedStarSDKManager::getInstance()->showShare();
// 	return;
// 
// 

	Music::sharedMusicPlayer()->buttonClicked();

	//// jni 崩溃测试
	//{
	//	vector<string> test;
	//	test[7] = "";
	//}
	if (GameInforEditer::getGameInfor()->isFirstRunMe())
//	if (true)
	{
		LevelManager::getManager()->goToInitGuideUI();
		GameInforEditer::getGameInfor()->firstRunOverApp();
	}
	else
	{
		LevelManager::getManager()->goToSeasonUI(false);
	}

}
void MainFrameLayer::settingClicked(CCObject* obj)
{
//	Music::sharedMusicPlayer()->buttonClicked();

	shareClicked(NULL);

	float actionTime = 0.5f;

	if (setPic->getRotation() != 0 && setPic->getRotation()!=180)
		return;

	setPic->stopAllActions();

	if (setPic->getRotation() <= 0)
	{
		//打开
		setPic->runAction(CCRotateTo::create(actionTime,180));
		moveOut(settingGroup,setItem);
	}else
	{
		// 关闭
		setPic->runAction(CCRotateTo::create(actionTime,0));
		moveBack(settingGroup,setItem);
	}
}
void MainFrameLayer::moveBack(vector<CCMenuItemImage*> group,CCNode* root, bool up)
{
	if (group.size() <= 0)
		return;

	if (up)
	{
		float distance = -abs(group[group.size() - 1]->getPositionY() - (root->getPositionY() + Offset_Y));
		for (size_t i=0;i<group.size();i++)
		{
			CCMoveBy* move = CCMoveBy::create(0.5f,ccp(0,distance));
			group[i]->runAction(move);
		}
	}
	else
	{
		float distance = -abs(group[group.size() - 1]->getPositionX() - (root->getPositionX() + Offset_X));
		for (size_t i=0;i<group.size();i++)
		{
			CCMoveBy* move = CCMoveBy::create(0.5f,ccp(distance,0));
			group[i]->runAction(move);
		}
	}
}
void MainFrameLayer::moveOut(vector<CCMenuItemImage*> group,CCNode* root, bool down)
{
	if (group.size() <= 0)
		return;

	if (down)
	{
		float distance = -abs(group[0]->getPositionY() - (root->getPositionY()-Offset_Y) );
		for (size_t i=0;i<group.size();i++)
		{
			CCMoveBy* move = CCMoveBy::create(0.5f,ccp(0,-distance));
			group[i]->runAction(move);
		}
	}
	else
	{
		float distance = -abs(group[0]->getPositionX() - (root->getPositionX()-Offset_X) );
		for (size_t i=0;i<group.size();i++)
		{
			CCMoveBy* move = CCMoveBy::create(0.5f,ccp(-distance,0));
			group[i]->runAction(move);
		}
	}
}
void MainFrameLayer::shareClicked(CCObject* obj)
{
	Music::sharedMusicPlayer()->buttonClicked();

	if (sharePic->getRotation() != 0 && sharePic->getRotation()!=180)
		return;
 
	sharePic->stopAllActions();

	if (sharePic->getRotation() <= 0)
	{
		sharePic->runAction(CCRotateTo::create(0.5f,180));
		moveOut(shareGroup,shareItem,false);
	}else
	{
		sharePic->runAction(CCRotateTo::create(0.5f,0));
		moveBack(shareGroup,shareItem,false);
	}
}

void MainFrameLayer::exitItemClicked(CCObject* obj)
{
	CCDirector::sharedDirector()->end();
}

void MainFrameLayer::helpItemClicked(CCObject* obj)
{
	CCSprite* base = CCSprite::create(ResManager::getManager()->getFilePathByLanguageType(g_mainlayerPath+"help_board.png").c_str());
	CCMenu* menu = CCMenu::create();
	LFAlert* alert = new LFAlert();
	alert->setBaseContainer(base,menu);
	alert->setAutoDismiss("______",true);
	alert->show();
	alert->release();
}
void MainFrameLayer::moreGameItemClicked(CCObject* obj)
{

}
void MainFrameLayer::tick(float t)
{
	for (size_t i=0;i<settingGroup.size();i++)
	{
		setMenuItemVisible(settingGroup[i],setItem);
	}
	for (size_t i=0;i<shareGroup.size();i++)
	{
		setMenuItemVisible(shareGroup[i],shareItem, false);
	}
}
void MainFrameLayer::setMenuItemVisible(CCMenuItemImage* menuItem,CCMenuItemImage* menuItemRoot, bool up)
{
	if (up)
	{
		if (menuItem->getPositionY() - menuItemRoot->getPositionY() < 0)
		{
			if (menuItem->isVisible())
			{
				menuItem->setVisible(false);
				menuItem->setEnabled(false);
			}
		}
		else
		{
			if (!menuItem->isVisible() )
			{
				menuItem->setVisible(true);
				menuItem->setEnabled(true);
			}
		}
	}
	else
	{
		if (menuItem->getPositionX() - menuItemRoot->getPositionX() < 0)
		{
			if (menuItem->isVisible())
			{
				menuItem->setVisible(false);
				menuItem->setEnabled(false);
			}
		}
		else
		{
			if (!menuItem->isVisible() )
			{
				menuItem->setVisible(true);
				menuItem->setEnabled(true);
			}
		}
	}
}

void MainFrameLayer::restarClicked(CCObject* obj)
{
	Music::sharedMusicPlayer()->buttonClicked();
	GameInforEditer::getGameInfor()->makeNew();
	CCDirector::sharedDirector()->replaceScene(MainFrameLayer::createScene());
}

void MainFrameLayer::achieveClicked(CCObject* obj)
{
	Music::sharedMusicPlayer()->buttonClicked();
	LFShareKit::getKit()->showGameCenterAchievement();
}
void MainFrameLayer::languageItemClicked(CCObject*obj)
{
	Music::sharedMusicPlayer()->buttonClicked();

	//测试代码。
	ResManager::getManager()->setLanguageType(ResManager::getManager()->getLanguageType()==kLanguageChinese?kLanguageEnglish:kLanguageChinese);
	LFStrings::init();

	languageItem->initWithNormalImage(
		ResManager::getManager()->getFilePathByLanguageType("current_language.png").c_str(),
		ResManager::getManager()->getFilePathByLanguageType("current_language_select.png").c_str(),
		ResManager::getManager()->getFilePathByLanguageType("current_language.png").c_str(),
		this,SEL_MenuHandler(&MainFrameLayer::languageItemClicked)
		);

	GameInforEditer::getGameInfor()->saveSessonInforList();
}

void MainFrameLayer::storeClicked(CCObject *obj)
{
	Music::sharedMusicPlayer()->buttonClicked();

#if ((CC_TARGET_PLATFORM == CC_PLATFORM_IOS && Version_Platform == 0) || DEBUG_IOSMarket)
	IOSStoreLayer *layer = IOSStoreLayer::create();
	CCDirector::sharedDirector()->getRunningScene()->addChild(layer, 200);
#else
	MarketLayer *layer = new MarketLayer(MS_SuperTools);
	CCDirector::sharedDirector()->getRunningScene()->addChild(layer, 200);
	layer->release();
#endif
}

void MainFrameLayer::rankingClicked(CCObject *obj)
{
 	rankingLayer* pLayer = rankingLayer::create();
	CCDirector::sharedDirector()->getRunningScene()->addChild(pLayer, 200);
}

void MainFrameLayer::collectClicked(CCObject *obj)
{
	collectLayer *layer = collectLayer::create();
	CCDirector::sharedDirector()->getRunningScene()->addChild(layer, 1000);
}

void MainFrameLayer::toShowUFO(float dt)
{
//牛秋晨修改 5月7日

	/**/
    float speed_level = 4.0f; 
	/**/
	CCSize size = mTilte->getContentSize();

	//ufo
	CCSprite *ufo = CCSprite::create((ResManager::getManager()->getSharedFilePath()+g_mainlayerPath+"main_title_xiaoguo_xingqiu.png").c_str());
	mTilte->addChild(ufo, -1);
	ufo->setOpacity(0);
	ufo->setPosition(ccp(size.width/2-324, size.height-170+70));
	ufo->runAction(CCSequence::create(
		CCSpawn::create(
			CCSequence::create(CCMoveTo::create(3.0f / speed_level, ccp(size.width/2 - 177, size.height - 98 + 70)),
							   CCMoveTo::create(3.0/1.3 / speed_level, ccp(size.width/2 - 65, size.height - 66 + 70)),
							   CCMoveTo::create(3.0/2.26 / speed_level, ccp(size.width/2, size.height - 61 + 70)),
							   CCMoveTo::create(3.0/2.26 / speed_level, ccp(size.width/2 + 65, size.height - 66 + 70)),
							   CCMoveTo::create(3.0/1.3 / speed_level, ccp(size.width/2 + 177, size.height - 98 + 70)),
							   CCMoveTo::create(3.0 / speed_level, ccp(size.width/2 + 324, size.height - 164 + 70)),
							   NULL), 
			CCSequence::create(CCFadeTo::create(6.62f, 255),CCDelayTime::create(0),CCFadeTo::create(6.62f,0), NULL),
			NULL),
		CCRemoveSelf::create(true),
		NULL));
}

void MainFrameLayer::toShowStarEffect(float dt)
{
	ResManager* resManager = ResManager::getManager();

static int starEffectIdx = 0;
	if (++starEffectIdx % 2 == 0)
	{
//牛秋晨修改 5月7日
		const float ROTATE = 30.0f;
		const float TIME = 0.035f;
		mStar->runAction(CCSequence::create(
			CCScaleTo::create(0.1f, 1.05f),
			CCRotateBy::create(TIME, ROTATE),
			CCRotateBy::create(TIME, -ROTATE),
			CCRotateBy::create(TIME, -ROTATE),
			CCRotateBy::create(TIME, ROTATE),
			CCRotateBy::create(TIME, ROTATE),
			CCRotateBy::create(TIME, -ROTATE),
			CCScaleTo::create(0.1f, 1.0f),
			CCCallFuncN::create(this,SEL_CallFuncN(&MainFrameLayer::rotateBack)),
			NULL));
	}
	else
	{
		//大猩猩光效
		CCAnimation *anim = CCAnimation::create();
		char buf[128];
		for (int i=1; i<8; ++i)
		{
			sprintf(buf, (resManager->getSharedFilePath()+g_mainlayerPath+"main_title_xingxing_xiaoguo_%d.png").c_str(), i);
			//CCSpriteFrame::create(buf, CCRect(0,0,127,122));
			anim->addSpriteFrameWithFileName(buf);
		}
		anim->setDelayPerUnit(0.05f);
		//
		CCSprite *sss = CCSprite::create(resManager->getSharedFilePath(g_mainlayerPath+"main_title_xingxing_xiaoguo_1.png").c_str());
		sss->setPosition(ccp(mStar->getContentSize().width/2, mStar->getContentSize().height/2));
		mStar->addChild(sss);
		sss->runAction(CCSequence::create(
			CCAnimate::create(anim), CCRemoveSelf::create(true), NULL));
	}


	//4个随机点 闪烁星星
	CCPoint pos[4] = {ccp(190,243), ccp(290,285), ccp(530,286), ccp(632,244)};
#define COUNT 3
	CCSprite *star[COUNT];
	for (int i=0; i<COUNT; ++i)
	{
		star[i] = CCSprite::create(resManager->getSharedFilePath(g_mainlayerPath+"main_title_xiaoguo.png").c_str());
		mTilte->addChild(star[i]);
		int f = CCRANDOM_0_1() * 10;
		int idx = f % 4;
		star[i]->setPosition(pos[idx]);
//		star[i]->setPosition(pos[i]);
		star[i]->setOpacity(0);
		star[i]->runAction(CCSequence::create(
			CCDelayTime::create(StarEffect_Time / COUNT * (i+1)),
			CCFadeTo::create(0.15f, 255),
			CCDelayTime::create(0.25f),
			CCFadeTo::create(0.15f, 0),
			CCRemoveSelf::create(true),
			NULL));
	}
	
}

void MainFrameLayer::toShowStartMenuEffect(float dt)
{
	mStartItem->runAction(CCSequence::create(
		CCScaleTo::create(0.1f, 1.1f),
		CCScaleTo::create(0.1f, 1.0f),
		NULL));
}
void MainFrameLayer::moveChangeEnd(MainThingEffect *node)
{
	node->moveEffect();
}
void MainFrameLayer::moveThing()
{
	for (vector<MainThingEffect*>::iterator iter = m_sMainThingList.begin();iter != m_sMainThingList.end();iter++)
	{
		(*iter)->moveEffect();
	}
}
void MainFrameLayer::dmjEye(float dt)
{
	if (m_nDmjEyeCount == 0)
	{
		this->unschedule(SEL_SCHEDULE(&MainFrameLayer::dmjEye));
		this->schedule(SEL_SCHEDULE(&MainFrameLayer::dmjEye),KDMJEyeTime);
		map<int,CCSprite *>::iterator iter = m_sDmjEyeList.find(4);
		if (iter != m_sDmjEyeList.end())
		{
			iter->second->setVisible(false);
		}
	}
	else
	{
		map<int,CCSprite *>::iterator iter = m_sDmjEyeList.find(m_nDmjEyeCount);
		if (iter != m_sDmjEyeList.end())
		{
			iter->second->setVisible(false);
		}
	}
	m_nDmjEyeCount++;
	{
		map<int,CCSprite *>::iterator iter = m_sDmjEyeList.find(m_nDmjEyeCount);
		if (iter != m_sDmjEyeList.end())
		{
			iter->second->setVisible(true);
		}
	}

	if (m_nDmjEyeCount == 4)
	{
		m_nDmjEyeCount = 0;
		this->unschedule(SEL_SCHEDULE(&MainFrameLayer::dmjEye));
		this->schedule(SEL_SCHEDULE(&MainFrameLayer::dmjEye),KDMJEyeSideTime);
	}
}
void MainFrameLayer::qsyEye(float dt)
{
	if (m_nQsyEyeCount == 0)
	{
		this->unschedule(SEL_SCHEDULE(&MainFrameLayer::qsyEye));
	}
	else
	{
		map<int,CCSprite *>::iterator iter = m_sQsyEyeList.find(m_nQsyEyeCount);
		if (iter != m_sQsyEyeList.end())
		{
			iter->second->setVisible(false);
		}
	}
	m_nQsyEyeCount++;
	if (m_nQsyEyeCount == 6)
	{
		m_nQsyEyeCount = 0;
		this->unschedule(SEL_SCHEDULE(&MainFrameLayer::qsyEye));
		this->schedule(SEL_SCHEDULE(&MainFrameLayer::qsyEye),KQsyEyeSideTime);
		return;
	}
	else
	{
		map<int,CCSprite *>::iterator iter = m_sQsyEyeList.find(m_nQsyEyeCount);
		if (iter != m_sQsyEyeList.end())
		{
			iter->second->setVisible(true);
		}
	}
	if (m_nQsyEyeCount == 1)
	{
		this->schedule(SEL_SCHEDULE(&MainFrameLayer::qsyEye),KQsyEyeTime);
	}
	
}
void MainFrameLayer::dmjPushUpArm(float dt)
{
	if (m_nDmjArmCount == 0)
	{
		this->unschedule(SEL_SCHEDULE(&MainFrameLayer::dmjPushUpArm));
	}
	else
	{
		map<int,CCSprite *>::iterator iter = m_sDmjArmList.find(m_nDmjArmCount);
		if (iter != m_sDmjArmList.end())
		{
			iter->second->setVisible(false);
		}
	}

	m_nDmjArmCount++;
	{
		map<int,CCSprite *>::iterator iter = m_sDmjArmList.find(m_nDmjArmCount);
		if (iter != m_sDmjArmList.end())
		{
			iter->second->setVisible(true);
		}
	}

	if (m_nDmjArmCount == 3)
	{
		this->unschedule(SEL_SCHEDULE(&MainFrameLayer::dmjPushUpArm));
		this->schedule(SEL_SCHEDULE(&MainFrameLayer::dmjStar),0.1f);
	}
	else if (m_nDmjArmCount == 1)
	{
		this->schedule(SEL_SCHEDULE(&MainFrameLayer::dmjPushUpArm),KDMJPushUpTime);
	}
}
void MainFrameLayer::dmjPushDownArm(float dt)
{
	{
		map<int,CCSprite *>::iterator iter = m_sDmjArmList.find(m_nDmjArmCount);
		if (iter != m_sDmjArmList.end())
		{
			iter->second->setVisible(false);
		}
	}

	m_nDmjArmCount--;
	if (m_nDmjArmCount == 0)
	{
		this->unschedule(SEL_SCHEDULE(&MainFrameLayer::dmjPushDownArm));
		this->schedule(SEL_SCHEDULE(&MainFrameLayer::dmjPushUpArm),KDMJPushUpSideTime);
	}
	else
	{
		map<int,CCSprite *>::iterator iter = m_sDmjArmList.find(m_nDmjArmCount);
		if (iter != m_sDmjArmList.end())
		{
			iter->second->setVisible(true);
		}
	}
}
void MainFrameLayer::dmjStar(float dt)
{
	CCSprite *starSpr = NULL;
	int apha = 0;
	if (m_nDmjStarCount < 10)
	{
		apha = ( m_nDmjStarCount + 1 ) * 255 /10;
	}
	else
	{
		apha = 255;
	}

	int starCount = m_nDmjStarCount % 5;

	{
		map<int,CCSprite *>::iterator iter = m_sDmjStarList.find(starCount);
		if (iter != m_sDmjStarList.end())
		{
			iter->second->setVisible(false);
		}
	}

	m_nDmjStarCount++;
	{
		if (starCount == 4)
		{
			map<int,CCSprite *>::iterator iter = m_sDmjStarList.find(1);
			if (iter != m_sDmjStarList.end())
			{
				iter->second->setVisible(true);
				iter->second->setOpacity(apha);
				starSpr = iter->second;
			}
		}
		else
		{
			map<int,CCSprite *>::iterator iter = m_sDmjStarList.find(starCount + 1);
			if (iter != m_sDmjStarList.end())
			{
				iter->second->setVisible(true);
				iter->second->setOpacity(apha);
				starSpr = iter->second;
			}
		}

	}
	if (m_nDmjStarCount == (int)(KDMJPushDownTime * 1.0f / 0.1f) )
	{
		m_nDmjStarCount = 0;
		this->unschedule(SEL_SCHEDULE(&MainFrameLayer::dmjStar));
		this->schedule(SEL_SCHEDULE(&MainFrameLayer::dmjPushDownArm),0.1f);

		CCFadeOut *out = CCFadeOut::create(KDMJPushUpSideTime - 1.0f);
		starSpr->stopAllActions();
		starSpr->runAction(out);
	}
}
void MainFrameLayer::rotateBack(CCNode *node)
{
	node->setRotation(m_nStarRotate);
}


void MainFrameLayer::fillOuty(CCMenuItemImage* p,string imgfile){

	CCSprite* s=CCSprite::create(imgfile.c_str());
	s->setScale(1.02f);
	p->setSelectedImage(s);



}

void MainFrameLayer::setClicked(CCObject* obj){


}
void MainFrameLayer::achiveClicked(CCObject* obj){


}
void MainFrameLayer::gamecenterClicked(CCObject* obj){


}

void MainFrameLayer::facebookClicked(CCObject* obj){



}


void MainFrameLayer::xinUi(){

	   
	 

}

void MainFrameLayer::yuntuoXian(){

	 string imgfile=ResManager::getManager()->getFilePath(g_mainlayerPath+"ZY_xiaoguo_yunduo.png");

	 CCPoint pleft=CCPoint(0,hight*0.6);
	 CCPoint pright=CCPoint(width,hight*0.625);

	 for(int i=1;i<=6;i++){

	    CCSprite* p= CCSprite::create(imgfile.c_str());
		p->setOpacity(50);
		p->setRotation(45.0f);
		if(i<=3){
			float scalef=0.8f- 1.0*(i-1)/10;
			p->setScale(scalef);
			p->setPosition(pleft);
			p->runAction( CCMoveTo::create(2.0f,CCPoint(p->getPositionX()+(i-1)*50,p->getPositionY())));

		}else{

			float scalef=0.5f- 1.0*(i-4)/10;
			p->setScale(scalef);
			p->setPosition(pright);
			p->runAction(CCMoveTo::create(2.0f,CCPoint(p->getPositionX()-((i-3)-1)*25,p->getPositionY())));
		}
		addChild(p);
	 }

	 Buttonfadein();


}


void MainFrameLayer::Buttonfadein(){


	//facebook
	string facebookimg=ResManager::getManager()->getFilePath(g_mainlayerPath+"ZY_anniu_facebook.png");
	CCMenuItemImage* facebooklogin = CCMenuItemImage::create(facebookimg.c_str(),facebookimg.c_str(),
		this,SEL_MenuHandler(&MainFrameLayer::facebookClicked));

	facebooklogin->setPosition(CCPoint(20+facebooklogin->getContentSize().width/2,facebooklogin->getContentSize().height/2+20));
	fillOuty(facebooklogin,facebookimg);
	facebooklogin->setOpacity(0);
	facebooklogin->runAction(CCFadeIn::create(1.0f));

   //gamecenter
	string gameceter=ResManager::getManager()->getFilePath(g_buttonPath+"ZY_anniu_gamecenter.png");
	CCMenuItemImage* gameceterbutton = CCMenuItemImage::create(gameceter.c_str(),gameceter.c_str(),
		this,SEL_MenuHandler(&MainFrameLayer::gamecenterClicked));

	gameceterbutton->setPosition(CCPoint(facebooklogin->getPositionX()+facebooklogin->getContentSize().width/2+20+gameceterbutton->getContentSize().width/2,gameceterbutton->getContentSize().height/2+20));
	fillOuty(gameceterbutton,gameceter);
	gameceterbutton->setOpacity(0);
	gameceterbutton->runAction(CCFadeIn::create(1.0f));



	//设置
	string setflie=ResManager::getManager()->getFilePath(g_buttonPath+"ZY_anniu_shezhi.png");
	CCMenuItemImage* setbutton = CCMenuItemImage::create(setflie.c_str(),setflie.c_str(),
		this,SEL_MenuHandler(&MainFrameLayer::setClicked));

	setbutton->setPosition(CCPoint(width-20-setbutton->getContentSize().width/2,setbutton->getContentSize().height/2+20));
	fillOuty(setbutton,setflie);
	setbutton->setOpacity(0);
	setbutton->runAction(CCFadeIn::create(1.0f));


   //成就
    string aciveflie=ResManager::getManager()->getFilePath(g_buttonPath+"ZY_anniu_chengjiu.png");
	CCMenuItemImage* acivebutton = CCMenuItemImage::create(aciveflie.c_str(),aciveflie.c_str(),
		this,SEL_MenuHandler(&MainFrameLayer::achieveClicked));
	acivebutton->setPosition(CCPoint(setbutton->getPositionX()-setbutton->getContentSize().width/2-20-acivebutton->getContentSize().width/2,acivebutton->getContentSize().height/2+20));
	fillOuty(acivebutton,aciveflie);
	acivebutton->setOpacity(0);
	acivebutton->runAction(CCFadeIn::create(1.0f));


	CCMenu* fmenu = CCMenu::create(facebooklogin,setbutton,acivebutton,gameceterbutton,NULL);
	fmenu->setPosition(CCPointZero);
	addChild(fmenu,129);

}


void MainFrameLayer::onFrameEvent(CCBone *bone, const char *evt, int originFrameIndex, int currentFrameIndex){

	
	 string name=evt;
	 if(originFrameIndex==62&&name.compare("wenzitihuan")==0&&mark==1){
		CCNode* nd=armature->getBone("biaoti_fubiaoti")->getDisplayRenderNode();
	    nd->setVisible(false);
		//CCPoint p= armature->getBone("biaoti_fubiaoti")->getDisplayRenderNode()->convertToWorldSpaceAR(ccp(0, 0));
		CCLabelTTF* pLabel = CCLabelTTF::create(LFStrings::getValue("miaoshu").c_str(),fontStr_KlavikaBold, 28);
		CCSprite* back_node=(CCSprite*)getChildByTag(back_tag);
		ccColor3B color=back_node->getDisplayedColor();//back_node->getColor();
		pLabel->setColor(color);
		pLabel->setPosition(ccp(width/2,hight*0.63));
		pLabel->setZOrder(1025);
		addChild(pLabel);
	 } 

	 if(mark==1&&originFrameIndex==95){

		CCNode* nd=armature->getBone("anniu_play")->getDisplayRenderNode();
		nd->setVisible(false);
		mark=0;
		CCLabelTTF* pLabel = CCLabelTTF::create(LFStrings::getValue("play").c_str(),fontStr_KlavikaBold, 40);
		if(ResManager::getManager()->getLanguageType()==kLanguageChinese){
			pLabel->setFontSize(40);
		}else if(ResManager::getManager()->getLanguageType()==kLanguageEnglish){
			pLabel->setFontSize(60);
		}

//		Cutout(pLabel);

		pLabel->setPosition(ccp(width/2,hight*0.45));
		pLabel->setZOrder(1025);
		addChild(pLabel);
	 }
}

void MainFrameLayer::animationEvent(CCArmature *armature, MovementEventType movementType, const char *movementID){


	if (movementType == COMPLETE){
		if(armature->getTag()==armrate_chuxiantag){
			//armature->getAnimation()->setMovementEventCallFunc(this,nullptr);

			armature->getAnimation()->play("zhuye_dengdai");
			
			//出现云朵
            yuntuoXian();

		}
	}

}

void MainFrameLayer::playerAnimation(CCNode* node,string animationame,CCPoint point,bool isloop){

	  //chengjiu_chuxian 新成就
	  //chengjiu_dengdai 成就领取
	
	  armature=CCArmature::create(UI_animation_data);
	  if(isloop==true){
		  armature->getAnimation()->play(animationame.c_str(),-1,1);
	  }else{
		  armature->getAnimation()->play(animationame.c_str());
		  if(animationame.compare("zhuye_chuxian")==0){  
			  armature->getAnimation()->setMovementEventCallFunc(this, movementEvent_selector(MainFrameLayer::animationEvent));
			  armature->getAnimation()->setFrameEventCallFunc(this, frameEvent_selector(MainFrameLayer::onFrameEvent));
			  armature->setTag(armrate_chuxiantag);
		  }
	  }
	  armature->setZOrder(1025);
	  armature->setPosition(point);
	  node->addChild(armature);



}

void MainFrameLayer::onEnter(){

	
	BaseMainOptionLayer::onEnter();
}

void MainFrameLayer::dataLoaded(float percent){

	if (percent < 1) return;
	playerAnimation(this,"zhuye_chuxian",CCPoint(width/2,hight/2),false);
}


void MainFrameLayer::onExit(){


	m_sMainThingList.clear();
	m_sDmjEyeList.clear();
	m_sQsyEyeList.clear();

	BaseMainOptionLayer::onExit();
//	CCNotificationCenter::sharedNotificationCenter()->removeObserver(this,MSG_PHONENUMBER_INPUT_OK);
}

MainFrameLayer::~MainFrameLayer()
{
	CCArmatureDataManager::sharedArmatureDataManager()->removeArmatureFileInfo(ResManager::getManager()->getFilePath(UI_mainaramite_name).c_str());
}



/*
armature = cocos2d::extension::CCArmature::create("Cowboy");
armature->getAnimation()->play("FireWithoutBullet");
armature->getAnimation()->setSpeedScale(0.2f);
armature->setScaleX(-0.2f);
armature->setScaleY(0.2f);
armature->setPosition(ccp(VisibleRect::left().x + 70, VisibleRect::left().y));
//帧动画
armature->getAnimation()->setFrameEventCallFunc(this, frameEvent_selector(TestColliderDetector::onFrameEvent));
addChild(armature);
*/
