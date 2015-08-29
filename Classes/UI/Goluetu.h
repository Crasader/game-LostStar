#pragma once
#include "cocos2d.h"
#include "cocos-ext.h"
#include "Stopgameright.h"
#include "Recorder.h"
#include "Baselisttable.h"
#include "urlSprite.h"
#include "ShieldTouchlayer.h"

enum Goluetunum
{
	close_gltutag=1000,
	glutulayer_tag,
	freindlayer_tag,
	guanfalayer_tag,
	chankanguanfangjiefa_tag,
	close_gfgltutag,
	gflayertag,
	

};
using namespace std;
USING_NS_CC;

class Goluetu :public CCLayer{
public:
	Goluetu(void);
	Goluetu(bool mark);
	~Goluetu(void);
	CREATE_FUNC(Goluetu);
	virtual bool init();  
	virtual void onEnter();
	virtual void onExit();
	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);

	static Goluetu*  creteBytag(bool mark);
	void Golutugailayer();

	void snsData(CCObject* obj);
	void snsgetData();
	void shuaixinData();
	void son_praent(CCObject *object);
	void actionF();
	void actiongf();
	void answerchangeyuan();
	void gfchangemain();
	void drawPlaylayer(msgSnsdata data);
	void zan_button(CCObject* object);
	void fillOuty(CCMenuItemImage* p,string imgfile);
	void allCallback(CCObject *object);

	void click_zan(CCObject* object);
	void clickisz(CCObject* object);

	void refreshSolution(CCObject* object);
	void chankangfjieFa(CCObject* object);
	void allCallback_buy(CCObject* object);
	void refreshgfjiefa();

	void payResult(CCObject* pMsg);

	void loginfaceBook(CCObject* ob);
	void logincallback(CCObject* ob);
	void drawguanfalayer();

	void addplaygf();

	void drawlogin();

	void refreshloginlayer();

#if 0
	CCMenu* createInviteButton(CCNode* parent);
	void onInviteClick(CCObject* obj);
	void loginResult(CCObject* pMsg);
#endif

public:

	void drawGongluetu();

	void gLuetFh();
	void Fadeinnodes(CCNode* node);
	void Fadeoutnodes(CCNode* node);
	//void setStopgameright(Stopgameright* p);

	void setAllRecordData(vector<vector<record> >* pAllData)
	{
		if (!pAllData)
		{
			return;
		}
		m_allRecordData = *pAllData;
	}
	
private:

	CCSize size;
	CCSize playersize;
	cocos2d::extension::CCScrollView* pPlayRecordLayer_Official;
	Stopgameright* game;
	//记录的数据，播放的时候使用
	vector<vector<record> > m_allRecordData;

	CCLayer* goltu_layer;
	CCScale9Sprite* back;

	CCLayer* clickgflayer;

	CCLayer* gflayer;

	Baselisttable* list_friend;
	//CCMenu*			_inviteButton;
	const TaskInfor* info_layer;
	bool mark_sns;
	CCObject* freind_solution;
	bool is_answer;
	ShieldTouchlayer* answer;

	CCLabelTTF* name_label;
	CCMenuItemImage* z_ccbt;
	CCLabelTTF* z_label;
	urlSprite* toux_sp;
    
	int current_layermark;
	bool iscloseactiondone;
	bool isgolutuactiondone;
	bool isclickZ;
	bool isgfjiefa;

	CCLayer* loginlayer;

	CCMenuItemImage*  startmenu_l;
	CCMenuItemImage* back_gfhave;//有官方解法时
	CCSprite* back_nogfslution;//没有有官方解法时
	bool isliandian;

	bool isStoplayer;

	msgSnsdata recorld_data;
	vector<msgSnsdata> dataUnion;
};
