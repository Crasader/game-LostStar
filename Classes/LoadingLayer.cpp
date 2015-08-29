#include "LoadingLayer.h"
#include "ResManager.h"
#include "Logo.h"
#include "GameInforEditer.h"
#include "GameLayerFactory.h"
#include "SpriteHelper.h"
#include "LevelManager.h"
#include "SpriteHelper.h"
#include "AchievementManager.h"
#include "Music.h"
#include "LFShareKit.h"
#include "LFStrings.h"
#include "font.h"
#include "JuqingDaoJuManager.h"
#include "VersionControl.h"
#include "History.h"
#include "UserDataManager.h"
#include "ResManager.h"
#include "SocialSeasonInfor.h"

#define USE_ARMATURE		1

//Intent_Section2GameIn 的情况需要传入int seasonId
LoadingLayer::LoadingLayer(LoadingIntent intent,int seasonId /*= -1*/)
{
	Music::sharedMusicPlayer()->pauseBackgroundSound();

	m_bTextureLoadingOver = false;
	m_bAnimationOver = false;
	needLoadImageNum = 0;
	mIntent = intent;
	mSeasonId = seasonId;
	barMoveSide = NULL;

	// 载入load的tips
	{
		// 
		if (mIntent != Intent_AppDelegate2Main)
		{
			char temp[64];
			string tip;
			for (int i=1;;i++)
			{
				sprintf(temp,"loading_tips%d",i);
				tip = LFStrings::getValue(temp);
				if (tip == "")
					break;
				loadingTips.push_back(tip);
			}

			changeTip(0);
		}
	}

	// 先释放
	TexturePreLoad::getInstance()->clearCurrentLoadedTexture();

	switch(mIntent)
	{
	case Intent_AppDelegate2Main:
	case Intent_GameIn2Season:
	case Intent_GameIn2Main:
	case Intent_GameIn2Section:
		createOutterLoading();
		schedule(schedule_selector(LoadingLayer::startOuterLoading),0.01f);
		break;
	case Intent_Section2GameIn:
		CCAssert(mSeasonId!=-1,"必须传入SeasonId");
		createInnerLoading();
		schedule(schedule_selector(LoadingLayer::startInnerLoading),0.01f);
		break;
	}
}

#define TIP_TAG	987690
void LoadingLayer::changeTip(float time)
{
	if (loadingTips.empty())
		return;

	size_t index = CCRANDOM_0_1() * loadingTips.size();
	if (index >= loadingTips.size())
	{
		index = loadingTips.size()-1;
	}

	CCLabelTTF* tip = NULL;
	if (getChildByTag(TIP_TAG) == NULL)
	{
		tip = CCLabelTTF::create(loadingTips[index].c_str(),fontStr_katong,30,CCSizeMake(1000,0),kCCTextAlignmentCenter);
		tip->setAnchorPoint(ccp(0.5f,0.5f));
		this->addChild(tip,1024,TIP_TAG);
		tip->setPosition(ccp(getContentSize().width/2,getContentSize().height/8));
		tip->setColor(ccWHITE);
	}else
	{
		tip = (CCLabelTTF*)getChildByTag(TIP_TAG);
		tip->setString(loadingTips[index].c_str());
	}

}

void LoadingLayer::createInnerLoading()
{
	ResManager::getManager()->loadAllArmatureInfo();
    m_bAnimationOver = true;
}
void LoadingLayer::createOutterLoading()
{
	m_bAnimationOver = true;
	ResManager::getManager()->removeAllArmatureInfo();
}

void LoadingLayer::outlayerLoadover(CCNode* pNode)
{
	m_bAnimationOver = true;
	loadOver();
}

