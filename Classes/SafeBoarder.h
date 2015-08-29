#pragma once

#include "Box2D/Box2D.h"
#include "cocos2d.h"
using namespace cocos2d;

#define  Tag_SafeBoard_CoverLayer 9990241
#define Tag_SafeBoard_Neibu		  9990242

class SafeBoarder : public CCObject
{
private:
	b2World* mWorld;
	CCLayer* mLayer;
	// 
	CCLayer* mCoverLayer;
	vector<b2Body*> boaders;

	b2Body* createSide(CCPoint pos,float width,float angle,CCSprite* sprite,int TAG);


	void timeToCreateNodeBoarder(CCNode* not_use);

public:
	// 创建一个安全边框
	SafeBoarder(b2World* mWorld,CCLayer* layer);
	vector<b2Body*> getBoarders();
	~SafeBoarder(void);

	CCLayer* getCoverLayer(){return mCoverLayer;};
	void createNew();
};

class BaseContentLayer;
class SuperTool : public CCObject
{
private:
	CCSprite* effectTemplate;
	CCAction* action;
	b2World* mWorld;
	BaseContentLayer* mBaseLayer;
	b2Vec2 mCurrentBlowPos;
	bool mBlowing;
	vector<b2Body*> mSafeBoards;

	void resetFireItem(CCNode* sprite);

public:
	SuperTool(b2World* mWorld,BaseContentLayer* baseLayer);
	~SuperTool();

	void setBlowPos(b2Vec2 posInWorld);

	void setBorders(vector<b2Body*> safeBoards);

	// 吹
	void blow_tick(float applayTime);

	// 开始
	void start();
	// 停
	void stop();

	//清除点击的效果
	void clear();

	void createAndRunWaves();

	void createNew();
	void remove();
};

