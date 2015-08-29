#include "BaseMainOptionLayer.h"
#include "Music.h"
#include "LFShareKit.h"
#include "GameInforEditer.h"
#include "ResManager.h"
#include "TeamInfoLayer.h"
#include "GameInforEditer.h"
#include "GameLayerFactory.h"
#include "VersionControl.h"
#include "MessageDisplay.h"
#include "LFStrings.h"
string downLoadPage = "http://www.camellia-network.com/";

string _shareToSinaContent = "我正在玩《红星大挪移》真给力！大挪移不是你所想的那个挪移，而是这个挪移。蒙圈了吧，过来试试就知道了。\n"+downLoadPage;
string _shareSinaContent_coded = "%E6%88%91%E6%AD%A3%E5%9C%A8%E7%8E%A9%E3%80%8A%E7%BA%A2%E6%98%9F%E5%A4%A7%E6%8C%AA%E7%A7%BB%E3%80%8B%E7%9C%9F%E7%BB%99%E5%8A%9B%EF%BC%81%E5%A4%A7%E6%8C%AA%E7%A7%BB%E4%B8%8D%E6%98%AF%E4%BD%A0%E6%89%80%E6%83%B3%E7%9A%84%E9%82%A3%E4%B8%AA%E6%8C%AA%E7%A7%BB%EF%BC%8C%E8%80%8C%E6%98%AF%E8%BF%99%E4%B8%AA%E6%8C%AA%E7%A7%BB%E3%80%82%E8%92%99%E5%9C%88%E4%BA%86%E5%90%A7%EF%BC%8C%E8%BF%87%E6%9D%A5%E8%AF%95%E8%AF%95%E5%B0%B1%E7%9F%A5%E9%81%93%E4%BA%86%E3%80%82%20"+downLoadPage;

string _shareToFB_TwitContent[] = {
	"I came to season one Getting Started in PICK UP THE STAR.Let's help the girl to get the shiny star together! "+downLoadPage,
	"I came to season two The Motor in PICK UP THE STAR.Let's help the girl to get the shiny star together! "+downLoadPage,
	"I came to season three The Boiler in PICK UP THE STAR.Let's help the girl to get the shiny star together! "+downLoadPage,
	"I came to season four Frozen Experiment in PICK UP THE STAR.Let's help the girl to get the shiny star together! "+downLoadPage,
	"I came to season five Chaos Space  in PICK UP THE STAR.Let's help the girl to get the shiny star together! "+downLoadPage
};

extern bool g_NeedRetina;

