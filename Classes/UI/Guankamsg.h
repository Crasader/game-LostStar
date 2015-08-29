#pragma once
#include "cocos2d.h"
#include "cocos-ext.h"
#include "Baselisttable.h"
#include <vector>
#include "CmdHelper.h"
#include "SocialSeasonInfor.h"
#include "Uimsg.h"
#include "urlSprite.h"
#include "ShieldTouchlayer.h"
#include "Recorder.h"

using namespace std;
USING_NS_CC;
USING_NS_CC_EXT;
using namespace cocos2d::extension;


class Guankamsg: public CCLayer
{
public:
	Guankamsg(void);
	~Guankamsg(void);
	Guankamsg(int season,int gkid);
public:


	enum Gklayerenum{

		LianjieFb_tag=100,
		Start_gametag=101,
		Dingwei_tag,
		Returnxiaogk_tag,
		centeraction_tag,
		leftaction_tag,
		rightaction_tag,
		close_friendsolutiontag,
		list_tag,
	};


    static Guankamsg* createByid(int season,int gkid);
	CREATE_FUNC(Guankamsg);
	virtual bool init();  
	virtual void onEnter();
	virtual void onExit();
	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);

public:
	
	void snsgetData();
	void snsData(CCObject* obj);
	void shuaixinData();

	void snsgetsolutiondata();
public:
	void layerinit();
	void centeraction();
	void leftaction();
	void endaction();
    //donghua
	void Fadeinnodes(CCNode* node,int tag);
	void Fadeoutnodes(CCNode* node,int tag);
	void Fadeinnodes(CCNode* node,float time);
	void actionupdate(float time);
public:
	void fillOuty(CCMenuItemImage* p,string imgfile);
	void allCallback(CCObject* ob);

	void drawfriendlayer(msgSnsdata data);
	void zan_button(CCObject* obj);
	void clickisz(CCObject* obj);
	void solutionplay(CCObject* obj);

	//好友解法列表
	void  friendSolutionlist();
	void refreshSolution(CCObject* obj);

	void actionFaihui();
	void actionFaihuimain(CCNode* node);
	void Fadeoutnodes(CCNode* node);
	void Fadeinnodes(CCNode* node);
	//点赞回调
	void click_zan(CCObject* object);

	//
	bool getIsPlayRecord(){return m_bIsPlayRecord;};
	
private:
	CCSize size;
	int currnt_season;
	int currnt_gk;
	Baselisttable* list;
	//关卡信息
	vector<SectionInfor*> msg_setion;
	int maxsocore;
	const TaskInfor* info_layer;
	//sns 数据相关
	CCLabelTTF* lablen1;
	CCLabelTTF* lablen2;
	CCLabelTTF* lablen3;


	
	bool is_solution;
    
	CCLayer* gkmsg_layer;
	CCLayer* center_layer;
	CCPoint center_point;
	CCPoint centerstart_point;
	CCSprite* center_sp;

	CCLayer* left_layer;
	CCPoint  left_point;
	CCPoint  leftstart_point;

	CCLayer* leftcontent_layer;
	CCLayer* left_layerm;
	CCPoint  left_layermpoint;
	CCPoint  leftstart_layermpoint;

	CCLayer* right_layer;
	CCPoint  right_point;
	CCPoint  rightstart_point;
	//
	CCLabelTTF* gk_season;
	CCLabelTTF* label_startN;
	CCLabelTTF* label_startS;

	//动作时间管理
	float center_time;
	float left_time;
	float rght_time;
	CCLayerColor* color;

	int num_time;
	int all_time;

	int back_time;
	int first_actiontime;
	int bt_time;
	int xinxin_time;
	int text_time;
	int butoon_time;
	int dom_buttontime;
	int newcorlds_time;



	
	
	
	
	bool isclickZ;

	//
	vector<vector<record> > m_allRecordData;
	vector<msgSnsdata> dataUnion;
	CCObject* freind_solution;
	msgSnsdata data_play;
	bool isgklayer;
	int result_idx;

	bool m_bIsPlayRecord;
};

