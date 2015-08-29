
#ifndef __LoadingLayer_H__
#define __LoadingLayer_H__

#include "LFLayer.h"
#include "TexturePreLoad.h"
#include "cocos-ext.h"

using namespace cocos2d;

enum LoadingIntent
{
	Intent_AppDelegate2Main,	// AppDelegate到Main，第一次启动
	Intent_Section2GameIn,		// section选择界面到游戏内部
	Intent_GameIn2Section,		// 游戏内部到Section选择页面
	Intent_GameIn2Main,			// 游戏内部到Main
	Intent_GameIn2Season,		// 游戏内部到Season
};


class LoadingLayer : public LFLayer,TexturePreLoadCallBack
{
private:
	CCSprite* barMoveSide;
	int needLoadImageNum;
	LoadingIntent mIntent;
	int mSeasonId;
	vector<string> loadingTips;

	bool			m_bAnimationOver;
 	bool			m_bTextureLoadingOver;

	//检测版本号，不对应则清空数据（...用户会很伤心的$_$）
	void checkVersionCode();

	void changeTip(float time);

	void startOuterLoading(float time);
	void loadOuterEffectiveSound();
	void loadInnterEffectiveSound();
	void loadingOuterOver(CCNode* node);
	void goToSectionPage(float time);
	void goToSeasonPage(float time);

	void startInnerLoading(float time);
	void loadingInnerOver();
	void goToSection(float time);		// 进入游戏


	void createInnerLoading();
	void createOutterLoading();


	void outlayerLoadover(CCNode* pNode);

	// 回调接口
	void textureLoaded(int waitingTaskNum,string fileName);
	void loadOver();
public:
	LoadingLayer(LoadingIntent intent,int seasonId = -1);
    
    void goToMainPage(float time);
};

class LaunchIconLayer : public CCLayer
{
public:
	enum iTag
	{
        iTag_LaunchIconLayer = 10,
        iTag_LoadingLayer,
		iTag_LayerColor,
		iTag_Logo,
		iTag_Logo_Armature,
	};

public:
	LaunchIconLayer();
	~LaunchIconLayer();

	float m_fDelaytime;
	std::vector<string>		m_LogoFileNameVec;
    
    void startAnimation();
    void goToMainLayer();

	void startTrueLoad(CCNode* null_obj);
	void removeNode(CCNode* node);
	void oneLogoShowDone(CCNode* pNode);

	void MovementEventCallFunc(cocos2d::extension::CCArmature *pArmature, cocos2d::extension::MovementEventType movementType, const char *movementID);
    
    void play(float dt);
};


#endif