BaseMainOptionLayer::BaseMainOptionLayer(void)
{
	//ResManager::getManager()->setPrefixPath("global_public_pic/");//用全局的图片
	drawerOuted = true;
	action = false;
// 	facebook = CCMenuItemImage::create(
// 		getString("facebook.png").c_str(),
// 		getString("facebook_select.png").c_str(),
// 		this,
// 		SEL_MenuHandler(&BaseMainOptionLayer::facebookClicked));
// 
// 	twitter = CCMenuItemImage::create(
// 		getString("twitter.png").c_str(),
// 		getString("twitter_select.png").c_str(),
// 		this,
// 		SEL_MenuHandler(&BaseMainOptionLayer::twitterClicked));
// 
// 	itemSina = CCMenuItemImage::create(
// 		getString("weibo.png").c_str(),
// 		getString("weibo_select.png").c_str(),
// 		this,
// 		SEL_MenuHandler(&BaseMainOptionLayer::sinaClicked));

	resetGame = CCMenuItemImage::create(
		getSharedFilePath(g_buttonPath+"restart.png").c_str(),
		getSharedFilePath(g_buttonPath+"restart_select.png").c_str(),
		this,
		SEL_MenuHandler(&BaseMainOptionLayer::resetGameItemClick));

	setItem = CCMenuItemImage::create(
		getSharedFilePath(g_buttonPath+"main_button.png").c_str(),
		getSharedFilePath(g_buttonPath+"main_button_select.png").c_str(),
		this,
		SEL_MenuHandler(&BaseMainOptionLayer::settingClicked)
		);

	//音效开关
	{
		if ( Music::sharedMusicPlayer()->isEffectiveMute() )
		{
			itemEffctiveSound = CCMenuItemImage::create(
				getSharedFilePath(g_buttonPath+"set_sound_off.png").c_str(),
				getSharedFilePath(g_buttonPath+"set_sound_off_select.png").c_str(),
				this,
				SEL_MenuHandler(&BaseMainOptionLayer::effctiveSoundClicked));
		}else
		{
			itemEffctiveSound = CCMenuItemImage::create(
				getSharedFilePath(g_buttonPath+"set_sound_on.png").c_str(),
				getSharedFilePath(g_buttonPath+"set_sound_on_select.png").c_str(),
				this,
				SEL_MenuHandler(&BaseMainOptionLayer::effctiveSoundClicked));
		}
	}
	
	//背景音乐开关
	{
		if ( Music::sharedMusicPlayer()->isBackgroundMute() )
		{
			itemBackgroundSound = CCMenuItemImage::create(
				getSharedFilePath(g_buttonPath+"set_music_off.png").c_str(),
				getSharedFilePath(g_buttonPath+"set_music_off_select.png").c_str(),
				this,
				SEL_MenuHandler(&BaseMainOptionLayer::soundClicked));
		}else
		{
			itemBackgroundSound = CCMenuItemImage::create(
				getSharedFilePath(g_buttonPath+"set_music_on.png").c_str(),
				getSharedFilePath(g_buttonPath+"set_music_on_select.png").c_str(),
				this,
				SEL_MenuHandler(&BaseMainOptionLayer::soundClicked));
		}
	}
	
	leaderboardImage = CCMenuItemImage::create(
		getSharedFilePath(g_buttonPath+"gamecenter.png").c_str(),
		getSharedFilePath(g_buttonPath+"gamecenter_select.png").c_str(),this,SEL_MenuHandler(&BaseMainOptionLayer::menuLeaderboardCallback));

	itemExit = CCMenuItemImage::create(
		getSharedFilePath(g_buttonPath+"set_exit.png").c_str(),
		getSharedFilePath(g_buttonPath+"set_exit_select.png").c_str(),
		this,SEL_MenuHandler(&BaseMainOptionLayer::menuExitCallback));
    
    userInfoBtn = CCMenuItemImage::create(
                                          getSharedFilePath(g_buttonPath+"set_gerenxinxi.png").c_str(),
                                          getSharedFilePath(g_buttonPath+"set_gerenxinxi_select.png").c_str(),
                                          this,SEL_MenuHandler(&BaseMainOptionLayer::menuUserInfoCallBack));


	//game_center = CCMenuItemImage::create(
	//	getString("gamecenter_main.png").c_str(),
	//	getString("gamecenter_main_select.png").c_str(),
	//	this,
	//	SEL_MenuHandler(&BaseMainOptionLayer::gameCenterItemClick));

	//exitImage = CCMenuItemImage::create(
	//	getString("exit_main.png").c_str(),
	//	getString("exit_main_select.png").c_str(),this,SEL_MenuHandler(&BaseMainOptionLayer::menuExitCallback));
	//if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	//	exitImage->setVisible(false);
	//else
	//	exitImage->setVisible(true);
	//frameButtons = CCMenu::create(itemSina,facebook,twitter,itemBackgroundSound,itemEffctiveSound,itemInfo,game_center,resetGame,exitImage,leaderboardImage,NULL);
	
	CCArray* items = CCArray::create();
// 	items->addObject(itemSina);
// 	items->addObject(facebook);
// 	items->addObject(twitter);
	items->addObject(itemBackgroundSound);
	items->addObject(itemEffctiveSound);


//	CCSprite* content = CCSprite::create(ResManager::getManager()->getFilePathByLanguageType("Credits.png").c_str());
//	if (content->getContentSize().width > 10 && content->getContentSize().height > 10)
	{
		items->addObject(itemExit);
		itemExit->setVisible(false);
		itemExit->setEnabled(false);
	}
// 	else
// 	{
// 		itemExit = NULL;
// 	}


	if (NeedExitButton != 1)
	{
		itemExit->setVisible(false);
		itemExit = NULL;
	}
    
    if (NeedUserInfoBtn != 0)
    {
        items->addObject(userInfoBtn);
    }
    else
    {
        userInfoBtn = NULL;
    }

	items->addObject(resetGame);
	items->addObject(leaderboardImage);
	items->addObject(setItem);

	frameButtons = CCMenu::createWithArray(items);
	frameButtons->setPosition(CCPointZero);
	addChild(frameButtons,129);



	float posX_start = PosX_start;
	float posY = PosY;
	float distance = Distance;

// 	facebook->setPosition(ccp(posX_start,posY));
// 
// 	twitter->setPosition(ccp(posX_start+=distance,posY));
// 
// 	itemSina->setPosition(ccp(posX_start+=distance,posY));

	//game_center->setVisible(false);
	//game_center->setEnabled(false);
	resetGame->setPosition(ccp(posX_start+=distance,posY));

	leaderboardImage->setPosition(ccp(posX_start+=distance,posY));

	//exitImage->setPosition(ccp(980,posY));

	posY = 419;
	itemBackgroundSound->setPosition(ccp(980,posY));

	itemEffctiveSound->setPosition(ccp(980,posY-distance));
}

