/*
 *如果是iOS,请将.cpp改成.mm  不然会报错
 *if is iOS,please replace ".cpp" to ".mm"
 */
#include "LFShareKit.h"
#include <stdlib.h>
#include "VersionControl.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import "GameKitHelper.h"
#endif
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "jni/com_ican_cocos2dx_jni_LFCocos2dxHelper.h"
extern void openURLByJava(const char* url,int urlLength);
#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#include "windows.h"
#endif

LFShareKit* LFShareKit::kit;

LFShareKit::LFShareKit(void)
{
	enableGameCenter = true;

	// lite 和 91 的都不用GameCenter
#if (Version_Lite == 1 || Version_iOS91Copy == 1)
	enableGameCenter = false;
#endif
    
#if GameCenter_Enable != 0
    enableGameCenter = true;
#else
    enableGameCenter = false;
#endif

	if (enableGameCenter)
	{
		gameCenterAuthenticateLocalUser();
	}
}

LFShareKit::~LFShareKit(void)
{
}

LFShareKit* LFShareKit::getKit()
{
	if(kit == NULL)
		kit = new LFShareKit();
	return kit;
}
bool LFShareKit::isGameCenterEnabled()
{
	return enableGameCenter;
}
void LFShareKit::share(ShareTo shareto,string content)
{
	switch(shareto)
	{
	case kShareToFacebook:
		openURl(facebookBaseShareUrlPre+content+facebookBaseShareUrlPreEnd);
		break;
	case kShareToTwitter:
		openURl(twitterBaseShareUrl+content);
		break;
	case kShareToSinaWeibo:
		openURl(sinaBaseShareUrl+content);
		break;
	case kShareToRenren:
		openURl(renrenBaseShareUrl+content);
		break;
	case kShareToTencent:
		openURl(tencentBaseShareUrl+content);
		break;
	case kShareToAll:
		break;
	}
}

void LFShareKit::share(ShareTo shareto)
{
	share(shareto,lfShareContent);
}

void LFShareKit::gameCenterAuthenticateLocalUser()
{
	if (!enableGameCenter)
		return;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	//验证用户
	[[GameKitHelper sharedGameKitHelper] authenticateLocalUser];
#endif
}

void LFShareKit::showGameCenter()
{
	if (!enableGameCenter)
		return;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	[[GameKitHelper sharedGameKitHelper] showLeaderboard];
#endif
}

void LFShareKit::showGameCenterAchievement()
{
	if (!enableGameCenter)
		return;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	[[GameKitHelper sharedGameKitHelper] showAchievements];
#endif
}

void LFShareKit::reportScoreToGameCenter(float score,string category)
{
	if (!enableGameCenter)
		return;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	NSString *str=[[NSString alloc] initWithUTF8String:category.c_str()];
	[[GameKitHelper sharedGameKitHelper] reportScore:score forCategory:str];

	[str release];
#endif
}

void LFShareKit::reportAchievementToGameCenter(string identifier,float percent)
{
	if (!enableGameCenter)
		return;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	NSString *str=[[NSString alloc] initWithUTF8String:identifier.c_str()];
    [[GameKitHelper sharedGameKitHelper] reportAchievementWithID:str percentComplete:percent];    
	[str release];
#endif
}

void LFShareKit::openURl(string url)
{
    //    {
    //        url = "itms-apps://phobos.apple.com/WebObjects/MZStore.woa/waiewSoftware?id=627200252";
    //    }
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

	NSString *str=[[NSString alloc] initWithUTF8String:url.c_str()];

	NSURL *_url = [[NSURL alloc] initWithString:[str stringByAddingPercentEscapesUsingEncoding:NSUTF8StringEncoding]];

	[[UIApplication sharedApplication] openURL:_url];
  
    [_url release];
	[str release];
#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	openURLByJava(url.c_str(),url.length());
#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)

	const char* u = url.c_str();
	//确定转换为Unicode需要多少缓冲区(返回值也包含了最后一个NULL字符)。
	int nLen = MultiByteToWideChar(CP_ACP, 0, u, -1, NULL, 0);
	WCHAR* wBuf = new WCHAR[nLen];
	MultiByteToWideChar(CP_ACP, 0, u, -1, wBuf, nLen);
	ShellExecute(NULL, L"open",wBuf, NULL, NULL, SW_SHOWNORMAL); 
	delete [] wBuf;
	//ShellExecute(NULL, L"open",L"www.google.com", NULL, NULL, SW_SHOWNORMAL); 
	//string str = "start "+url;
	//system(str.c_str());
#endif

}