void LoadingLayer::startInnerLoading(float time)
{
	unschedule(schedule_selector(LoadingLayer::startInnerLoading));

	loadInnterEffectiveSound();
	m_bTextureLoadingOver = false;
	needLoadImageNum = TexturePreLoad::getInstance()->preLoadInnerTexture(mSeasonId);
	TexturePreLoad::getInstance()->startAsync(this);
}
void LoadingLayer::loadingInnerOver()
{
	scheduleOnce(schedule_selector(LoadingLayer::goToSection),0.01f);
}
void LoadingLayer::goToSection(float time)
{
	int seasonId = LevelManager::getManager()->getLastSeasonId();
	int sectionId = LevelManager::getManager()->getLastSectionId();
	LevelManager::getManager()->setShowLoadingAnimation(true);
	LevelManager::getManager()->goTo(seasonId,sectionId);
}
void LoadingLayer::startOuterLoading(float time)
{
	unschedule(schedule_selector(LoadingLayer::startOuterLoading));
	// 先释放
	TexturePreLoad::getInstance()->clearCurrentLoadedTexture();

	if (mIntent == Intent_AppDelegate2Main)
	{
		History::getHistory()->initData();
		GameLayerFactory::getInstance()->loadAllGameLayerBySeasonInfo();
		GameLayerFactory::getInstance()->initAllNewitemsGameLayerInfo();
		GameInforEditer::getGameInfor()->initData();//载入文件初始化;
		SocialInfoManager::getInstance()->initLocalData();//载入模拟好友关卡信息
		UserDataManager::getInstance()->initData();
		AchievementManager::getInstance()->initData();
		LFStrings::init();//确保  string在GameInforEditer之后初始化，因为需要用到上次游戏保存的语言设置
		//CmdHelper::getInstance()->initSyncServerData();


		ResManager* resmgr = ResManager::getManager();
		for (int i = 0; i < 5; i++)
		{
			CCTextureCache::sharedTextureCache()->addImage(CCString::createWithFormat("background_s%d.png", i+1)->getCString());
		}

#if 0
		CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("scale9sprites.plist");

		for (int i = 0; i < 5; i++)
		{
			CCTextureCache::sharedTextureCache()->addImage(resmgr->getFilePath(g_mainlayerPath+CCString::createWithFormat("ZJ_bg_season%d.png", i+1)->getCString()).c_str());
		}
#endif
		//屏蔽剧情道具相关
//		JuqingDaoJuManager::getManager();

		//CmdHelper::getHelper()->cmdSavePayInfoToServer();

		checkVersionCode();

// 		//提交支付信息到我们的服务器上需要ClientId， 所以需要先注册一个ClientId
// 		//pp也是使用的我们随机的用户名，没有办法获取用户名
// #if ( Version_Platform != 10 && Version_Platform != 11 && Version_Platform != 13 ) || CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
// 		if (GameInforEditer::getGameInfor()->getClientId() == 0)
// 		{
// 			RedStarSDKManager::getInstance()->loginCallBack(1, CmdHelper::getHelper()->getRandUserName());
// 		}
// #endif
	}

	//add by jj
//	loadInnterEffectiveSound();
	m_bTextureLoadingOver = false;
	needLoadImageNum = TexturePreLoad::getInstance()->preLoadOuterTexture();
	TexturePreLoad::getInstance()->startAsync(this);
}
void LoadingLayer::loadOuterEffectiveSound()
{
	Music* music = Music::sharedMusicPlayer();
	// 先释放
	music->unLoadAllEffectSound();

	music->loadEffectSound("button.wav");
	music->loadEffectSound("curtain_close.wav");
	music->loadEffectSound("curtain_open.wav");
	music->loadEffectSound("season_clicked.wav");
	music->loadEffectSound("section_clicked.wav");
	music->loadEffectSound("sectionItemAppear.wav");
}
void LoadingLayer::loadInnterEffectiveSound()
{
	Music* music = Music::sharedMusicPlayer();
	// 先释放
	music->unLoadAllEffectSound();

	music->loadEffectSound("button.wav");
	music->loadEffectSound("curtain_close.wav");
	music->loadEffectSound("curtain_open.wav");

	music->loadEffectSound("achievement.wav");
	music->loadEffectSound("black_hole_bomb.wav");
	music->loadEffectSound("black_hole_sound.wav");
	music->loadEffectSound("boiler_openDoor.wav");
	music->loadEffectSound("boiler_sound.wav");
	music->loadEffectSound("bomb_explosion_sound_0_5.wav");
	music->loadEffectSound("bomb_explosion_sound_0_25.wav");
	music->loadEffectSound("bomb_explosion_sound_1_0.wav");
	music->loadEffectSound("box_bomb_big1.wav");
	music->loadEffectSound("box_bomb_big2.wav");
	music->loadEffectSound("box_bomb_big3.wav");
	music->loadEffectSound("box_bomb_small1.wav");
	music->loadEffectSound("box_bomb_small2.wav");
	music->loadEffectSound("box_bomb_small3.wav");
	music->loadEffectSound("cut_rope_sound.wav");
	music->loadEffectSound("dida_L1_5.wav");
	music->loadEffectSound("ding.wav");
	music->loadEffectSound("dudu.wav");
	music->loadEffectSound("laugh.wav");
	music->loadEffectSound("motor_sound.wav");
	music->loadEffectSound("star1.wav");
	music->loadEffectSound("star2.wav");
	music->loadEffectSound("star3.wav");
	music->loadEffectSound("strobe_close.wav");
	music->loadEffectSound("strobe_open.wav");
	music->loadEffectSound("timer_bomb_dur.wav");
	music->loadEffectSound("timer_bomb_sound.wav");
	music->loadEffectSound("win.wav");

	music->loadEffectSound("kuaimen.wav");
	music->loadEffectSound("loading.wav");
	music->loadEffectSound("juqingdaoju_souji.wav");


	float volumeSound[] = {0.2f,0.4f,0.6f,0.8f,1.0f,1.2f};
	char temp[128];
	for (int i=0;i<sizeof(volumeSound)/sizeof(float);i++)
	{
		for (int j=0;j<5;j++)
		{
			sprintf(temp, "wood collision a%d.wav", j+1);
			string fileName = music->getDefaultVolumeFile(temp,volumeSound[i]);
			music->loadEffectSound(fileName);
		}
		string fileName2 = music->getDefaultVolumeFile("ball_bounce.wav",volumeSound[i]);
		music->loadEffectSound(fileName2);
	}
}
void LoadingLayer::loadingOuterOver(CCNode* node)
{
	////再同步的载入一次 (不能，iOS会崩溃)
	if (mIntent == Intent_AppDelegate2Main)
	{
        LaunchIconLayer* pLaunchIconLayer = (LaunchIconLayer*)CCDirector::sharedDirector()->getRunningScene()->getChildByTag(LaunchIconLayer::iTag_LaunchIconLayer);
        if (pLaunchIconLayer)
        {
            pLaunchIconLayer->startAnimation();
        }
        else
        {
            scheduleOnce(schedule_selector(LoadingLayer::goToMainPage),0.1f);
        }
		// 载入完毕之后再校验，以免出错
		{
			// 会自动验证GameCenter
			LFShareKit::getKit();
		}
	}
	else if (mIntent == Intent_GameIn2Main)
	{
		scheduleOnce(schedule_selector(LoadingLayer::goToMainPage),0.2f);
	}
	else if (mIntent == Intent_GameIn2Section)
	{
		scheduleOnce(schedule_selector(LoadingLayer::goToSectionPage),0.2f);
	}else if (mIntent == Intent_GameIn2Season)
	{
		scheduleOnce(schedule_selector(LoadingLayer::goToSeasonPage),0.2f);
	}
}
void LoadingLayer::goToSectionPage(float time)
{
	LevelManager::getManager()->goToSectionUI(false);
}
void LoadingLayer::goToSeasonPage(float time)
{
	LevelManager::getManager()->goToSeasonUI(true);
}
void LoadingLayer::goToMainPage(float time)
{
	CCScene* scene = Logo::createLogoScene();
//	CCTransitionFade* tran = CCTransitionFade::create(0.01f,scene);
	CCDirector::sharedDirector()->replaceScene(scene);
}
void LoadingLayer::loadOver()
{
	float end = 90;
	// 取消回调
	TexturePreLoad::getInstance()->cancelCallback();
	m_bTextureLoadingOver = true;
	switch(mIntent)
	{
	case  Intent_AppDelegate2Main:
		{
			loadingOuterOver(NULL);
		}
		break;
	case  Intent_GameIn2Section:
	case  Intent_GameIn2Main:
	case  Intent_GameIn2Season:
		{
			if (m_bAnimationOver)
			{
				loadingOuterOver(NULL);
			}
		}
		break;
	case Intent_Section2GameIn:
		{
            if (m_bAnimationOver)
            {
                loadingInnerOver();
            }
		}
		break;
	}
}
void LoadingLayer::textureLoaded(int waitingTaskNum,string fileName){}

