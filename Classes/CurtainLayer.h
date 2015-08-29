//
//  BaseBackgroundLayer.h
//  RedStarFall
//
//  Created by Ryan.wu on 12-9-15.
//
//

#ifndef __CurtainLayer__
#define __CurtainLayer__

#include "cocos2d.h"
#include <string>
#include <list>
#include "ResManager.h"
#include "SpriteHelper.h"
#include "ICurtain.h"

using namespace cocos2d;
using namespace std;


class CurtainLayer : public CCLayer,public ICurtain
{
public:
	CurtainLayer(int seasonId,int sectionId,bool toClose = false);// 关闭  
	virtual void setAutoRemve(bool remove);
	virtual void action(CurtainLayerCallback* callback = NULL);
private:
	void initCurtain();
	void actionEnd(CCNode* node);
	void open();
	void close();

	CCSprite* leftCurtain;
	CCSprite* rightCurtain;
	CCLayer* hdLayerMask;

	bool autoRemove;
	bool toClose;
	int mSeasonId;
	int mSectionId;
	CurtainLayerCallback* callback;

};
#endif