BaseMainOptionLayer::~BaseMainOptionLayer(void)
{
}

BaseMainOptionLayer* BaseMainOptionLayer::createLayer()
{
	BaseMainOptionLayer* layer = new BaseMainOptionLayer();
	layer->autorelease();
	return layer;
}


void BaseMainOptionLayer::menuUserInfoCallBack(CCObject* pObj)
{
// #if Version_Platform == 13 || Version_Platform == 12
//     RedStarSDKManager::getInstance()->userInfoBtnClick();
// #endif
}

void BaseMainOptionLayer::facebookClicked(CCObject* obj)
{
	Music::sharedMusicPlayer()->buttonClicked();

	int currentFinishedSeasonId = GameInforEditer::getGameInfor()->getLastPlayedSeasonId();
	int gameSeasonNum = GameLayerFactory::getInstance()->getValidSeasonNum();
	if (currentFinishedSeasonId < 1 )
	{
		currentFinishedSeasonId = 1;
	}
	if (currentFinishedSeasonId > gameSeasonNum)
	{
		currentFinishedSeasonId = gameSeasonNum;
	}

	if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	{
		LFShareKit::getKit()->share(kShareToFacebook,_shareToFB_TwitContent[currentFinishedSeasonId - 1]);
	}else
	{
		LFShareKit::getKit()->share(kShareToFacebook,_shareToFB_TwitContent[currentFinishedSeasonId - 1]);
	}
}
void BaseMainOptionLayer::twitterClicked(CCObject* obj)
{
	Music::sharedMusicPlayer()->buttonClicked();

	int currentFinishedSeasonId = GameInforEditer::getGameInfor()->getLastPlayedSeasonId();
	int gameSeasonNum = GameLayerFactory::getInstance()->getValidSeasonNum();
	if (currentFinishedSeasonId < 1 )
	{
		currentFinishedSeasonId = 1;
	}
	if (currentFinishedSeasonId > gameSeasonNum)
	{
		currentFinishedSeasonId = gameSeasonNum;
	}

	if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	{
		LFShareKit::getKit()->share(kShareToTwitter,_shareToFB_TwitContent[currentFinishedSeasonId - 1]);
	}else
	{
		LFShareKit::getKit()->share(kShareToTwitter,_shareToFB_TwitContent[currentFinishedSeasonId - 1]);
	}
}
void BaseMainOptionLayer::sinaClicked(cocos2d::CCObject *obj)
{
	Music::sharedMusicPlayer()->buttonClicked();
	if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	{
		LFShareKit::getKit()->share(kShareToSinaWeibo,_shareSinaContent_coded);
	}else
	{
		LFShareKit::getKit()->share(kShareToSinaWeibo,_shareToSinaContent);
	}
}

void BaseMainOptionLayer::soundClicked(CCObject* obj)
{
	Music::sharedMusicPlayer()->buttonClicked();

	if (Music::sharedMusicPlayer()->isBackgroundMute()	)
	{//
		itemBackgroundSound->initWithNormalImage(
			getSharedFilePath(g_buttonPath+"set_music_on.png").c_str(),
			getSharedFilePath(g_buttonPath+"set_music_on_select.png").c_str(),
			getSharedFilePath(g_buttonPath+"set_music_on.png").c_str(),
			this,SEL_MenuHandler(&BaseMainOptionLayer::soundClicked));
		Music::sharedMusicPlayer()->setBackgroundMute(false);
	}else
	{
		itemBackgroundSound->initWithNormalImage(
			getSharedFilePath(g_buttonPath+"set_music_off.png").c_str(),
			getSharedFilePath(g_buttonPath+"set_music_off_select.png").c_str(),
			getSharedFilePath(g_buttonPath+"set_music_off.png").c_str(),
			this,SEL_MenuHandler(&BaseMainOptionLayer::soundClicked));

		Music::sharedMusicPlayer()->setBackgroundMute();
	}

	GameInforEditer::getGameInfor()->saveSessonInforList();
}
void BaseMainOptionLayer::effctiveSoundClicked(CCObject* obj)
{
	Music::sharedMusicPlayer()->buttonClicked();
	if (Music::sharedMusicPlayer()->isEffectiveMute())
	{//
		itemEffctiveSound->initWithNormalImage(
			getSharedFilePath(g_buttonPath+"set_sound_on.png").c_str(),
			getSharedFilePath(g_buttonPath+"set_sound_on_select.png").c_str(),
			getSharedFilePath(g_buttonPath+"set_sound_on.png").c_str(),
			this,SEL_MenuHandler(&BaseMainOptionLayer::effctiveSoundClicked));
		Music::sharedMusicPlayer()->setEffectiveMute(false);
	}else
	{
		itemEffctiveSound->initWithNormalImage(
			getSharedFilePath(g_buttonPath+"set_sound_off.png").c_str(),
			getSharedFilePath(g_buttonPath+"set_sound_off_select.png").c_str(),
			getSharedFilePath(g_buttonPath+"set_sound_off.png").c_str(),
			this,SEL_MenuHandler(&BaseMainOptionLayer::effctiveSoundClicked));

		Music::sharedMusicPlayer()->setEffectiveMute();
	}

	GameInforEditer::getGameInfor()->saveSessonInforList();
}
//void BaseMainOptionLayer::inforClicked(CCObject* obj)
//{
//	Music::sharedMusicPlayer()->buttonClicked();
//}
void BaseMainOptionLayer::vedioClicked(CCObject* obj)
{
	Music::sharedMusicPlayer()->buttonClicked();
}
void BaseMainOptionLayer::settingClicked(CCObject* obj)
{
	Music::sharedMusicPlayer()->buttonClicked();
}

