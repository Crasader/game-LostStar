
#ifndef __LOGO_H__
#define __LOGO_H__

#include "cocos2d.h"
#include "CommonDefine.h"
#include "BaseGuide.h"
#include "GameInforEditer.h"

using namespace cocos2d;

class Logo : public CCLayer
{
public:
	//创建一个Logo的场景
	static CCScene* createLogoScene();
    
	~Logo();
private:
	Logo();
	static Logo* createLayer();
	//每次create都会调用
	void initLogoLayer();

    //显示某一个字符
    void showChar(CCObject* charactor);
    
	void logoEnd(float time);
	//开始播放动画
	void playAction(float time);
    
    CCProgressTimer* createProgressTimer(const char* fileName);
	CCAction* action;
	CCSprite* logoSpr;

};



#endif