void LoadingLayer::checkVersionCode()
{
	string cur_ver = CCUserDefault::sharedUserDefault()->getStringForKey("Current_Version_Code", "1.0");
	if (cur_ver != Current_Version_Code)
	{
		CCLOG("old version = %s, cur version = %s", cur_ver.c_str(), Current_Version_Code);

		//现在不清空了
		//CmdHelper::getHelper()->resetAllData(true);
		CCUserDefault::sharedUserDefault()->setStringForKey("Current_Version_Code", Current_Version_Code);
		CCUserDefault::sharedUserDefault()->flush();
	}
}

LaunchIconLayer::~LaunchIconLayer()
{
#if	USE_ARMATURE != 0
	CCArmatureDataManager::sharedArmatureDataManager()->removeArmatureFileInfo("2F_logo.ExportJson");
#endif
}

LaunchIconLayer::LaunchIconLayer()
{
	init();

	CCLayerColor* pLayerColor = CCLayerColor::create(ccc4(255, 255, 255, 255));
	this->addChild(pLayerColor, -1, iTag_LayerColor);

	m_fDelaytime = 1.0f;

#if	USE_ARMATURE != 0
	CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo("2F_logo.ExportJson");
    scheduleOnce(schedule_selector(LaunchIconLayer::play), 1.0f);
#else
	ResManager* resManger = ResManager::getManager();
	m_LogoFileNameVec.push_back(resManger->getFilePath(g_loading_logoPath+"camellia_logo.png", true).c_str());

	CCSprite* pLogo = CCSprite::create(m_LogoFileNameVec.at(0).c_str());
	pLogo->setPosition(pLayerColor->getContentSize()/2);
	this->addChild(pLogo, 1, iTag_Logo);
    
    pLogo->setScale(0.75f);

	startTrueLoad(NULL);
#endif
}