void BaseMainOptionLayer::mingxieItemClicked(cocos2d::CCObject *obj)
{
	Music::sharedMusicPlayer()->buttonClicked();
	CCTransitionFade* tran = CCTransitionFade::create(0.3f,TeamInfoLayer::scene());
	CCDirector::sharedDirector()->replaceScene(tran);
}

//点击退出按钮
void BaseMainOptionLayer::menuExitCallback(CCObject* pSender)
{
#if	CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	MessageDisplay*layer = MessageDisplay::create();
	layer->setComeFrom(from_ExitGame);
	CCDirector::sharedDirector()->getRunningScene()->addChild(layer, 100);
#else
	MessageDisplay*layer = MessageDisplay::create();
	layer->setComeFrom(from_ExitGame);
	CCDirector::sharedDirector()->getRunningScene()->addChild(layer, 100);
#endif
}
void BaseMainOptionLayer::menuLeaderboardCallback(CCObject* pSender)
{
	Music::sharedMusicPlayer()->buttonClicked();	
}
void BaseMainOptionLayer::gameCenterItemClick(cocos2d::CCObject *obj)
{
	Music::sharedMusicPlayer()->buttonClicked();
	LFShareKit::getKit()->showGameCenter();
}

void BaseMainOptionLayer::resetGameItemClick(cocos2d::CCObject *obj)
{
	Music::sharedMusicPlayer()->buttonClicked();
}


void BaseMainOptionLayer::actionEnd(float time)
{
	unschedule(schedule_selector(BaseMainOptionLayer::actionEnd));
	action = false;
}

void BaseMainOptionLayer::setItemDisable(bool able)
{
	frameButtons->setEnabled(able);
}

void BaseMainOptionLayer::hideAll(bool enable /* = false */)
{
	// 这里就应该每一个单独的hide和enable，这样子类就可以单独设置某一个元素的可见性
// 	twitter->setVisible(false);
// 	facebook->setVisible(false);
// 	itemSina->setVisible(false);
	leaderboardImage->setVisible(false);
	if (itemExit)
	{
		itemExit->setVisible(false);
		itemExit->setEnabled(false);
	}
	
	//game_center->setVisible(false);
	resetGame->setVisible(false);
	//exitImage->setVisible(false);
	itemBackgroundSound->setVisible(false);
	itemEffctiveSound->setVisible(false);
	setItem->setVisible(false);

	itemEffctiveSound->setEnabled(false);
	itemBackgroundSound->setEnabled(false);
	//exitImage->setEnabled(false);
	resetGame->setEnabled(false);
	//game_center->setEnabled(false);
	
	leaderboardImage->setEnabled(false);
// 	itemSina->setEnabled(false);
// 	facebook->setEnabled(false);
// 	twitter->setEnabled(false);
	setItem->setEnabled(false);

}

string BaseMainOptionLayer::getSharedFilePath(std::string str)
{
//	ResManager* resMgr = ResManager::getManager();
//	string str_new = resMgr->getSharedFilePath(str);
	string str_new = ResManager::getManager()->getSharedFilePath(str);
	return str_new;
}