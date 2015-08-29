#pragma once

#include "cocos2d.h"
#include "cocos-ext.h"
#include "Stopgameright.h"
#include "Baselisttable.h"
#include "urlSprite.h"
#include "Recorder.h"

using namespace std;
USING_NS_CC;


class  Friendmethod :public CCLayer{
public:
	Friendmethod(void);
	~Friendmethod(void);
	CREATE_FUNC(Friendmethod);
	virtual bool init();  
	virtual void onEnter();
	virtual void onExit();
	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);

public:

	void drawfriendlayer();
	void actionFaihui();
	void playFh();

	void son_praent(CCObject *object);
	void drawPlaylayer(msgSnsdata data);

	void play_button(CCObject* obj);
	void zan_button(CCObject* obj);

	void Fadeinnodes(CCNode* node);
	void Fadeoutnodes(CCNode* node);

	//void setStopgameright(Stopgameright* p);

	void refreshSolution(CCObject* object);
	void refreshClickz(CCObject* object);
	
public:
	//sns 数据
	void snsgetData();
	void snsData(CCObject* obj);
	void shuaixinData();
	void clickisz(CCObject* obj);
private:

	CCLabelTTF* name_label;
	CCLabelTTF* z_label;
	CCMenuItemImage* z_ccbt;
	urlSprite* toux_sp;
	//Baselisttable* list_friend;
	Baselisttable* list_friend;
	CCSize size;
	CCScale9Sprite* back;
	int mark;
	CCLayer* answer;
	CCMenuItemImage* play_bt;
	bool is_answer;
	int who_layer;
	const TaskInfor* info_layer;
	Stopgameright* game;



	vector<vector<record> > m_allRecordData;
	
	bool mark_sns;

	CCObject* freind_solution;
	bool isclickZ;
	
};

