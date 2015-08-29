#ifndef __VersionControl_H__
#define __VersionControl_H__



//当前版本号
//（从1.1开始，每次游戏启动，会检测当前版本号与配置文件中是否一致，不一致会清空所有数据）
#define Current_Version_Code "1.0"

#include "cocos2d.h"

using namespace cocos2d;

#define marketlayer_tag					2233
#define delayedTime						3.0f

#define Version_Free					0

#if Version_Free != 0
#define Show_End_BigGuanQia					1
#else
#define Show_End_BigGuanQia					0
#endif

enum _Version_Platform
{
    CMGE_Version							= 0,        //中手游版本(包括安卓版本和IOS正版)
    LianTong_Wo_Version						= 1,        //联通沃商店
    LianTong_SMS_Version					= 2,        //联通短信      (暂未使用)
    YiDong_YouXiJiDi_Version				= 3,        //移动游戏基地
    DianXin_AiYouXi_Version					= 4,        //电信爱游戏
    YiDong_MM_Version						= 5,        //移动MM
    DianXin_TianYi_Version					= 6,        //电信天翼空间   (暂未使用)
    LianTong_KuanDai_Version				= 7,        //联通宽带
    IOS_Unlock_CMGE_Version					= 8,        //IOS越狱_中手游
    Android_QuDaoBao_Test_Version			= 9,        //安卓渠道审核包
    IOS_91_Version							= 10,       //IOS越狱_91平台
    IOS_PP_Version							= 11,       //IOS越狱_PP助手平台
    IOS_KuaiYong_Version					= 12,       //IOS越狱_快用平台
    IOS_iTools_Version						= 13,       //IOS越狱_iTools平台
	Android_QuDaoBao_Release_Version		= 14,        //安卓渠道正式包
};

//定义平台
#define Version_Platform         5

//const static int Version_Platform = IVersion_Platform;

// 控制
#define Version_iOS91Copy				0		
#define Version_Lite					0		
#define Android_Tag_YeahMobi			1

#define Limit_Section_Max_Play_Num		0					// 限制每一个section的次数

// 关卡解锁 : 优先级从上到下，越来越低
#define NOT_AutoUnlockSeason			0					// 不直接解锁season，也就是season不会自动解锁，需要购买
#define StarNum_Unlock_Season			0					// 使用星星自动解锁Season
#define LAST_SEASON_UNLOCK_NEW_SEASON	1					// 上一章所有关卡都成功了，才解锁下一章
#define COLLECTION_ALL_SPECIAL_ITEM     0                   // 上一张所有道具收集了，才解锁下一章


// 主题
#define Theme_StarToUnlock				0
#define Theme_PayToUnlock				1
#define Current_SeasonItem_Theme		Theme_PayToUnlock	// season的主题是用点击解锁的形象，点击之后就会弹出对话框解锁


// 计费点设置
#define Not_Need_Buy_Season				1	// 不需要购买season的选项


#define	JUST_EN							0	// 只是英文版
#define JUST_ZH							1	// 只是中文版
#define AUTO_ADJUST_SCREEN				1	// 自动适配屏幕
#define USE_DOOR_REPLACE_CURTAIN		1	// 使用门来代替窗帘


// 支付的版本控制
// 就是没有，就直接选择
// 如果要指定某个支付方式，那么就直接写对应支付方式的的枚举即可
//enum PaymentTag
//{
//	PT_YEAH_MOBI_SMS		= 0,			// YeahMobi短信支付
//	PT_UNIPAY_SDK			= 1,			// 联通的SDK支付
//	PT_UNISMSPAY_SDK		= 2,			// 联通短信的SDK支付
//	PT_YOUXIJIDI_SDK		= 3,			// 游戏基地SDK支付
//	PT_EGameSMSOffLine		= 4,			// 电信
//	PT_MMSMSBillingSDK		= 5,			// MM
//	PT_CTEStoreSDKLocalSms	= 6,			// 天翼空间支付
//};


// 是否需要游戏中的CD，1是需要，0是不需要
#define NEED_GAME_CD_TIME	0

// 超能道具形式
// 1.类似割绳子的形式
// 2.直接停掉物理效果，直接拖动红星到指定的位置
#define SUPER_TOOL_TYPE_LIKECUTROPE 1
#define SUPER_TOOL_TYPE_NO_PHYSICS  2

#define SUPER_TOOL_TYPE SUPER_TOOL_TYPE_LIKECUTROPE

// 关于一些外界的按钮设置
// 游戏菜单:更多游戏、游戏帮助、游戏关于、退出游戏

#define NeedHelpButton		1

#if  Version_Platform == 11 || Version_Platform == 12 || Version_Platform == 13
#define NeedUserInfoBtn     1
#else
#define NeedUserInfoBtn     0
#endif

//IOS不显示退出按钮，和更多按钮
//电信和移动基地显示更多按钮，其他不显示
//运营商不显示分享，c-talk和广告，其它显示
#if Version_Platform == 9 || Version_Platform == 10 || Version_Platform == 11 \
	|| Version_Platform == 12 || Version_Platform == 13 || Version_Platform == 14 \
	|| (Version_Platform > 0 && Version_Platform < 8) 
//运营商包
    #if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
        #if (Version_Platform == 3) || (Version_Platform == 4)
        //更多游戏按钮
            #define NeedMoreGameButton	1
        #else
            #define NeedMoreGameButton	0
        #endif
    #else
        #define NeedMoreGameButton	0
    #endif
    //退出游戏按钮,GameCenter
    #if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
        #define NeedExitButton		0

        #if (Version_Platform == 0)
            #define GameCenter_Enable   0
        #else
            #define GameCenter_Enable   0
        #endif
    #else
        #define NeedExitButton		1
    #endif
#else
//中手游，IOS正版，IOS越狱
    #define NeedMoreGameButton	0
//如果是IOS则不显示退出按钮，IOS正版不显示c_talk按钮
    #if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
        #define NeedExitButton		0
    #else
        #define NeedExitButton		1
    #endif
#endif


#endif //__VersionControl_H__