void LaunchIconLayer::play(float dt)
{
#if	USE_ARMATURE != 0
    CCArmature* pArmature = CCArmature::create("2F_logo");
    pArmature->getAnimation()->play("logo");
    pArmature->setPosition(this->getContentSize()/2);
    pArmature->getAnimation()->setMovementEventCallFunc(this, movementEvent_selector(LaunchIconLayer::MovementEventCallFunc));
    this->addChild(pArmature, 2, iTag_Logo_Armature);
    pArmature->setScale(0.75f);
#endif
}

void LaunchIconLayer::MovementEventCallFunc(CCArmature *pArmature, MovementEventType movementType, const char *movementID)
{
	std::string id = movementID;

	if (movementType == LOOP_COMPLETE)
	{
	}
	else if (movementType == COMPLETE)
	{
		startTrueLoad(NULL);
	}
	else if (movementType == START)
	{

	}
}

void LaunchIconLayer::startAnimation()
{
#if USE_ARMATURE == 0
	if (m_LogoFileNameVec.size() == 1)
#endif
	{
		CCLayerColor* pLayerColor = (CCLayerColor*)this->getChildByTag(iTag_LayerColor);
		pLayerColor->runAction(CCSequence::create(
			CCDelayTime::create(m_fDelaytime+0.3f),
			CCFadeOut::create(m_fDelaytime/2+0.1f),
			CCCallFunc::create(this, callfunc_selector(LaunchIconLayer::goToMainLayer)),
			CCRemoveSelf::create(),
			NULL));
	}

#if USE_ARMATURE != 0
	CCArmature* pLogoArmature = (CCArmature*)this->getChildByTag(iTag_Logo_Armature);
	pLogoArmature->runAction(CCSequence::create(
		CCDelayTime::create(m_fDelaytime),
		CCFadeOut::create(m_fDelaytime/2),
		NULL));
#else
	CCSprite* pLogo = (CCSprite*)this->getChildByTag(iTag_Logo);
	pLogo->runAction(CCSequence::create(
		CCDelayTime::create(m_fDelaytime),
		CCFadeOut::create(m_fDelaytime/2),
		CCCallFuncN::create(this, callfuncN_selector(LaunchIconLayer::oneLogoShowDone)),
		NULL));
#endif
}

void LaunchIconLayer::oneLogoShowDone(CCNode* pNode)
{
#if USE_ARMATURE == 0
	m_LogoFileNameVec.erase(m_LogoFileNameVec.begin());
	if (m_LogoFileNameVec.size() > 0)
	{
		CCSprite* pTemp = CCSprite::create(m_LogoFileNameVec.at(0).c_str());
		((CCSprite*)pNode)->initWithTexture(pTemp->getTexture(), pTemp->getTextureRect());
		((CCSprite*)pNode)->setOpacity(0);
		((CCSprite*)pNode)->runAction(CCSequence::create(
			CCFadeTo::create(m_fDelaytime*3, 255),
			CCDelayTime::create(m_fDelaytime),
			CCFadeOut::create(m_fDelaytime/2),
			CCCallFuncN::create(this, callfuncN_selector(LaunchIconLayer::oneLogoShowDone)),
			NULL));

		if (m_LogoFileNameVec.size() == 1)
		{
			CCLayerColor* pLayerColor = (CCLayerColor*)this->getChildByTag(iTag_LayerColor);
			if (pLayerColor)
			{
				pLayerColor->runAction(CCSequence::create(
				CCDelayTime::create(m_fDelaytime*4+0.3f),
				CCFadeOut::create(m_fDelaytime/2+0.1f),
				CCCallFunc::create(this, callfunc_selector(LaunchIconLayer::goToMainLayer)),
				CCRemoveSelf::create(),
				NULL));
			}
		}
	}
#endif
}

void LaunchIconLayer::startTrueLoad(CCNode* null_obj)
{
	CCTextureCache::sharedTextureCache()->removeUnusedTextures();
//	Music::sharedMusicPlayer()->playEffectSound("loading.wav");
	LoadingLayer* layer = new LoadingLayer(Intent_AppDelegate2Main);
	addChild(layer, 1, iTag_LoadingLayer);
    layer->release();
}

void LaunchIconLayer::removeNode(CCNode* node)
{
	if (node != NULL)
	{
		node->removeFromParent();
	}
}

void LaunchIconLayer::goToMainLayer()
{
    LoadingLayer* pLaodingLayer = (LoadingLayer*)this->getChildByTag(iTag_LoadingLayer);
    pLaodingLayer->goToMainPage(0);
}



