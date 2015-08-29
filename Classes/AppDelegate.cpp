#include "cocos2d.h"
#include "CCEGLView.h"
#include "AppDelegate.h"
#include "SimpleAudioEngine.h"
#include "StringIntMaper.h"
#include "ResManager.h"
#include "Music.h"
#include <vector>
#include "BaseContentLayer.h"
#include "LoadingLayer.h"
#include "GuideLayer.h"
#include "LFFileUtiles.h"
#include "VersionControl.h"
#include "AppMacros.h"
#include "History.h"
#include "FXPlugin/FXPluginManager.h"


using namespace cocos2d;
using namespace CocosDenshion;

USING_NS_CC;
bool g_IsRetina = false;
char g_BoundleResourcePath[256];
extern std::vector<string> g_NeedBlurFileNameVec;

#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
#pragma comment(lib, "libprotobuf-lite")
#endif

AppDelegate::AppDelegate()
{
}

AppDelegate::~AppDelegate()
{
    SimpleAudioEngine::end();
/*	Sqlite3Helper::getHelper()->closeDb(); */
}

bool AppDelegate::applicationDidFinishLaunching()
{
	CCLog("===================================================================");

//	setMachinePerformanceLevel(RedStarSDKManager::getInstance()->getTotalMemory());

	CCLog("%s : -----------> start",__FUNCTION__);
	// initialize director
	CCDirector *pDirector = CCDirector::sharedDirector();
	CCEGLView* pEGLView = CCEGLView::sharedOpenGLView();
	pDirector->setOpenGLView(CCEGLView::sharedOpenGLView());

	//sdk准备
	FX::PluginManager::getInstance()->loadPlugin();

	// Set the design resolution
#if AUTO_ADJUST_SCREEN == 1
	pEGLView->setDesignResolutionSize(
		designResolutionSize.width, 
		designResolutionSize.height, 
		kResolutionFixedHeight); // 要适配所有的分辨率，那么图片应该是1382 * 768 的 ---> 1152 * 640

#else
	pEGLView->setDesignResolutionSize(
		designResolutionSize.width, 
		designResolutionSize.height, 
		kResolutionShowAll); // 要适配所有的分辨率，那么图片应该是1382 * 768 的 ---> 1152 * 640

#endif

	//pEGLView->setDesignResolutionSize(
	//	designResolutionSize.width, 
	//	designResolutionSize.height, 
	//		kResolutionExactFit);

	CCSize frameSize = pEGLView->getFrameSize();

	vector<string> searchPath;

	searchPath.push_back(mediumResource.directory);
	pDirector->setContentScaleFactor(MIN(ipadResource.size.height/designResolutionSize.height, ipadResource.size.width/designResolutionSize.width));

	searchPath.push_back(mediumResource.directory+string("/ui"));
	searchPath.push_back(mediumResource.directory+string("/scene"));
	searchPath.push_back(mediumResource.directory+string("/item"));
	searchPath.push_back(mediumResource.directory+string("/character"));
	searchPath.push_back(mediumResource.directory+string("/animation"));
	searchPath.push_back(mediumResource.directory+string("/aa"));

	// set searching path
	CCFileUtils::sharedFileUtils()->setSearchPaths(searchPath);


//     // 开屏广告
//     AdManager::getManager()->adShowSplashAd();
// 	// 预加载插屏广告
// 	AdManager::getManager()->adPreLoadInterstitialAd();
// 	// 显示横幅广告
// 	AdManager::getManager()->adShowBannerAd();
    
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
	pDirector->setDisplayStats(true);
#endif

// 	// 初始话
// 	GlobalNetworkMsgDispacher::getMsgDispacher();
// 	NetWorkThreadProc::getProc();
// 	OtherNetworkThreadProc::getProc();

	CCScene* scene = CCScene::create();
	LaunchIconLayer* iconLayer = new LaunchIconLayer();
    scene->addChild(iconLayer,1, LaunchIconLayer::iTag_LaunchIconLayer);
    iconLayer->release();
    pDirector->runWithScene(scene);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void appDelegate_applicationDidEnterBackground()
{
	CCDirector::sharedDirector()->stopAnimation();

	if (CC_TARGET_PLATFORM != CC_PLATFORM_IOS)
	{

		//不是静音的情况下才暂停
		bool isBgMute = Music::sharedMusicPlayer()->isBackgroundMute();
		if (!isBgMute)
		{
			SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
		}
		bool isEffectMute = Music::sharedMusicPlayer()->isEffectiveMute();
		if (!isEffectMute)
		{
			Music::sharedMusicPlayer()->setEffectiveMute(isEffectMute);
		}

		Music::sharedMusicPlayer()->setBackgroundMuteBeforeBackGround(isBgMute);
		Music::sharedMusicPlayer()->setEffectMuteBeforeBackGround(isEffectMute);
	}

// 	//小内存机器不需要退出保存数据（因为会出现把infor.txt写成空文件的情况）
// 	if (getMachinePerformanceLevel() > 0)
// 	{
// 		//保存数据
// 		GameInforEditer::getGameInfor()->saveSessonInforList();
// 	}

	History::getHistory()->save();
//	OrderList::save();
}

void AppDelegate::applicationDidEnterBackground()
{
	appDelegate_applicationDidEnterBackground();
}


// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground()
{
    CCDirector::sharedDirector()->startAnimation();

	if (CC_TARGET_PLATFORM != CC_PLATFORM_IOS)
	{
		bool isBgMute = Music::sharedMusicPlayer()->getBackgroundMuteBeforeBackGround();
		bool isEffectMute = Music::sharedMusicPlayer()->getEffectMuteBeforeBackGround();
		Music::sharedMusicPlayer()->setBackgroundMute(isBgMute);
		Music::sharedMusicPlayer()->setEffectiveMute(isEffectMute);
		if (!isBgMute)
		{
			SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
		}
	}

}
